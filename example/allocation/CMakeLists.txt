# allocation子目录CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# 查找所有.cpp文件
set(SOURCES 
    server.cpp)

# 为每个源文件创建可执行文件
foreach(SOURCE_FILE ${SOURCES})
    get_filename_component(TARGET_NAME ${SOURCE_FILE} NAME_WE)
    add_executable(allocation_${TARGET_NAME} ${SOURCE_FILE})
    target_link_libraries(allocation_${TARGET_NAME} PRIVATE Boost::system Threads::Threads)
endforeach() 