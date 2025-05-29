#include <iostream>
#include <boost/asio.hpp>
#include <array>

using boost::asio::ip::tcp;

int main(int argc, char* argv[])
{
    try
    {
        if(argc != 2)
        {
            std::cerr << "usage: client <host>\n";
            return 1;
        }

        boost::asio::io_context io;
        /*
        resolver获取host和service的name，然后将他们传给endpoint
        resolve函数的参数是host和server name，这里分别是argv[1] ,daytime
        tcp::resolver::results_types is a range,with begin() and end(),can be used for iterating over(迭代遍历) the result
        */ 
        tcp::resolver resolver(io);
        tcp::resolver::results_type endpoints = resolver.resolve(argv[1], "daytime");
        // create and connect the socket
        /*
        The list of endpoint contain both IPV4 and IPv6, we need to try each of them until find one that works.
        But connect() does it automatically.
        */
        tcp::socket socket(io);
        boost::asio::connect(socket, endpoints);
        /* 
        The connect is open. We need to read the response from service.
        We can use array, vector，char[] to hold the received data. The boost::asio::buffer() automatically determines the size of the array
        */
        for(;;)
        {
            std::array<char, 128> buf;
            boost::system::error_code error;
            size_t len = socket.read_some(boost::asio::buffer(buf), error);
            // if connection is closed, ip::tcp::socket::read_some() will exit with the boost::system::error::eof error,which is how to exit the loop.
            if (error == boost::asio::error::eof)
                break;
            else if(error)
                throw boost::system::system_error(error);
            
            std::cout.write(buf.data(), len);
        }
    }
    // handle any exceptios that may have been thrown
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}