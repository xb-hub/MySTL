//
// Created by 许斌 on 2022/2/24.
//
/**
 * construct: 创建对象
 * destory: 释放容器内存储的对象
 */
#ifndef MYTINYSTL_CONSTRUCT_H
#define MYTINYSTL_CONSTRUCT_H
// #define __DEBUG

#include <new>
#include "iterator.h"

namespace mystl
{
    template<class T>
    void construct(T* p)
    {
        ::new(p) T();
    }
    // placement new
    template<class T1, class T2>
    void construct(T1* p, const T2& value)
    {
        ::new(p) T1(value);
    }

    template<class T1, class... Args>
    void construct(T1* p, Args&&... args)
    {
        ::new(p) T1(std::forward<Args>(args)...);
    }


    // 有基本的析构函数，例如不含指针的类和基本类型
    template<class Iterator>
    void destory_one(Iterator p, std::true_type)
    {
        #ifdef __DEBUG
            std::cout << "has_trivially_destructible" << std::endl;
        #endif
    }

    // 没有基本的析构函数，例如含有指针的类，需要调用析构函数释放内存。
    template<class Iterator>
    void destory_one(Iterator p, std::false_type)
    {
        #ifdef __DEBUG
            std::cout << "no_trivially_destructible" << std::endl;
        #endif
        if(!p)
        {
            p->~T();
        }
    }

    // std::is_trivially_destructible<T>::value  判断类型T是否可以被破坏。
    template<class Iterator>
    void destory(Iterator p)
    {
        typedef std::is_trivially_destructible<typename iterator_traits<Iterator>::value_type> trivial_destructor;
        destory_one(p, trivial_destructor());
    }

    template<class Iterator>
    void destory_dispatch(Iterator first, Iterator last, std::true_type) {}

    template<class Iterator>
    void destory_dispatch(Iterator first, Iterator last, std::false_type)
    {
        for(; first != last; first++)
        {
            destory(first);
        }
    }

    template<class Iterator>
    void destory(Iterator first, Iterator last)
    {
        typedef std::is_trivially_destructible<typename iterator_traits<Iterator>::value_type> trivial_destructor;
        destory_dispatch(first, last, trivial_destructor());
    }
}

#endif //MYTINYSTL_CONSTRUCT_H
