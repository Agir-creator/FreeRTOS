cmake_minimum_required(VERSION 3.12)
project(rpc_protocol CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Исходные файлы
set(SOURCES
    src/rpc_physical.cpp
    src/rpc_data_link.cpp
    src/rpc_transport.cpp
    src/rpc_application.cpp
    src/main.cpp
)

add_executable(rpc_protocol ${SOURCES})
target_include_directories(rpc_protocol PRIVATE include)
target_compile_options(rpc_protocol PRIVATE -Wall -Wextra)