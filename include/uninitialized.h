//
// Created by 许斌 on 2022/3/11.
//

#ifndef MYTINYSTL_UNINITIALIZED_H
#define MYTINYSTL_UNINITIALIZED_H
#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "algobase.h"

namespace mystl
{
    template<class InputIterator, class ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
    {

    }

    /***** uninitialized_fill *****/
    template<class Iterator, class T>
    void __uninitialized_fill_aux(Iterator first, Iterator last, const T& value, std::false_type)
    {
        Iterator cur = first;
        try
        {
            while (cur != last)
            {
                mystl::construct(cur, value);
                cur++;
            }
        }
        catch (...)
        {
            while (first != cur)
            {
                mystl::destory(first);
                first++;
            }
        }
    }

    template<class Iterator, class T>
    void __uninitialized_fill_aux(Iterator first, Iterator last, const T& value, std::true_type)
    {
        mystl::fill(first, last, value, iterator_category(first));
    }

    template<class Iterator, class T, class T1>
    void __uninitialized_fill(Iterator first, Iterator last, const T& value, T1*)
    {
        typedef typename type_traits<T1>::is_POD_type is_POD;
        __uninitialized_fill_aux(first, last, value, is_POD());
    }

    template<class Iterator, class Size, class T>
    void uninitialized_fill_n(Iterator first, Size n, const T& value)
    {
        __uninitialized_fill(first, first + n, value, value_type(first));
    }

//    template<class Iterator, class T>
//    void uninitialized_fill(Iterator first, Iterator last, const T& value)
//    {
//        __uninitialized_fill(first, last, value, value_type(first));
//    }

}

#endif //MYTINYSTL_UNINITIALIZED_H
