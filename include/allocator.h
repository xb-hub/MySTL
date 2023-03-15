//
// Created by 许斌 on 2022/2/24.
//
/**
 * allocate: 申请内存
 * deallocate: 释放申请的内存
 */
#ifndef MYTINYSTL_ALLOCATOR_H
#define MYTINYSTL_ALLOCATOR_H
// #define __DEBUG

#include <iostream>
#include "construct.h"

namespace mystl
{
    template<typename T>
    class allocator
    {
    public:
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using const_pointer = const T*;
        using const_reference = const T&;
        using difference_type = ptrdiff_t;
    public:
        static T* allocate();
        static T* allocate(size_t n);

        static void deallocate(T* p);
        static void deallocate(T* p, size_t n);

        static void construct(T* p);
        static void construct(T* p, const T& value);
        static void construct(T* p, T&& value);

        template<typename... Args>
        static void construct(T*p, Args&& ...args);

        static void destory(T* p);
        static void destory(T* frist, T* last);
    };

    template<typename T>
    T* allocator<T>::allocate()
    {
        return static_cast<T*>(::operator new(sizeof(T)));
    }

    template<typename T>
    T* allocator<T>::allocate(size_t n)
    {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    template<typename T>
    void allocator<T>::deallocate(T *p)
    {
        if(p == nullptr)    return;
        ::operator delete(p);
    }

    template<typename T>
    void allocator<T>::deallocate(T *p, size_t n)
    {
        if(p == nullptr)    return;
        ::operator delete(p);
    }

    template<typename T>
    void allocator<T>::construct(T *p)
    {
        mystl::construct(p);
    }

    template<typename T>
    void allocator<T>::construct(T *p, const T &value)
    {
        #ifdef __DEBUG
            std::cout << "placement new!" << std::endl;
        #endif
        mystl::construct(p, value);
    }

    template<typename T>
    void allocator<T>::construct(T *p, T &&value)
    {
        #ifdef __DEBUG
            std::cout << "right value!" << std::endl;
        #endif
        mystl::construct(p, std::move(value));
    }

    template<typename T>
    template<typename ...Args>
    void allocator<T>::construct(T *p, Args&& ...args)
    {
        mystl::construct(p, std::forward<Args>(args)...);
    }

    template<typename T>
    void allocator<T>::destory(T *p)
    {
        mystl::destory(p);
    }

    template<typename T>
    void allocator<T>::destory(T *first, T *last)
    {
        mystl::destory(first, last);
    }
}


#endif //MYTINYSTL_ALLOCATOR_H
