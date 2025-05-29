#include <boost/asio.hpp>
#include <thread>
#include <functional>
#include <iostream>

class printer
{
    public:
    printer(boost::asio::io_context& io)
        : strand_(boost::asio::make_strand(io)),
          timer1_(io, boost::asio::chrono::seconds(1)),
          timer2_(io, boost::asio::chrono::seconds(1)),
          count_(0)
    {
        timer1_.async_wait(boost::asio::bind_executor(strand_,
                        std::bind(&printer::print1, this)));
        timer2_.async_wait(boost::asio::bind_executor(strand_,
                        std::bind(&printer::print2, this)));
        
    }
    ~printer()
    {   
        std::cout << "final count_: " << count_ << std::endl;
    }

    void print1()
    {
        if(count_ < 10)
        {
            std::cout << "timer 1:" << count_ << std::endl;
            count_++;
        timer1_.expires_at(timer1_.expiry() + boost::asio::chrono::seconds(1));
        timer1_.async_wait(boost::asio::bind_executor(strand_,
            std::bind(&printer::print1, this)));
            std::cout << std::this_thread::get_id() << std::endl;
        }

    }
    
    void print2()
    {
        if(count_ < 10)
        {
            std::cout << "timer 2:" << count_ << std::endl;
            count_++;
        timer2_.expires_at(timer2_.expiry() + boost::asio::chrono::seconds(1));
        timer2_.async_wait(boost::asio::bind_executor(strand_,
            std::bind(&printer::print2, this)));
            std::cout << std::this_thread::get_id() << std::endl;
        }

    }



private:
    boost::asio::strand<boost::asio::io_context::executor_type> strand_;
    boost::asio::steady_timer timer1_;
    boost::asio::steady_timer timer2_;
    int count_;
};


int main()
{
    boost::asio::io_context io;
    printer p(io);
    std::thread t([&]{io.run();});
    
    io.run();
    t.join();


    return 0;
}