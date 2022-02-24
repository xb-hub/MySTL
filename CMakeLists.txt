cmake_minimum_required(VERSION 3.20)
project(MyTinySTL)

include_directories(
        include
)

set(CMAKE_CXX_STANDARD 14)

add_executable(MyTinySTL test/main.cpp)
