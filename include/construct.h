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
template<typename T>
void construct(T* p)
{
    ::new(p) T();
}
// placement new
template<typename T1, typename T2>
void construct(T1* p, const T2& value)
{
    ::new(p) T1(value);
}

template<typename T1, typename... Args>
void construct(T1* p, Args&&... args)
{
    ::new(p) T1(std::forward<Args>(args)...);
}


// 有基本的析构函数，例如不含指针的类和基本类型
template<typename Iterator>
void destory_one(Iterator p, std::true_type)
{
    #ifdef __DEBUG
        std::cout << "has_trivially_destructible" << std::endl;
    #endif
}

// 没有基本的析构函数，例如含有指针的类，需要调用析构函数释放内存。
template<typename T>
void destory_one(T* p, std::false_type)
{
    #ifdef __DEBUG
        std::cout << "no_trivially_destructible" << std::endl;
    #endif
    if(!p)
    {
        std::cout << "析构函数" << std::endl;
        p->~T();
    }
}

template<typename Iterator>
using trivial_destructor = std::is_trivially_destructible<typename iterator_traits<Iterator>::value_type>;

// std::is_trivially_destructible<T>::value  判断类型T是否可以被破坏。
template<typename Iterator>
void destory(Iterator p)
{
    destory_one(p, trivial_destructor<Iterator>());
}

template<typename Iterator>
void destory_dispatch(Iterator first, Iterator last, std::true_type) {}

template<typename Iterator>
void destory_dispatch(Iterator first, Iterator last, std::false_type)
{
    for(; first != last; first++)
    {
        destory(first);
    }
}

template<typename Iterator>
void destory(Iterator first, Iterator last)
{
    destory_dispatch(first, last, trivial_destructor<Iterator>());
}

}

#endif //MYTINYSTL_CONSTRUCT_H
