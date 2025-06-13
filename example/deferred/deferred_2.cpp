#include <iostream>
#include <boost/asio.hpp>

using boost::asio::deferred;

int main()
{
    boost::asio::io_context io_context;

    boost::asio::steady_timer timer(io_context);
    timer.expires_after(std::chrono::seconds(1));

    // 创建了延迟操作的描述，但还没执行
    auto deferred_op = timer.async_wait(deferred(
        [&](boost::system::error_code ec)
        {
            std::cout << "first timer wait finished:" << ec.message() << std::endl;
            timer.expires_after(std::chrono::seconds(1));
            return timer.async_wait(deferred);
        }
    ));



    std::move(deferred_op)(
        [](boost::system::error_code ec)
        {
            std::cout << "timer wait finished:" << ec.message() << std::endl;
        }
    );
    io_context.run();
    return 0;
}