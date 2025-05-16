异步的使用计时器
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

taken：完成令牌，用于生成完成处理器
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
expires_at
 Any pending asynchronous wait operations will be cancelled
 https://www.boost.org/doc/libs/latest/doc/html/boost_asio/reference/basic_waitable_timer/expires_at.html