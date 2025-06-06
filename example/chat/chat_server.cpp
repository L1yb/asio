#include "chat_message.hpp"
#include <iostream>
#include <list>
#include <memory>
#include <set>
#include <utility>
#include <deque>
#include <boost/asio.hpp>
using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_deque;

class chat_participant
{
public:
    virtual ~chat_participant()
    {
    }

    virtual void deliver(const chat_message& msg) = 0;
};

typedef std::shared_ptr<chat_participant> chat_participant_ptr;

class chat_room
{
public:
    void join(chat_participant_ptr participant)
    {
        participants_.erase(participant);
        for (auto msg : recent_msgs_)
        {
            participant->deliver(msg);
        }
    }

    void leave(chat_participant_ptr participant)
    {
        participants_.erase(participant);
    }

    void deliver(const chat_message& msg)
    {
        recent_msgs_.push_back(msg);
        while (recent_msgs_.size() > max_recent_msgs)
            recent_msgs_.pop_front();

        for (auto participant : participants_)
            participant->deliver(msg);
    }

private:
    std::set<chat_participant_ptr> participants_;
    enum { max_recent_msgs = 100 };
    chat_message_deque recent_msgs_;
};

class chat_session : public chat_participant,
                     public std::enable_shared_from_this<chat_session>
{
public: 
    chat_session(tcp::socket socket, chat_room& room)
        :socket_(std::move(socket)), room_(room)
    {
    }

    void start()
    {
        room_.join(shared_from_this());
        do_read_header();
    }

    void deliver(const chat_message& msg)
    {
        bool write_in_progress = !write_msgs_.empty();
        write_msgs_.push_back(msg);
        if (!write_in_progress)
        {
            do_write();
        }
    }

private:
    void do_read_header()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.data(), chat_message::header_length),
                                [this, self](boost::system::error_code ec, std::size_t)
                                {
                                    if (!ec && read_msg_.decode_header())
                                    {
                                        do_read_body();
                                        std::cout.write(read_msg_.data(), chat_message::header_length) << std::endl;
                                    }
                                    else
                                    {
                                        room_.leave(shared_from_this());
                                    }
                                });
    }
    void do_read_body()
    {
        auto self(shared_from_this());
        boost::asio::async_read(socket_, boost::asio::buffer(read_msg_.body(), read_msg_.body_length()),
        [this, self](boost::system::error_code ec, std::size_t)
        {
            if (!ec)
            {
                std::cout.write(read_msg_.body(), read_msg_.body_length()) << std::endl;
                room_.deliver(read_msg_);
                do_read_header();
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
    }


    void do_write()
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(write_msgs_.front().data(), write_msgs_.front().length()),
        [this,self](boost::system::error_code ec, std::size_t)
        {
            if (!ec)
            {
                write_msgs_.pop_front();
                if (!write_msgs_.empty())
                {
                    do_write();
                }
            }
            else
            {
                room_.leave(shared_from_this());
            }
        });
    }

    tcp::socket socket_;
    chat_room& room_;
    chat_message read_msg_;
    chat_message_deque write_msgs_;
};

class chat_server
{
public:
    chat_server(boost::asio::io_context &io_context, const tcp::endpoint &endpoint)
        : acceptor_(io_context, endpoint)
    {
        do_accept();
    }

private:
    void do_accept(){
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec)
                {
                    std::cout << "get client\n";
                    std::make_shared<chat_session>(std::move(socket), room_)->start();
                }
                do_accept();
            });
    }
    tcp::acceptor acceptor_;
    chat_room room_;

};

int main(int argc, char* argv[])
{
    try
    {
        if (argc < 2)
        {
            std::cerr << "Usage: server [port] [<prot> ...]\n";
            return 1;
        }

        boost::asio::io_context io_context;

        std::list<chat_server> servers;
        for(int i = 1; i < argc; i++)
        {
            tcp::endpoint endpoint(tcp::v4(), std::atoi(argv[i]));
            servers.emplace_back(io_context, endpoint);
        }
        
        io_context.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    

    return 0;
}