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
template<typename InputIterator, typename ForwardIterator>
ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, mystl::m_true_type)
{
    return mystl::copy(first, last, result);
}

template<typename InputIterator, typename ForwardIterator>
ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, mystl::m_fasle_type)
{
    ForwardIterator cur = result;
    while (first != last)
    {
        mystl::construct(cur, *first);
        ++cur;  ++first;
    }
    return cur;
}

template<typename InputIterator, typename ForwardIterator, typename T>
ForwardIterator __uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result, T*)
{
    typedef typename mystl::is_POD_type<T>::type is_POD;
    return __uninitialized_copy_aux(first, last, result, is_POD());
}

template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result)
{
    return __uninitialized_copy(first, last, result, value_type(result));
}

/******************************** uninitialized_fill ********************************/
/******************************** template<typename Iterator, typename Size, typename T> ********************************/
template<typename Iterator, typename Size, typename T>
Iterator __uninitialized_fill_n_aux(Iterator first, Size n, const T& value, mystl::m_fasle_type)
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

template<typename Iterator, typename Size, typename T>
Iterator __uninitialized_fill_n_aux(Iterator first, Size n, const T& value, mystl::m_true_type)
{
#ifdef __DEBUG
    std::cout << "POD_type" << std::endl;
#endif
    return mystl::fill_n(first, n, value, iterator_category(first));
}

template<typename Iterator, typename Size, typename T, typename T1>
Iterator __uninitialized_fill_n(Iterator first, Size n, const T& value, T1*)
{
    typedef typename mystl::is_POD_type<T>::type is_POD;
    return __uninitialized_fill_n_aux(first, n, value, is_POD());
}

template<typename Iterator, typename Size, typename T>
Iterator uninitialized_fill_n(Iterator first, Size n, const T& value)
{
    return __uninitialized_fill_n(first, n, value, value_type(first));
}

/******************************** template<typename Iterator, typename T> ********************************/
template<typename Iterator, typename T>
void __uninitialized_fill_aux(Iterator first, Iterator last, const T& value, mystl::m_fasle_type)
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

template<typename Iterator, typename T>
void __uninitialized_fill_aux(Iterator first, Iterator last, const T& value, mystl::m_true_type)
{
    fill(first, last, value);
}

template<typename Iterator, typename T>
inline void __uninitialized_fill(Iterator first, Iterator last, const T& value, T*)
{
    typedef typename mystl::is_POD_type<T>::type is_POD;
    __uninitialized_fill_aux(first, last, value, is_POD());
}


template<typename Iterator, typename T>
inline void uninitialized_fill(Iterator first, Iterator last, const T& value)
{
    __uninitialized_fill(first, last, value, value_type(first));
}

}

#endif //MYTINYSTL_UNINITIALIZED_H
