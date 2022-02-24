//
// Created by 许斌 on 2022/2/24.
//

#ifndef MYTINYSTL_ALLOC_H
#define MYTINYSTL_ALLOC_H

#include <new>
#include <cstddef>
#include <iostream>
#include <cstdlib>

namespace mystl
{
    union FreeList
    {
        union FreeList* free_list_link;
        char* address[1];
    };

    const int MaxByte = 128;
    const int ALIGN = 8;
    const int NFreeLists = MaxByte / ALIGN;

    class Alloc
    {
    private:
        static char* start_free;    // 所有对象共用内存池
        static char* end_free;
        static size_t heap_size;
        static FreeList* free_list[NFreeLists];

    private:
        static size_t freelist_index(size_t n)
        {
            return (n + ALIGN - 1) / (ALIGN - 1);
        }

        static size_t round_up(size_t n)
        {
            return (n + ALIGN - 1) & ~(ALIGN - 1);
        }

        static char* my_chunk_alloc(size_t n, int& nobjs);
        static void* refill(size_t n);

    public:
        Alloc(/* args */) {}
        ~Alloc() {}

        static void* allocate(size_t n);
        static void deallocate(void* p, size_t n);
        static void reallocate(void* p, size_t n);




    };
} // namespace mystl
#endif //MYTINYSTL_ALLOC_H
