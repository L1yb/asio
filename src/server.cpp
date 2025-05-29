/*a syncchronous TCP daytime server*/
#include <boost/asio.hpp>
#include <iostream>
#include <string>
using boost::asio::ip::tcp;

/*
We define the make_daytime_string to create the string to be sent back to the client.
*/
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
        // tcp::acceptor is created to listen for new connections. It is initialised to listen on TCP port 13, for IPV4.
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 13));
        // This is a iterative server, which means the server will handle one connection at a time. 
        // Create a socket that will represent the connection to the client, and then wait for a connection.
        int num = 0;
        for (;;)
        {
            tcp::socket socket(io);
            acceptor.accept(socket);
            // A client is accessing this server. Determine the current time and transfer the information to the client.
            std::string message = make_daytime_string();

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);
            std::cout << num++ << std::endl; 

        }

    }
    catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
