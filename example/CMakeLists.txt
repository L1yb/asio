# example目录主CMakeLists.txt

# 设置输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/examples)

# 全局编译选项
add_compile_definitions(EXAMPLE_MODE)

# 添加所有子目录
add_subdirectory(chat)
add_subdirectory(buffers)
add_subdirectory(allocation)
add_subdirectory(deferred)

# 安装配置
install(DIRECTORY ${CMAKE_BINARY_DIR}/bin/examples/
        DESTINATION bin/examples
        USE_SOURCE_PERMISSIONS
)
