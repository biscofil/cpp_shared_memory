cmake_minimum_required(VERSION 3.13)

project(shared_memory LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

set(TARGET_NAME shared_memory)

add_executable(server server.cpp)
add_executable(client client.cpp)

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})
