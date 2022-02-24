//
// Created by 许斌 on 2022/2/24.
//

#ifndef MYTINYSTL_ITERATOR_H
#define MYTINYSTL_ITERATOR_H

#include <cstddef>
#include <iterator>

namespace mystl
{
    class iterator
    {
        template<class Iterator>
        struct iterator_traits
        {
            typedef typename Iterator::iterator_category iterator_category;
            typedef typename Iterator::value_type        value_type;
            typedef typename Iterator::pointer           pointer;
            typedef typename Iterator::reference         reference;
            typedef typename Iterator::difference_type   difference_type;
        };

        template<class T>
        struct iterator_traits<T*>
        {
            typedef std::random_access_iterator_tag iterator_category;
            typedef T   value_type;
            typedef T*  point;
            typedef T&  reference;
            typedef ptrdiff_t difference_type;
        };

        template<class T>
        struct iterator_traits<const T*>
        {
            typedef std::random_access_iterator_tag iterator_category;
            typedef T   value_type;
            typedef const T*  point;
            typedef const T&  reference;
            typedef ptrdiff_t difference_type;
        };
    };
} // namespace mystl


#endif //MYTINYSTL_ITERATOR_H
