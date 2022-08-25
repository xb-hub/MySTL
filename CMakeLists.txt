cmake_minimum_required(VERSION 3.10)
project(MyTinySTL)

include_directories(
        include
)

set(CMAKE_CXX_STANDARD 11)

add_executable(MyTinySTL test/main.cpp)

add_executable(memorypool test/memorypool_test.cpp)
add_executable(allocator test/allocator_test.cpp)
add_executable(vector test/vector_test.cpp)
add_executable(list test/list_test.cpp)
add_executable(deque test/deque_test.cpp)
add_executable(stack test/stack_test.cpp)
add_executable(heap test/heap_test.cpp)
add_executable(priority_queue test/priority_queue_test.cpp)
add_executable(rb_tree test/rb_tree_test.cpp)
add_executable(set_test test/set_test.cpp)
add_executable(map_test test/map_test.cpp)
add_executable(hashtable_test test/hashtable_test.cpp)