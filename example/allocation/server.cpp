// server.cpp
// copy from https://www.boost.org/doc/libs/latest/doc/html/boost_asio.html
#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <array>
#include <utility> //std::move
#include <type_traits> //std::aligned_storage
#include <cstdlib> // std::atoi
#include <algorithm>
using boost::asio::ip::tcp;

// manage the memory to be used for handler_based custom allocation.
// If the memory is in use when a request is made, 
// the allocator delegates allocation to the global heap. 
class handler_memory
{
public:
    handler_memory()
    : in_use_(false)
    {

    }
    handler_memory(const handler_memory&) = delete;
    handler_memory& operator= (const handler_memory&) = delete;
    void* allocate(std::size_t size)
    {
        // 未使用这块内存并且申请的内存小于这块内存大小
        if (!in_use_ && size < sizeof(storage_))
        {
            in_use_ = true;
            return &storage_;
        }
        else
        {
            // ::全局作用域解析符，使用c++底层函数operator new分配内存
            // 区别于new T()--会分配内存并使用构造函数， ::operator new()只会分配内存 
            return ::operator new(size);
        }
    }
    void deallocate(void* pointer)
    {
        if (pointer == &storage_)
        {
            in_use_ = false;
        }
        else
        {
            ::operator delete(pointer);
        }
    }
private:
    // Storage space used for handle_based custom memory allocation.
    //std::aligned_storage<1024>获取一块内存， ::type获取实际存储的数据类型
    typename std::aligned_storage<1024>::type storage_;
    // whether storage space has been used.
    bool in_use_;

};

template <typename T>
class handler_allocator
{
public:
    using value_type = T;
    explicit handler_allocator(handler_memory& mem)
    : memory_(mem)
    {

    }
    template<typename U>
    handler_allocator(const handler_allocator<U>& other) noexcept
    : memory_(other.memory_)
    {

    }
    bool operator ==(const handler_allocator& other) const noexcept
    {
        return &memory_ == other;
    }
    bool operator !=(const handler_allocator& other) const noexcept
    {
        return &memory_ != other;
    }
    T* allocate(std::size_t n) const
    {
        // handler_memory 的allocate是获取内存的，传入需要获取的内存的大小，返回指向内存的指针
        // static_cast用于类型转换，不能对不相关的指针或引用进行转换
        return static_cast<T*>(memory_.allocate(sizeof(T) * n));
    }
    void deallocate(T* p, std::size_t) const
    {
        return memory_.deallocate(p);
    }


private:
    // 模板类不是类，实例化模板之后才是类，类之间可以访问私有成员进行运算符运算
    template <typename> friend class handler_allocator;
    handler_memory& memory_;
};


//
class session : public std::enable_shared_from_this<session>
{
public: 
    session(tcp::socket socket)
    :socket_(std::move(socket))
    {
    }
    void start()
    {
        do_read();
    }

private:
    void do_read()
    {
        auto self(shared_from_this());
        socket_.async_read_some(boost::asio::buffer(data_),
            boost::asio::bind_allocator(
                handler_allocator<int>(handler_memory_),
                [this, self](boost::system::error_code ec, std::size_t length)
                {
                    if (!ec)
                    {
                        std::reverse(data_.begin(),data_.begin() + length - 1);
                        std::cout.write(data_.data(), length);
                        do_write(length);
                    }
                }));
    }
    void do_write(std::size_t length)
    {
        auto self(shared_from_this());
        boost::asio::async_write(socket_, boost::asio::buffer(data_, length),
            boost::asio::bind_allocator(
                handler_allocator<int>(handler_memory_),
                [this, self](boost::system::error_code ec, std::size_t)
                {
                    if (!ec)
                    {
                        do_read();
                    }
                }));
    }

    tcp::socket socket_;
    std::array<char, 1024> data_;
    handler_memory handler_memory_;
};

class server
{
public:
    server(boost::asio::io_context& io_context, short port) 
    : acceptor_(io_context, tcp::endpoint(tcp::v4(), port))
    {
        std::cout << "port: " << acceptor_.local_endpoint().port() << std::endl;
        do_accept();
    }


private:
    void do_accept()
    {
        acceptor_.async_accept(
            [this](boost::system::error_code error, tcp::socket socket)
            {
                if (!error)
                {
                    std::make_shared<session>(std::move(socket))->start();
                }
                do_accept();
            });
    }

    tcp::acceptor acceptor_;
};


int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cout << "Usage: server <host>\n"; 
        return 1;
    }

    try
    {
        boost::asio::io_context io_context;
        server server(io_context, std::atoi(argv[1]));
        io_context.run();
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return 0;
}