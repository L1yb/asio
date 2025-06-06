# buffers子目录CMakeLists.txt
cmake_minimum_required(VERSION 3.16)

# 查找所有.cpp文件
set(SOURCES 
    reference_counted.cpp)

# 为每个源文件创建可执行文件
foreach(SOURCE_FILE ${SOURCES})
    get_filename_component(TARGET_NAME ${SOURCE_FILE} NAME_WE)
    add_executable(buffers_${TARGET_NAME} ${SOURCE_FILE})
    
    # 使用顶层定义的公共库
    target_link_libraries(buffers_${TARGET_NAME} PRIVATE ${COMMON_LIBS})
    
    # 添加到安装目标
    install(TARGETS buffers_${TARGET_NAME}
            RUNTIME DESTINATION bin/examples/buffers)
endforeach() 