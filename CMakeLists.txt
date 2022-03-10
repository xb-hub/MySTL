cmake_minimum_required(VERSION 3.10)
project(MyTinySTL)

include_directories(
        include
)

set(CMAKE_CXX_STANDARD 11)

#add_executable(MyTinySTL test/main.cpp)

add_executable(memorypool test/memorypool_test.cpp)
add_executable(allocator test/allocator_test.cpp)