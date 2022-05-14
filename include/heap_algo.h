//
// Created by 许斌 on 2022/4/6.
//

#ifndef MYTINYSTL_HEAP_ALGO_H
#define MYTINYSTL_HEAP_ALGO_H

#include "iterator.h"

namespace mystl
{
/******************************** push_heap ********************************/
/**
 * @param first 堆顶地址
 * @param holeIndex 空位相对于首地址的位置
 * @param topIndex  堆顶
 * @param value 插入值
 */
template<typename RandomAccessIterator, typename Distance, typename T>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value)
{
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && *(first + parent) < value)
    {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

template<typename RandomAccessIterator, typename Distance, typename T>
void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)
{
    __push_heap(first, last - first - 1, static_cast<Distance>(0), static_cast<T>(*(last - 1)));
}

template<typename RandomAccessIterator>
void push_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __push_heap_aux(first, last, difference_type(first), value_type(first));
}

// comp重载
template<typename RandomAccessIterator, typename Distance, typename T, typename Compare>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value, Compare comp)
{
    Distance parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value))
    {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

template<typename RandomAccessIterator, typename Distance, typename T, typename Compare>
void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp)
{
    __push_heap(first, last - first - 1, static_cast<Distance>(0), *(last - 1), comp);
}

template<typename RandomAccessIterator, typename Compare>
void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    __push_heap_aux(first, last, difference_type(first), value_type(first), comp);
}


/******************************** pop_heap ********************************/

template<typename RandomAccessIterator, typename Distance, typename T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value)
{
    Distance topIndex = holeIndex;
    Distance rchild = (holeIndex + 1) * 2;
    while (rchild < len)
    {
        // 比较左右子节点大小,value与较大节点比较
        if(*(first + rchild) < *(first + rchild - 1))
        {
            rchild--;
        }
        *(first + holeIndex) = *(first + rchild);
        holeIndex = rchild;
        rchild = (holeIndex + 1) * 2;
    }
    if(rchild == len)   // holeIndex没有右节点
    {
        *(first + holeIndex) = *(first + rchild - 1);
        holeIndex = rchild - 1;
    }
    *(first + holeIndex) = value;
    __push_heap(first, holeIndex, topIndex, value);
}

template<typename RandomAccessIterator, typename Distance, typename T>
void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*)
{
    // 最后一位 last - 1 放置堆顶元素
    *result = *first;
    __adjust_heap(first, static_cast<Distance>(0), static_cast<Distance>(last - first), value);
}

template<typename RandomAccessIterator, typename T>
void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*)
{
    __pop_heap(first, last - 1, last - 1, *(last - 1), difference_type(first));
}

template<typename RandomAccessIterator>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __pop_heap_aux(first, last, value_type(first));
}

// comp重载
template<typename RandomAccessIterator, typename Distance, typename T, typename Compare>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value, Compare comp)
{
    Distance topIndex = holeIndex;
    Distance rchild = (holeIndex + 1) * 2;
    while (rchild < len)
    {
        // 比较左右子节点大小,value与较大节点比较
        if(comp(*(first + rchild), *(first + rchild - 1)))
        {
            rchild--;
        }
        *(first + holeIndex) = *(first + rchild);
        holeIndex = rchild;
        rchild = (holeIndex + 1) * 2;
    }
    if(rchild == len)   // holeIndex没有右节点
    {
        *(first + holeIndex) = *(first + rchild - 1);
        holeIndex = rchild - 1;
    }
    *(first + holeIndex) = value;
    __push_heap(first, holeIndex, topIndex, value, comp);
}

template<typename RandomAccessIterator, typename Distance, typename T, typename Compare>
void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*, Compare comp)
{
    // 最后一位 last - 1 放置堆顶元素
    *result = *first;
    __adjust_heap(first, static_cast<Distance>(0), static_cast<Distance>(last - first), value, comp);
}

template<typename RandomAccessIterator, typename T, typename Compare>
void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*, Compare comp)
{
    __pop_heap(first, last - 1, last - 1, *(last - 1), difference_type(first), comp);
}

template<typename RandomAccessIterator, typename Compare>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    __pop_heap_aux(first, last, value_type(first), comp);
}

/******************************** sort_heap ********************************/

template<typename RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    // 每pop_heap后 last-1
    while((last - first) > 1)
    {
        mystl::pop_heap(first, last--);
    }
}

template<typename RandomAccessIterator, typename Compare>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    while((last - first) > 1)
    {
        mystl::pop_heap(first, last--, comp);
    }
}

/******************************** make_heap ********************************/
template<typename RandomAccessIterator, typename Distance, typename T>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)
{
    Distance len = last - first;
    Distance holeIndex = len / 2 - 1;

    while (true)
    {
        __adjust_heap(first, holeIndex, len, static_cast<T>(*(first + holeIndex)));
        if(holeIndex == 0)  return;
        holeIndex--;
    }
}


template<typename RandomAccessIterator>
void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
    __make_heap(first, last, difference_type(first), value_type(first));
}

template<typename RandomAccessIterator, typename Distance, typename T, typename Compare>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp)
{
    Distance len = last - first;
    Distance holeIndex = len / 2 - 1;

    while (true)
    {
        __adjust_heap(first, holeIndex, len, static_cast<T>(*(first + holeIndex)), comp);
        if(holeIndex == 0)  return;
        holeIndex--;
    }
}


template<typename RandomAccessIterator, typename Compare>
void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
    __make_heap(first, last, difference_type(first), value_type(first), comp);
}

}

#endif //MYTINYSTL_HEAP_ALGO_H
