//
// Created by 许斌 on 2022/3/11.
//

#ifndef MYTINYSTL_ALGOBASE_H
#define MYTINYSTL_ALGOBASE_H

#include <string>

namespace mystl
{
    template<class T>
    inline const T& max(const T& lv, const T& rv)
    {
        return lv > rv ? lv : rv;
    }

    /******************************** fill ********************************/
    template<class ForwardTterator, class T>
    void fill(ForwardTterator first, ForwardTterator last, const T& value, mystl::forward_iterator_tag)
    {
        while(first != last)
        {
            *first = value;
            first++;
        }
    }

    template <class Iterator, class Size, class T>
    void unchecked_fill_n(Iterator first, Size n, const T& value)
    {
        while(n > 0)
        {
            *first = value;
            ++first;    n--;
        }
    }
//
//    template<class RandomAccessIterator, class Size, class T>
//    void unchecked_fill_n(RandomAccessIterator first, Size n, const char& value)
//    {
//        memset(first, static_cast<unsigned char>(value), static_cast<size_t >(n));
//    }

    template<class RandomAccessIterator, class Size, class T>
    void fill_n(RandomAccessIterator first, Size n, const T& value, mystl::random_access_iterator_tag)
    {
        unchecked_fill_n(first, n, value);
    }

    /******************************** copy ********************************/
    template<class RandomAccessIterator, class OutputIterator, class Distance>
    inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*)
    {
        for(Distance n = last - first; n > 0; first++, result++, n--)
        {
            *first = *result;
        }
        return  result;
    }

    template<class InputIterator, class OutputIterator>
    inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag)
    {
        while (first != last)
        {
            *result = *first;
            first++;  result++;
        }
        return result;
    }

    template<class RandomAccessIterator, class OutputIterator>
    inline OutputIterator __copy(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, random_access_iterator_tag)
    {
        return __copy_d(first, last, result, difference_type(first));
    }

    template<class T>
    inline T* __copy_t(const T* first, const T* last, T* result, std::true_type)
    {
        memmove(result, first, sizeof(T) * (last - first));
        return result + (last - first);
    }

    template<class T>
    inline T* __copy_t(const T* first, const T* last, T* result, std::false_type)
    {
        return __copy_d(first, last, result, difference_type(first));
    }

    /* 仿函数
     * 普通函数无法进行偏特化，所以使用仿函数定义__copy_dispatch
     * */
    template<class InputIterator, class OutputIterator>
    struct __copy_dispatch
    {
        OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result)
        {
            return __copy(first, last, result, iterator_category(first));
        }
    };

    template<class T>
    struct __copy_dispatch<T*, T*>
    {
        T* operator()(T* first, T* last, T* result)
        {
            typedef typename std::is_trivially_copy_assignable<T> t;
            return __copy_t(first, last, result, t());
        }
    };

    template<class T>
    struct __copy_dispatch<const T*, T*>
    {
        T* operator()(const T* first, const T* last, T* result)
        {
            typedef typename std::is_trivially_copy_assignable<T> t;
            return __copy_t(first, last, result, t());
        }
    };

    template<class InputIterator, class OutputIterator>
    inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result)
    {

        return __copy_dispatch<InputIterator, OutputIterator>()(first, last, result);
    }

    /* char* wchar_t* copy()特化版本 */
    inline char* copy(const char* first, const char* last, char* result)
    {
        memmove(result, first, last - first);
        return result + (last - first);
    }

    inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result)
    {
        memmove(result, first, sizeof(wchar_t) * (last - first));
        return result + (last - first);
    }
}

#endif //MYTINYSTL_ALGOBASE_H
