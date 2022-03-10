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
template<class Category, class T, class Distsnce = ptrdiff_t, class Pointer = T*, class Reference = T&>
struct iterator
{
    typedef Category  iterator_category;
    typedef T         value_type;
    typedef Distsnce  difference_type;
    typedef Pointer   pointer;
    typedef Reference reference;
};

// iterator_traits实现
template<class Iterator>
struct iterator_traits
{
    typedef typename Iterator::iterator_category iterator_category;
    typedef typename Iterator::value_type        value_type;
    typedef typename Iterator::difference_type   difference_type;
    typedef typename Iterator::pointer           pointer;
    typedef typename Iterator::reference         reference;
};

template<class I>
struct iterator_traits<I*>
{
    typedef std::random_access_iterator_tag iterator_category;
    typedef I                               value_type;
    typedef ptrdiff_t                       difference_type;
    typedef I*                              pointer;
    typedef I&                              reference;
};

template<class I>
struct iterator_traits<const I*>
{
    typedef std::random_access_iterator_tag iterator_category;
    typedef I                               value_type;
    typedef ptrdiff_t                       difference_type;
    typedef const I*                              pointer;
    typedef const I&                              reference;
};

// 萃取category
template<class Iterator>
inline typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&)
{
    return typename iterator_traits<Iterator>::iterator_category();
}

// 萃取value_type
template<class Iterator>
inline typename iterator_traits<Iterator>::value_type* value_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}

// 萃取difference_type
template<class Iterator>
inline typename iterator_traits<Iterator>::difference_type* difference_type(const Iterator&)
{
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

template<class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type __distance(InputIterator first, InputIterator last, input_iterator_tag)
{
    typename iterator_traits<InputIterator>::difference_type n = 0;
    while (first != last)
    {
        first++;    n++;
    }
    return n;
}

template<class RandomAccessIterator>
inline typename iterator_traits<RandomAccessIterator>::difference_type __distance(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag)
{
    return last - first;
}

template<class Iterator>
inline typename iterator_traits<Iterator>::difference_type distance(Iterator first, Iterator last)
{
    __distance(first, last, category(first));
}

template<class InputIterator, class Distance>
inline void __advance(InputIterator& p, Distance n, input_iterator_tag)
{
    while(n--)  ++p;
}

template<class BidirectionalIterator, class Distance>
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

template<class RandomAccessIterator, class Distance>
inline void __advance(RandomAccessIterator& p, Distance n, random_access_iterator_tag)
{
    p += n;
}

template<class Iterator, class Distance>
inline void advance(Iterator& p, Distance n)
{
    __advance(p, n, iterator_category(p));
}

// type_traits


} // namespace mystl


#endif //MYTINYSTL_ITERATOR_H
