// a asynchronous TCP daytime server
#include <boost/asio.hpp>
#include <iostream>
#include <string>
#include <functional>
#include <ctime>
#include <memory>
using boost::asio::ip::tcp;

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

int main()
{
    try
    {
        int num = 0;
        std::cout << num++ << std::endl; 
        boost::asio::io_context io;
        tcp_server server(io);
        io.run();
    }
    catch(std::exception&e)
    {
        std::cerr << e.what() << std::endl;
    }


    return 0;
}
