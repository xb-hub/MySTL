//
// Created by 许斌 on 2022/2/24.
//

#ifndef MYTINYSTL_CONSTRUCT_H
#define MYTINYSTL_CONSTRUCT_H

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

    template<class T>
    void destory_one(T* p, std::false_type)
    {
        if(!p)
        {
            p->~T();
        }
    }

    template<class T>
    void destory_one(T* p, std::true_type) {}

    template<class ForwardIterator>
    void destory_batch(ForwardIterator first, ForwardIterator last, std::false_type)
    {
        for(; first != last; first++)
        {
            destory(first);
        }
    }

    template<class ForwardIterator>
    void destory_bath(ForwardIterator first, ForwardIterator last, std::true_type) {}

    // std::is_trivially_destructible<T>::value  判断类型T是否可以被破坏。
    template<class T>
    void destory(T* p)
    {
        destory_one(p, std::is_trivially_destructible<T>::value);
    }

    template<class ForwardIterator>
    void destory(ForwardIterator first, ForwardIterator last)
    {
        destory_batch(first, last, std::is_trivially_destructible<typename iterator_traits<ForwardIterator>::value_type>::value);
    }
}

#endif //MYTINYSTL_CONSTRUCT_H
