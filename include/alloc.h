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
        FreeList* free_list_link;   // 指向链表下一节点
        char* address[1];           // 当前节点指针
    };

    const int MaxByte = 128;
    const int ALIGN = 8;
    const int NFreeLists = MaxByte / ALIGN;

    class alloc
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
        alloc(/* args */) {}
        ~alloc() {}

        static void* allocate(size_t n);
        static void deallocate(void* p, size_t n);
        static void* reallocate(void* p, size_t n);
        static void* reallocate(void *p, size_t old_n, size_t new_n);

    };

    // 初始化static成员变量
    char* alloc::start_free = nullptr;
    char* alloc::end_free = nullptr;
    size_t alloc::heap_size = 1;

    FreeList* alloc::free_list[NFreeLists] = {nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr,
            nullptr, nullptr, nullptr, nullptr};

    // 从free_list取出free_node,供调用者使用。
    void* alloc::allocate(size_t n)
    {
        if(n > MaxByte)   // 超出free_list内存块最大值，通过malloc申请内存。
        {
            std::cout << "申请内存过大，内存池无法提供，直接申请内存！" << std::endl;
            return malloc(n);
        }
        FreeList* free_node = free_list[freelist_index(n)];
        FreeList* result = free_node;
        if(result == nullptr)   // 如果从free_list内存申请失败,从内存池填充,并返回。
        {
            std::cout << "free_list为空，从内存池内申请内存填充！" << std::endl;
            return refill(round_up(n));
        }
        std::cout << "free_list有空间，分配内存！" << result << std::endl;
        free_node = free_node->free_list_link;
        std::cout << "free_node:" << free_node << std::endl;
        return result;
    }

    // 回收free_node，加入free_list
    void alloc::deallocate(void *p, size_t n)
    {
        if(n > MaxByte) free(p);
        FreeList* free_node = free_list[freelist_index(n)];
        FreeList* q = (FreeList*)p;
        q->free_list_link = free_node;
        free_node = q;
    }

    void* alloc::reallocate(void *p, size_t old_n, size_t new_n)
    {
        deallocate(p, old_n);
        return allocate(new_n);
    }

    void* alloc::refill(size_t n)
    {
        int nobjs = 20;
        char* fill_node = my_chunk_alloc(n, nobjs);
        if(nobjs == 1)   // 如果只需申请一块内存，将内存块返回给调用者。
        {
            std::cout << "返回第一块给调用者！" << std::endl;
            return fill_node;
        }
        std::cout << "将剩下内存加入free_list！" << std::endl;
        FreeList* result = (FreeList*)fill_node;
        free_list[freelist_index(n)] = (FreeList*)(fill_node + n);
        FreeList* free_node = free_list[freelist_index(n)];
        for(int i = 2; i < nobjs; i++)
        {
            free_node->free_list_link = (FreeList*)(fill_node + i * n);
            free_node = free_node->free_list_link;
        }
        free_node->free_list_link = nullptr;
        std::cout << "free_node:" << free_list[freelist_index(n)] << std::endl;
        return result;
    }

// free_list内存不足，从内存池获取内存。
    char* alloc::my_chunk_alloc(size_t n, int& nobjs)
    {
        size_t total_byte = n * nobjs;
        size_t left_byte = end_free - start_free;
        char* result;
        if(total_byte <= left_byte) // 内存池足够分配nobjs
        {
            std::cout << "内存池足够分配nobjs" << std::endl;
            result = start_free;
            start_free += total_byte;
            return result;
        }
        else if(left_byte > n)   // 内存池不足以分配nobjs，但可以分配部分
        {
            std::cout << "内存池不足以分配nobjs，但可以分配部分" << std::endl;
            nobjs = left_byte / n;
            result = start_free;
            start_free += nobjs * n;
            return result;
        }
        else
        {
            size_t byte_to_get = 2 * total_byte + (heap_size >> 4); // 扩充内存池
            std::cout << "扩充大小：" << byte_to_get << std::endl;
            // 如果内存池内还有剩余空间，将其加入到相应的free_list
            if(left_byte > 0)
            {
                std::cout << "内存池内还有剩余空间，将其加入到相应的free_list" << std::endl;
                FreeList* free_node = free_list[freelist_index(left_byte)];
                ((FreeList*)start_free)->free_list_link = free_node;
                free_node = (FreeList*)start_free;
            }
            // 从堆内申请内存到内存池
            std::cout << "从堆内申请内存到内存池" << std::endl;
            start_free = (char*)malloc(byte_to_get);
            // 从堆申请内存失败
            if(start_free == nullptr)
            {
                // 遍历比size大的节点，若有未用内存块，释放。
                for(int i = freelist_index(n) + 1; i < NFreeLists; i++)
                {
                    FreeList* free_node = free_list[i];
                    if(free_node != nullptr)
                    {
                        free_list[i] = free_node->free_list_link;
                        start_free = (char*)free_node;
                        end_free = (char*)(free_node + (i + 1) * ALIGN);
                        return my_chunk_alloc(n, nobjs);
                    }
                }
                // 申请不到内存也找不到大空闲内存块，抛出异常。
                std::cout << "out of memory" << std::endl;
                end_free = nullptr;
                throw std::bad_alloc();
            }
                // 调整内存池大小，递归调用申请内存。
            else
            {
                heap_size += byte_to_get;
                end_free = start_free + byte_to_get;
                return my_chunk_alloc(n, nobjs);
            }
        }
    }
} // namespace mystl
#endif //MYTINYSTL_ALLOC_H
