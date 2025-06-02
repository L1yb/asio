cmake_minimum_required(VERSION 3.16)
project(asio VERSION 1.0.0 LANGUAGES CXX)

# 全局设置
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 定义输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 依赖库
find_package(Boost REQUIRED COMPONENTS system)
find_package(Threads REQUIRED)

# 全局编译选项
add_compile_options(-Wall -Wextra)

# 定义公共链接库
set(COMMON_LIBS 
    Boost::system 
    Threads::Threads
)

# 启用测试
option(BUILD_TESTS "Build test programs" OFF)

# 添加子目录
add_subdirectory(src)
add_subdirectory(example)

# 安装规则
install(TARGETS
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

# 打印配置信息
message(STATUS "Building ${PROJECT_NAME} version ${PROJECT_VERSION}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
