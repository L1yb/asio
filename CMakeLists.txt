cmake_minimum_required(VERSION 3.16)
project(asio)

# 设置C++标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Boost库
find_package(Boost REQUIRED COMPONENTS system)

# 添加可执行文件
add_executable(begin src/begin.cc)
add_executable(client src/client.cpp)
add_executable(server src/server.cpp)
# 包含头文件目录
target_include_directories(begin PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 查找pthread库
find_package(Threads REQUIRED)

# 链接库
target_link_libraries(begin PRIVATE Boost::system Threads::Threads)
target_link_libraries(client PRIVATE Boost::system Threads::Threads)
target_link_libraries(server PRIVATE Boost::system Threads::Threads)