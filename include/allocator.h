//
// Created by 许斌 on 2022/2/24.
//

#ifndef MYTINYSTL_ALLOCATOR_H
#define MYTINYSTL_ALLOCATOR_H
#include <iostream>

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
        T* allocate();
        T* allocate(size_t n);

        void deallocate(T* p);
        void deallocate(T* p, size_t n);
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
        ::operator delete(p, n);
    }
}


#endif //MYTINYSTL_ALLOCATOR_H