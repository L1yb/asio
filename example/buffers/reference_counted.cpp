#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <utility>
#include <vector>
#include <ctime>
#include <string>

using boost::asio::ip::tcp;
class shared_const_buffer
{
public:
    explicit shared_const_buffer(const std::string& data)
    : data_(new std::vector<char>(data.begin(), data.end())),
      buffer_(boost::asio::buffer(*data_))
    {

    }
    typedef boost::asio::const_buffer value_type;
    typedef const boost::asio::const_buffer* const_iterator;
    const boost::asio::const_buffer* begin() const { return &buffer_;}
    const boost::asio::const_buffer* end() const { return & buffer_ + 1;}

private:
    std::shared_ptr<std::vector<char>> data_;
    boost::asio::const_buffer buffer_;
};

class session : public std::enable_shared_from_this<session>
{
public:
    session(tcp::socket socket) : socket_(std::move(socket))
    {

    }
    void start()
    {
        do_write();
    }


private:
    void do_write()
    {
        std::time_t now = std::time(0);
        shared_const_buffer buffer(std::ctime(&now));
        auto self(shared_from_this());
        boost::asio::async_write(socket_, buffer, [self](boost::system::error_code , std::size_t){

        });
    }

    tcp::socket socket_;
};

class server
{
public:
    server(boost::asio::io_context& io_context, short port)
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
    {
        do_accept();
    }

private:
    void do_accept()
    {
        acceptor_.async_accept([this](boost::system::error_code ec, tcp::socket socket)
        {
            if(!ec)
            {
                std::make_shared<session>(std::move(socket))->start();
            }
            do_accept();
        });
    }

    tcp::acceptor acceptor_;
};

int main(int argc, char* argv[])
{
    try
{    if (argc < 2)
    {
        std::cerr << "Usage: server <port>\n";
        return 1;
    }

    boost::asio::io_context io_context;
    server server(io_context, std::atoi(argv[1]));
    io_context.run();}
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}
