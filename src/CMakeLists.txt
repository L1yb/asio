# src目录的CMakeLists.txt

# 声明可执行文件源码
set(EXECUTABLES
    begin.cc
    client.cpp
    server.cpp
    mutithread.cpp
    async_server.cpp
    udp_client.cpp
    udp_server.cpp
    udp_async_server.cpp
    com_TU_server.cpp
)

# 为每个源文件创建可执行文件
foreach(SOURCE_FILE ${EXECUTABLES})
    # 提取文件名（不带扩展名）作为目标名
    get_filename_component(TARGET_NAME ${SOURCE_FILE} NAME_WE)
    
    # 添加可执行文件
    add_executable(${TARGET_NAME} ${SOURCE_FILE})
    
    # 添加包含目录（如果需要）
    # target_include_directories(${TARGET_NAME} PRIVATE 
    #     ${CMAKE_SOURCE_DIR}/include
    # )
    
    # 链接公共库
    target_link_libraries(${TARGET_NAME} PRIVATE ${COMMON_LIBS})
    
    # 设置特定的编译标志（如果需要）
    # target_compile_options(${TARGET_NAME} PRIVATE -特定标志)
    
    # 添加到安装目标
    install(TARGETS ${TARGET_NAME} 
            RUNTIME DESTINATION bin
    )
endforeach()

# 对于需要特殊处理的特定文件
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/begin.cc)
    set_source_files_properties(begin.cc PROPERTIES LANGUAGE CXX)
endif()

# 添加测试
if(BUILD_TESTS)
    # 这里可以添加测试目标
    # add_executable(test_something test_something.cpp)
    # target_link_libraries(test_something PRIVATE ${COMMON_LIBS})
endif() 