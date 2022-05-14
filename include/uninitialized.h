//
// Created by 许斌 on 2022/3/11.
//

/**
 * 用于进行未初始化的复制，填充
 */
#ifndef MYTINYSTL_UNINITIALIZED_H
#define MYTINYSTL_UNINITIALIZED_H
// #define __DEBUG

#include "construct.h"
#include "iterator.h"
#include "type_traits.h"
#include "algobase.h"

namespace mystl
{
    /******************************** uninitialized_copy ********************************/
    template<class InputIterator, class ForwardIterator>
    ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, std::true_type)
    {
        return mystl::copy(first, last, result);
    }

    template<class InputIterator, class ForwardIterator>
    ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, std::false_type)
    {
        ForwardIterator cur = result;
        while (first != last)
        {
            mystl::construct(cur, *first);
            ++cur;  ++first;
        }
        return cur;
    }

    template<class InputIterator, class ForwardIterator, class T>
    ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
    {
        typedef typename type_traits<T>::is_POD_type is_POD;
        return __uninitialized_copy_aux(first, last, result, is_POD());
    }

    template<class InputIterator, class ForwardIterator>
    ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
    {
        return __uninitialized_copy(first, last, result, value_type(result));
    }

    /******************************** uninitialized_fill ********************************/
    /******************************** template<class Iterator, class Size, class T> ********************************/
    template<class Iterator, class Size, class T>
    Iterator __uninitialized_fill_n_aux(Iterator first, Size n, const T& value, std::false_type)
    {
#ifdef __DEBUG
        std::cout << "! POD" << std::endl;
#endif
        Iterator cur = first;
        try
        {
            while (n > 0)
            {
                mystl::construct(cur, value);
                ++cur;  --n;
            }
        }
        catch (...)
        {
            while (first != cur)
            {
                mystl::destory(first);
                ++first;
            }
        }
        return cur;
    }

    template<class Iterator, class Size, class T>
    Iterator __uninitialized_fill_n_aux(Iterator first, Size n, const T& value, std::true_type)
    {
#ifdef __DEBUG
        std::cout << "POD_type" << std::endl;
#endif
        return mystl::fill_n(first, n, value, iterator_category(first));
    }

    template<class Iterator, class Size, class T, class T1>
    Iterator __uninitialized_fill_n(Iterator first, Size n, const T& value, T1*)
    {
        typedef typename type_traits<T1>::is_POD_type is_POD;
        return __uninitialized_fill_n_aux(first, n, value, is_POD());
    }

    template<class Iterator, class Size, class T>
    Iterator uninitialized_fill_n(Iterator first, Size n, const T& value)
    {
        return __uninitialized_fill_n(first, n, value, value_type(first));
    }

    /******************************** template<class Iterator, class T> ********************************/
    template<class Iterator, class T>
    void __uninitialized_fill_aux(Iterator first, Iterator last, const T& value, std::false_type)
    {
        Iterator cur = first;
        try
        {
            while (cur != last)
            {
                mystl::construct(cur, value);
                ++cur;
            }
        }
        catch (...)
        {
            while (first != cur)
            {
                mystl::destory(first);
                ++first;
            }
        }
    }

    template<class Iterator, class T>
    void __uninitialized_fill_aux(Iterator first, Iterator last, const T& value, std::true_type)
    {
        fill(first, last, value);
    }

    template<class Iterator, class T>
    inline void __uninitialized_fill(Iterator first, Iterator last, const T& value, T*)
    {
        typedef typename mystl::type_traits<T>::is_POD_type is_POD;
        __uninitialized_fill_aux(first, last, value, is_POD());
    }


    template<class Iterator, class T>
    inline void uninitialized_fill(Iterator first, Iterator last, const T& value)
    {
        __uninitialized_fill(first, last, value, value_type(first));
    }

}

#endif //MYTINYSTL_UNINITIALIZED_H
