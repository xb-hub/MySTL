//
// Created by 许斌 on 2022/3/10.
//

#ifndef MYTINYSTL_VECTOR_H
#define MYTINYSTL_VECTOR_H
#include <cstddef>

namespace mystl
{
    template<class T, class Alloc = alloc>
    class vector
    {
    public:
        typedef T   value_type;
        typedef ptrdiff_t difference_type;
        typedef T*  pointer;
        typedef T*  iterator;
        typedef T&  reference;


    private:


    };
} // namespace mystl

#endif //MYTINYSTL_VECTOR_H
