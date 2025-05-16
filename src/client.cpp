#include <boost/asio.hpp>

using namespace boost::asio;
int main()
{
    io_service service;
    ip::tcp::socket socket(service);
    ip::tcp::endpoint ep(ip::address::from_string("127.0.0.1"), 2001);
    socket.connect(ep);
    return 0;
}