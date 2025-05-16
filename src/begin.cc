#include <boost/asio.hpp>
#include <iostream>
#include <functional>

void print1(const boost::system::error_code&)
{
    std::cout << "5second Hello World!\n";
}
void print2(const boost::system::error_code&, boost::asio::steady_timer* t, int* count)
{
    if(*count < 5)
    {
        std::cout << *count << std::endl;
        (*count)++;
        t->expires_at(t->expiry() + boost::asio::chrono::seconds(1));
        t->async_wait(std::bind(print2, boost::asio::placeholders::error, t, count));
    }
}
int main()
{
    // 创建一个io_service
    boost::asio::io_service io_serv;
    boost::asio::steady_timer t1(io_serv, boost::asio::chrono::seconds(5));
    int count = 0;
    boost::asio::steady_timer t2(io_serv, boost::asio::chrono::seconds(1));
    t1.async_wait(&print1);
    t2.async_wait(std::bind(print2, boost::asio::placeholders::error, &t2, &count));
    io_serv.run();
    std::cout << "final count is " << count << std::endl;
    return 0;
    
}