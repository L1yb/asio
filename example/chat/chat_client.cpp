#include <iostream>
#include <cstdlib>
#include <boost/asio.hpp>
#include "chat_message.hpp"
#include <thread>
#include <deque>

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_deque;

class chat_client 
{
public:
    chat_client(boost::asio::io_context &io_context,
                const tcp::resolver::results_type &endpoints)
        : io_context_(io_context), socket_(io_context_)
    {
        do_connect(endpoints);
    }
    void write(const chat_message &msg)
    {
        boost::asio::post(io_context_, [this, msg]()
                          {
            bool write_in_progress = !write_msgs_.empty();
            write_msgs_.push_back(msg);
            if(!write_in_progress)
            {
                do_write();
            } });
    }
    void close()
    {
        boost::asio::post(io_context_, [this]()
                          { socket_.close(); });
    }

private:
    void do_connect(const tcp::resolver::results_type &endpoints)
    {
        boost::asio::async_connect(socket_, endpoints,
                                   [this](boost::system::error_code ec, tcp::endpoint)
                                   {
                                       if (!ec)
                                       {
                                           do_read_header();
                                       }
                                   });
    }
    void do_read_header()
    {
        boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                                [this](boost::system::error_code ec, std::size_t)
                                {
                                    if (!ec && read_msg_.decode_header())
                                    {
                                        do_read_body();
                                    }
                                });
    }
    void do_read_body()
    {
        boost::asio::async_read(socket_,
                                boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
                                [this](boost::system::error_code ec, std::size_t)
                                {
                                    if (!ec)
                                    {
                                        std::cout.write(read_msg_.body(), read_msg_.body_length()) << std::endl;
                                        do_read_header();
                                    }
                                });
    }

    void do_write()
    {
        boost::asio::async_write(socket_,
                                 boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
                                 [this](boost::system::error_code ec, std::size_t)
                                 {
                                     if (!ec)
                                     {
                                         write_msgs_.pop_front();
                                         if (!write_msgs_.empty())
                                         {
                                             do_write();
                                             std::cout << "do write\n";
                                         }
                                     }
                                     else
                                     {
                                         socket_.close();
                                     }
                                 });
    }

private:
    boost::asio::io_context &io_context_;
    tcp::socket socket_;
    chat_message read_msg_;
    chat_message_deque write_msgs_;
};

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: client <host> <port>\n";
        return 1;
    }

    try
    {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(argv[1], argv[2]);
        chat_client client(io_context,endpoints);
        std::thread t([&io_context](){io_context.run();});

        char line[chat_message::max_body_length + 1];
        while (std::cin.getline(line, chat_message::max_body_length + 1))
        {
            chat_message msg;
            msg.body_length(std::strlen(line));
            std::memcpy(msg.body(), line, msg.body_length());
            msg.encode_header();
            client.write(msg);
        }
        client.close();
        t.join();
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    return 0;
}
