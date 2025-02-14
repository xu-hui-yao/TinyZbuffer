cmake_minimum_required(VERSION 3.19)
project(ZBuffer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(ZBuffer main.cpp)

target_include_directories(ZBuffer PRIVATE ${CMAKE_SOURCE_DIR}/include)

add_subdirectory(src)
add_subdirectory(ext)
