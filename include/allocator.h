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
    template<class T>
    class allocator
    {
    public:
        typedef T   value_type;
        typedef T*  pointer;
        typedef T&  reference;
        typedef const T*    const_pointer;
        typedef const T&    const_reference;
        typedef ptrdiff_t   difference_type;
    public:
        static T* allocate();
        static T* allocate(size_t n);

        static void deallocate(T* p);
        static void deallocate(T* p, size_t n);

        static void construct(T* p);
        static void construct(T* p, const T& value);
        static void construct(T* p, T&& value);

        template<class... Args>
        static void construct(T*p, Args&& ...args);

        static void destory(T* p);
        static void destory(T* frist, T* last);
    };

    template<class T>
    T* allocator<T>::allocate()
    {
        return static_cast<T*>(::operator new(sizeof(T)));
    }

    template<class T>
    T* allocator<T>::allocate(size_t n)
    {
        return static_cast<T*>(::operator new(n * sizeof(T)));
    }

    template<class T>
    void allocator<T>::deallocate(T *p)
    {
        if(p == nullptr)    return;
        ::operator delete(p);
    }

    template<class T>
    void allocator<T>::deallocate(T *p, size_t n)
    {
        if(p == nullptr)    return;
        ::operator delete(p);
    }

    template<class T>
    void allocator<T>::construct(T *p)
    {
        mystl::construct(p);
    }

    template<class T>
    void allocator<T>::construct(T *p, const T &value)
    {
        #ifdef __DEBUG
            std::cout << "placement new!" << std::endl;
        #endif
        mystl::construct(p, value);
    }

    template<class T>
    void allocator<T>::construct(T *p, T &&value)
    {
        #ifdef __DEBUG
            std::cout << "right value!" << std::endl;
        #endif
        mystl::construct(p, std::move(value));
    }

    template<class T>
    template<class ...Args>
    void allocator<T>::construct(T *p, Args&& ...args)
    {
        mystl::construct(p, std::forward<Args>(args)...);
    }

    template<class T>
    void allocator<T>::destory(T *p)
    {
        mystl::destory(p);
    }

    template<class T>
    void allocator<T>::destory(T *first, T *last)
    {
        mystl::destory(first, last);
    }
}


#endif //MYTINYSTL_ALLOCATOR_H
