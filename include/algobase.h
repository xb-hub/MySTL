//
// Created by 许斌 on 2022/3/11.
//

/**
 * 底层基本算法
 * copy， fill
 */
#ifndef MYTINYSTL_ALGOBASE_H
#define MYTINYSTL_ALGOBASE_H

#include <string>

namespace mystl
{
template<typename T>
inline const T& max(const T& lv, const T& rv)
{
    return lv > rv ? lv : rv;
}

/******************************** fill ********************************/
// 将[first, last)内元素填充新值
template<typename Iterator, typename T>
void fill(Iterator first, Iterator last, const T& value)
{
    while (first != last)
    {
        *first++ = value;
    }
}

// 将first后n个元素填充新值
template <typename Iterator, typename Size, typename T>
Iterator unchecked_fill_n(Iterator first, Size n, const T& value)
{
    while(n > 0)
    {
        *first++ = value;
        n--;
    }
    return first;
}

template<typename RandomAccessIterator, typename Size, typename T>
RandomAccessIterator fill_n(RandomAccessIterator first, Size n, const T& value, mystl::random_access_iterator_tag)
{
    return unchecked_fill_n(first, n, value);
}

/******************************** copy ********************************/
//    template<typename RandomAccessIterator, typename OutputIterator, typename Distance>
//    inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*)
//    {
//        for(Distance n = last - first; n > 0; n--)
//        {
//            *result++ = *first++;
//        }
//        return  result;
//    }
//
//    template<typename InputIterator, typename OutputIterator>
//    inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
//    {
//        while (first != last)
//        {
//            *result++ = *first++;
//        }
//        return result;
//    }
//
//    template<typename RandomAccessIterator, typename OutputIterator>
//    inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag)
//    {
//        return __copy_d(first, last, result, difference_type(first));
//    }
//
//    template<typename T>
//    inline T* __copy_t(const T* first, const T* last, T* result, std::true_type)
//    {
//        memmove(result, first, sizeof(T) * (last - first));
//        return result + (last - first);
//    }
//
//    template<typename T>
//    inline T* __copy_t(const T* first, const T* last, T* result, std::false_type)
//    {
//        return __copy_d(first, last, result, difference_type(first));
//    }
//
//    /* 仿函数
//     * 普通函数无法进行偏特化，所以使用仿函数定义__copy_dispatch
//     * */
//    template<typename InputIterator, typename OutputIterator>
//    struct __copy_dispatch
//    {
//        OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
//        {
//            return __copy(first, last, result, iterator_category(first));
//        }
//    };
//
//    template<typename T>
//    struct __copy_dispatch<T*, T*>
//    {
//        T* operator()(T* first, T* last, T* result)
//        {
//            typedef typename std::is_trivially_copy_assignable<T> t;
//            return __copy_t(first, last, result, t());
//        }
//    };
//
//    template<typename T>
//    struct __copy_dispatch<const T*, T*>
//    {
//        T* operator()(const T* first, const T* last, T* result)
//        {
//            typedef typename std::is_trivially_copy_assignable<T> t;
//            return __copy_t(first, last, result, t());
//        }
//    };
//
//    template<typename InputIterator, typename OutputIterator>
//    inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
//    {
//
//        return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
//    }
//
//    /* char* wchar_t* copy()特化版本 */
//    inline char* copy(const char* first, const char* last, char* result)
//    {
//        memmove(result, first, last - first);
//        return result + (last - first);
//    }
//
//    inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
//    {
//        memmove(result, first, sizeof(wchar_t) * (last - first));
//        return result + (last - first);
//    }

template<typename RandomAccessIterator, typename OutputIterator>
inline OutputIterator unchecked_copy_cat(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag)
{
    for(int i = last - first; i > 0; i--)
    {
        *result++ = *first++;
    }
    return result;
}

template<typename InputIterator, typename OutputIterator>
inline OutputIterator unchecked_copy_cat(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
{
    while(first != last)
    {
        *result++ = *first++;
    }
    return result;
}

template<typename InputIterator, typename OutputIterator>
inline OutputIterator unchecked_copy(InputIterator first, InputIterator last, OutputIterator result)
{
    return unchecked_copy_cat(first, last, result, iterator_category(first));
}

template<typename InputIterator, typename OutputIterator>
typename std::enable_if<std::is_same<typename std::remove_const<InputIterator>::type, OutputIterator>::value
            && std::is_trivially_copy_assignable<OutputIterator>::value, OutputIterator*>::type
unchecked_copy(InputIterator* first, InputIterator* last, OutputIterator* result)
{
    size_t n = static_cast<size_t>(last - first);
    if(n != 0)  memmove(result, first, n * sizeof(OutputIterator));
    return result + n;
}

template<typename InputIterator, typename OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
{
    return unchecked_copy(first, last, result);
}

/******************************** copy_backward ********************************/

/**
 * last, result都指向下一个位置
 * [最后一个元素]->[last]
 */
template<typename BidirectionalIterator1, typename BidirectionalIterator2>
inline BidirectionalIterator2 unchecked_copy_backward_cat(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result, bidirectional_iterator_tag)
{
    while (last != first)
    {
        *--result = *--last;
    }
    return result;
}

template<typename RandomAccessIterator1, typename RandomAccessIterator2>
inline RandomAccessIterator2 unchecked_copy_backward_cat(RandomAccessIterator1 first, RandomAccessIterator1 last, RandomAccessIterator2 result, random_access_iterator_tag)
{
    for(auto i = last - first; i > 0; i--)
    {
        *--result = *--last;
    }
    return result;
}

template<typename BidirectionalIterator1, typename BidirectionalIterator2>
inline BidirectionalIterator2 unchecked_copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result)
{
    return unchecked_copy_backward_cat(first, last, result, iterator_category(first));
}

template<typename Tp, typename Up>
typename std::enable_if<std::is_same<typename std::remove_const<Tp>::type, Up>::value
            && std::is_trivially_copy_assignable<Up>::value, Up*>::type
unchecked_copy_backward(Tp* first, Tp* last, Up* result)
{
    size_t n = static_cast<size_t>(last - first);
    if(n > 0) memmove(result - n, first, n * sizeof(Tp));
    return result;
}

template<typename BidirectionalIterator1, typename BidirectionalIterator2>
inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result)
{
    return unchecked_copy_backward(first, last, result);
}

}

#endif //MYTINYSTL_ALGOBASE_H
