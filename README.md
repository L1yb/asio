/usr/local/include/boost/asio
# 异步的使用计时器
https://www.boost.org/doc/libs/latest/doc/html/boost_asio/tutorial/tuttimer2.html

```cpp
void print1(const boost::system::error_code&)
{
    std::cout << "5second Hello World!\n";
}
void print2(const boost::system::error_code&)
{
    std::cout << "3second Hello World!\n";
}
int main()
{
    boost::asio::io_service io_serv;
    boost::asio::steady_timer t1(io_serv, boost::asio::chrono::seconds(5));
    boost::asio::steady_timer t2(io_serv, boost::asio::chrono::seconds(3));
    t1.async_wait(&print1);
    t2.async_wait(&print2);

    io_serv.run();
    return 0;
}
```
结果：
```bash
3second Hello World!
5second Hello World!
```
1. steady_timer::async_wait() 
- 这个函数会做一个异步等待，函数会立即返回而不阻塞当前线程
- 当计时器到期或者被取消，会调用回调函数（完成处理器）
- 对于每次调用，回调函数只被调用一次

**taken：完成令牌，用于生成完成处理器**
- 可以是use_future, use_awaitable, yield_context
- 对于完成处理器的函数签名有要求，必须是：
```cpp
void handler(const boost::system::error_code& error)
```
completion signature：完成签名
```cpp
void (boost::system::error_code)
```

2. run()
- 完成处理器只会调用run的线程调用，不调用run(),将不会调用完成处理器
- run函数会持续运行直到计时器到期并且完成处理器返回。
- run函数运行必须有“work”，没有任务的时候会立即返回


# 将参数绑定到completion handler

```cpp
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
```
这里使用了一种精准的时间控制，t->expiry()获取到期时间作为绝对时间，expires_at()设置到期时间作为绝对时间

## [expires_at]( https://www.boost.org/doc/libs/latest/doc/html/boost_asio/reference/basic_waitable_timer/expires_at.html
)
任何还未执行的异步等待任务将会被取消 Any pending asynchronous wait operations will be cancelled
return value：取消的任务数量
> 如果一个计时器在调用expires_at之时已经超时，对于异步等待操作处理器将会：
> 已经被调用过了
> 已经排队在将来被调用
此时无法取消这些任务，将传递一个错误码，指示需要成功完成这些等待操作

# 使用成员函数作为完成处理器


```cpp
class printer{
public:
    printer(boost::asio::io_context& io) 
    : timer_(io, boost::asio::chrono::seconds(1)), count_(0)
    {
        timer_.async_wait(std::bind(&printer::print, this));
    }
    ~printer()
    {
        std::cout << "final count: " << count_ << std::endl;
    }
    void print()
    {
        if(count_ < 5)
        {
            std::cout << count_ << std::endl;
            count_++;
            timer_.expires_at(timer_.expiry() + boost::asio::chrono::seconds(1));
            timer_.async_wait(std::bind(&printer::print, this));
        }
    }

private: 
    boost::asio::steady_timer timer_;
    int count_;

};
```
构造函数：将会引用一个io_context，并在初始化计时器的时候使用它，count_负责结束递归
    bind：**所有的非静态类成员函数都有隐式的this参数**，这里绑定了this，另外，没有使用placeholders::error的占位符，但是并不代表没有传入这个参数，回调函数会接收到error这个参数，但是根据bind的语法，参数数量大于占位词，多余的会被忽略，print函数没有这个参数所以被忽略了。
- 没有errorcode的print满足完成处理器的函数签名要求吗？
    答案是不满足，但是这里没有直接使用print函数，而是使用bind函数，有意思的是bind创建的新的函数对象竟然满足签名要求。
    bind的operator()在错误码传入的时候被调用，模板会接受这个传入的参数，但是会被忽略。


# 多线程中的同步完成处理器
完成处理器只会被调用boost::asio::io_context::run()的线程中被调用，在单线程的时候，完成处理器不会被同时调用。
单线程会遇到的限制：
- 当处理器需要很长时间完成时，响应较差
- 无法在多处理器系统上拓展
遇到这些限制，另一种方法是使用线程池来调用run()。但是由于处理器并发的执行，去访问共享的、非线程安全的资源的时候，需要一种同步方法。
```cpp
timer1_.async_wait(boost::asio::bind_executor(strand_,
                std::bind(&printer::print1, this)));
```
通过strand可以确保它提交的处理器不会同时执行，同一个strand提交的处理器会串行执行
 

 # 同步的tcp daytime 服务器和客户端
```cpp
// server

        boost::asio::io_context io;
        // tcp::acceptor is created to listen for new connections. It is initialised to listen on TCP port 13, for IPV4.
        tcp::acceptor acceptor(io, tcp::endpoint(tcp::v4(), 13));
        // This is a iterative server, which means the server will handle one connection at a time. 
        // Create a socket that will represent the connection to the client, and then wait for a connection.
        for (;;)
        {
            tcp::socket socket(io);
            acceptor.accept(socket);
            // A client is accessing this server. Determine the current time and transfer the information to the client.
            std::string message = make_daytime_string();

            boost::system::error_code ignored_error;
            boost::asio::write(socket, boost::asio::buffer(message), ignored_error);


        }

```
socket每次循环都创建一个，单一负责某一个客户端连接，并且根据RAII资源会自动管理
连接的过程和socket流程一样的，创建socket之后，acceptor负责处理监听，port和ip是由一个acceptor类完成，accept是其成员函数
传输数据使用write，数据是自定义的系统时间转化成字符串
对比socket编程： 
              socket -- bind（ port ip ）-- listen -- accept -- read -- write -- close -- 
io_context -- socket -- acceptor port ip ）-- accept -- read -- write -- close -- 

```cpp
// client
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
```


# explicit

