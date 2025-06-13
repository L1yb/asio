#include <iostream>
#include <boost/asio.hpp>

using boost::asio::deferred;

int main()
{
    boost::asio::io_context io_context;

    boost::asio::steady_timer timer(io_context);
    timer.expires_after(std::chrono::seconds(1));

    auto deferred_op = timer.async_wait(deferred);
    std::move(deferred_op)(
        [](boost::system::error_code ec)
        {
            std::cout << "timer wait finished:" << ec.message() << std::endl;
        }
    );
    io_context.run();
    return 0;
}