#include <array>
#include <iostream>
#include <boost/asio.hpp>
#include <ctime>
#include <string>
using boost::asio::ip::udp;
std::string make_daytime_string()
{
    using namespace std;
    time_t now = time(0);
    return ctime(&now);
}

int main()
{
    try
    {
        boost::asio::io_context io;
        udp::socket socket(io, udp::endpoint(udp::v4(), 13));
        for(;;)
        {
            std::array<char, 1> recv_buf; 
            udp::endpoint remote_endpoint;
            socket.receive_from(boost::asio::buffer(recv_buf), remote_endpoint);
            
            std::string message = make_daytime_string();
            
            boost::system::error_code ignored_error;
            socket.send_to(boost::asio::buffer(message), remote_endpoint, 0, ignored_error);

        }

    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }



    return 0;
}