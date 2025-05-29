/*a combined TCP/UDP asynchronous server*/
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <ctime>
#include <functional>
#include <memory>

using boost::asio::ip::tcp;
using boost::asio::ip::udp;
std::string make_daytime_string()
{
    using namespace std;
    time_t now = time(0);
    return ctime(&now);
}

class tcp_connection : public std::enable_shared_from_this<tcp_connection>
{
public:
    typedef std::shared_ptr<tcp_connection> pointer;
    static pointer create(boost::asio::io_context& io)
    {
        return pointer(new tcp_connection(io));
    }
    tcp::socket& socket()
    {
        return socket_;
    }

    void start()
    {
        message_ = make_daytime_string();
        boost::asio::async_write(socket_, boost::asio::buffer(message_),
                                std::bind(&tcp_connection::handle_write, shared_from_this() ,
                                boost::asio::placeholders::error, 
                                boost::asio::placeholders::bytes_transferred));
    }


private:
    tcp_connection(boost::asio::io_context& io) : socket_(io)
    {

    }
    void handle_write(const boost::system::error_code& e, size_t size)
    {
        if (!e)
        {
            std::cout << size << std::endl;
        }
    }
    tcp::socket socket_;
    std::string message_;
};

class tcp_server
{

public:
    tcp_server(boost::asio::io_context& io) : io_(io), acceptor_(io, tcp::endpoint(tcp::v4(), 13))
    {
        start_accept();
    }

private:
    void start_accept()
    {
        tcp_connection::pointer new_connection = tcp_connection::create(io_);
        acceptor_.async_accept(new_connection->socket(), 
                                std::bind(&tcp_server::handle_accept, this, new_connection,
                                        boost::asio::placeholders::error));
    }

    void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error)
    {
        if(!error)
        {
            new_connection->start();
        }
        start_accept();
    }

    boost::asio::io_context& io_;
    tcp::acceptor acceptor_;
};


class udp_server
{
public:
  udp_server(boost::asio::io_context& io_context)
    : socket_(io_context, udp::endpoint(udp::v4(), 13))
  {
    start_receive();
  }

private:
  void start_receive()
  {
    socket_.async_receive_from(
        boost::asio::buffer(recv_buffer_), remote_endpoint_,
        std::bind(&udp_server::handle_receive, this,
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
  }

  void handle_receive(const boost::system::error_code& error,
      std::size_t /*bytes_transferred*/)
  {
    if (!error)
    {
      std::shared_ptr<std::string> message(
          new std::string(make_daytime_string()));

      socket_.async_send_to(boost::asio::buffer(*message), remote_endpoint_,
          std::bind(&udp_server::handle_send, this, message,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

      start_receive();
    }
  }

  void handle_send(std::shared_ptr<std::string> /*message*/,
      const boost::system::error_code& /*error*/,
      std::size_t /*bytes_transferred*/)
  {
  }

  udp::socket socket_;
  udp::endpoint remote_endpoint_;
  std::array<char, 1> recv_buffer_;
};
int main()
{
    try
    {
        boost::asio::io_context io_context;
        tcp_server _tcp_server(io_context);
        udp_server _udp_server(io_context);
        io_context.run();
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
