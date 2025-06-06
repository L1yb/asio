# chat子目录CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# 查找所有.cpp文件
set(SOURCES 
    # sample.cpp
    chat_client.cpp
    chat_server.cpp)

# 为每个源文件创建可执行文件
foreach(SOURCE_FILE ${SOURCES})
    get_filename_component(TARGET_NAME ${SOURCE_FILE} NAME_WE)
    add_executable(${TARGET_NAME} ${SOURCE_FILE})
    
    # 使用顶层定义的公共库
    target_link_libraries(${TARGET_NAME} PRIVATE ${COMMON_LIBS})
    
    # 添加特定的包含目录（如果需要）
    # target_include_directories(chat_${TARGET_NAME} PRIVATE ...)
    
    # 添加到安装目标
    install(TARGETS ${TARGET_NAME}
            RUNTIME DESTINATION bin/examples/chat)
endforeach() 