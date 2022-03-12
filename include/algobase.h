//
// Created by 许斌 on 2022/3/11.
//

#ifndef MYTINYSTL_ALGOBASE_H
#define MYTINYSTL_ALGOBASE_H

#include <string>

namespace mystl
{
    template<class T>
    const T& max(const T& lv, const T& rv)
    {
        return lv > rv ? lv : rv;
    }

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
    void fill_n(RandomAccessIterator first, Size n, const T& value)
    {
        unchecked_fill_n(first, n, value);
    }

    template<class RandomAccessIterator, class T>
    void fill(RandomAccessIterator first, RandomAccessIterator last, const T& value, mystl::random_access_iterator_tag)
    {
        fill_n(first, last - first, value);
    }

}

#endif //MYTINYSTL_ALGOBASE_H
