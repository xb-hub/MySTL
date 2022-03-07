//
// Created by 许斌 on 2022/2/24.
//

#ifndef MYTINYSTL_CONSTRUCT_H
#define MYTINYSTL_CONSTRUCT_H

#include <new>
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

    }
}

#endif //MYTINYSTL_CONSTRUCT_H
