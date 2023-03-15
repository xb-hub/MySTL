//
// Created by 许斌 on 2022/2/24.
//

#ifndef MYTINYSTL_ITERATOR_H
#define MYTINYSTL_ITERATOR_H

#include <cstddef>

namespace mystl
{
// 5种迭代器类型
struct input_iterator_tag {};   // ++ 只读
struct output_iterator_tag {};  // ++ 只写
struct forward_iterator_tag : public input_iterator_tag {}; // ++ 区间写
struct bidirectional_iterator_tag : public forward_iterator_tag {}; // ++ --
struct random_access_iterator_tag : public bidirectional_iterator_tag {};   // 所有指针算术能力

// 实现每个迭代器时，用于继承
template<typename Category, typename T, typename Distsnce = ptrdiff_t, typename Pointer = T*, typename Reference = T&>
struct iterator
{
    using iterator_category = Category;
    using value_type = T;
    using difference_type = Distsnce;
    using pointer = Pointer;
    using reference = Reference;
};

// iterator_traits实现
template<typename Iterator>
struct iterator_traits
{
    using iterator_category =  typename Iterator::iterator_category;
    using value_type =  typename Iterator::value_type;
    using difference_type =  typename Iterator::difference_type;
    using pointer =  typename Iterator::pointer;
    using reference =  typename Iterator::reference;
};

template<typename I>
struct iterator_traits<I*>
{
    using iterator_category = mystl::random_access_iterator_tag;
    using value_type = I;
    using difference_type = ptrdiff_t;
    using pointer = I*;
    using reference = I&;
};

template<typename I>
struct iterator_traits<const I*>
{
    using iterator_category = mystl::random_access_iterator_tag;
    using value_type = I;
    using difference_type = ptrdiff_t;
    using pointer = const I*;
    using reference = const I&;
};

// 萃取category
template<typename Iterator>
inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&)
{
    return typename iterator_traits<Iterator>::iterator_category();
}

// 萃取value_type
template<typename Iterator>
inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

// 萃取difference_type
template<typename Iterator>
inline typename iterator_traits<Iterator>::difference_type* difference_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

template<typename InputIterator>
inline typename iterator_traits<InputIterator>::difference_type __distance(InputIterator first, InputIterator last, input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last)
    {
        first++;    n++;
    }
    return n;
}

template<typename RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type __distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
{
    return last - first;
}

template<typename Iterator>
inline typename iterator_traits<Iterator>::difference_type distance(Iterator first, Iterator last)
{
    __distance(first, last, category(first));
}

template<typename InputIterator, typename Distance>
inline void __advance(InputIterator& p, Distance n, input_iterator_tag)
{
    while(n--)  ++p;
}

template<typename BidirectionalIterator, typename Distance>
inline void __advance(BidirectionalIterator& p, Distance n, bidirectional_iterator_tag)
{
    if(n > 0)
    {
        while(n--)  ++p;
    }
    else
    {
        while(n++)  --p;
    }
}

template<typename RandomAccessIterator, typename Distance>
inline void __advance(RandomAccessIterator& p, Distance n, random_access_iterator_tag)
{
    p += n;
}

template<typename Iterator, typename Distance>
inline void advance(Iterator& p, Distance n)
{
    __advance(p, n, iterator_category(p));
}

// type_traits


} // namespace mystl


#endif //MYTINYSTL_ITERATOR_H
