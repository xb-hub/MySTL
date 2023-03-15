//
// Created by 许斌 on 2022/3/31.
//

#ifndef MYTINYSTL_DEQUE_H
#define MYTINYSTL_DEQUE_H
#define DEQUE_MAP_INIT_SIZE 8

#include "alloc.h"
#include "iterator.h"
#include "allocator.h"
#include "algobase.h"
#include "uninitialized.h"

namespace mystl
{
// constexpr: 表示编译期间算出value;
template<typename T>
struct deque_buf_size
{
//        static constexpr size_t value = sizeof(T) < 256 ? 4096 / sizeof(T) : 16;
    static constexpr size_t value = 8;
};

template<typename T, typename Ref, typename Ptr>
struct deque_iterator : iterator<random_access_iterator_tag, T>
{
    /**
     * ?? 为什么没有继承类型定义
     */
    using value_type = T;
    using reference = Ref;
    using pointer = Ptr;
    using size_type = size_t;
    using difference_type = ptrdiff_t;
    using value_pointer = T*;
    using map_pointer = T**;
    using iterator = deque_iterator<T, T&, T*>;
    using const_iterator = deque_iterator<T, const T&, const T*>;
    using self = deque_iterator;

private:
    const static size_type buf_size = deque_buf_size<T>::value;

/** ------------------------------
 *  node-｜1｜2｜3｜4｜5｜-｜-｜-｜
 *       ｜            ｜      ｜
 *      first         cur    last
 *  ------------------------------
 */
public:
    value_pointer first;
    value_pointer last;
    value_pointer cur;      // 当前第一个没有元素的位置

    map_pointer node;

public:
    deque_iterator() :
            first(nullptr),
            last(nullptr),
            cur(nullptr),
            node(nullptr)
    {
//            std::cout << "Default Constructor!" << std::endl;
    }

    deque_iterator(value_pointer v_ptr, map_pointer m_ptr) :
            first(*m_ptr),
            last(*m_ptr + buf_size),
            cur(v_ptr),
            node(m_ptr)
    {}

    deque_iterator(const deque_iterator& rhs) :
            first(rhs.first),
            last(rhs.last),
            cur(rhs.cur),
            node(rhs.node)
    {
//            std::cout << "Copy Constructor!" << std::endl;
    }

    deque_iterator(deque_iterator&& rhs) :
            first(rhs.first),
            last(rhs.last),
            cur(rhs.cur),
            node(rhs.node)
    {
//            std::cout << "Move Constructor!" << std::endl;
        rhs.first = nullptr;
        rhs.last = nullptr;
        rhs.cur = nullptr;
        rhs.node = nullptr;
    }

    // 引用返回不能返回临时变量，因为函数结束后临时变量被销毁
    self& operator=(const deque_iterator& rhs)
    {
        if(*this == rhs) return *this;
        first = rhs.first;
        last = rhs.last;
        cur = rhs.cur;
        node = rhs.node;
        return *this;
    }

    self& operator=(deque_iterator&& rhs)
    {
        if(*this == rhs) return *this;
        first = rhs.first;
        last = rhs.last;
        cur = rhs.cur;
        node = rhs.node;
        rhs.first = nullptr;
        rhs.last = nullptr;
        rhs.cur = nullptr;
        rhs.node = nullptr;
        return *this;
    }
public:
    // 移动map_pointer
    void set_node(map_pointer new_node)
    {
        node = new_node;
        first = *node;
        last = *node + buf_size;
    }
    /******************************** 操作符重载 ********************************/
    reference operator*() { return *cur; }
    pointer operator->() { return &(operator*()); }
    reference operator[](difference_type n) { return *(*this += n); }

    self& operator++()
    {
        ++cur;
        if(cur == last)
        {
            set_node(++node);
            cur = first;
        }
        return *this;
    }

    self operator++(int)
    {
        self ret = *this;
        ++*this;
        return ret;
    }

    self& operator--()
    {
        if(cur == first)
        {
            set_node(node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }

    self operator--(int)
    {
        self ret = *this;
        --*this;
        return ret;
    }

    self& operator+=(difference_type n)
    {
        difference_type offest = n + (cur - first);
        // 在同一缓冲区
        if(offest >= 0 && offest < static_cast<difference_type>(buf_size))
        {
            cur += n;
        }
        // 不在同一缓冲区
        else
        {
            difference_type node_offest = offest > 0 ? offest / static_cast<difference_type>(buf_size) :
                    -static_cast<difference_type>(-offest - 1) / static_cast<difference_type>(buf_size) - 1;
            set_node(node + node_offest);
            cur = first + (offest - node_offest * static_cast<difference_type>(buf_size));
        }
        return *this;
    }

    // +不修改this，+=修改this,因为返回临时变量，所以不能返回引用
    self operator+(difference_type n) const
    {
        self tmp = *this;
        return tmp += n;
    }

    self& operator-=(difference_type n)
    {
        return *this += -n;
    }

    // +不修改this，+=修改this,因为返回临时变量，所以不能返回引用
    self operator-(difference_type n) const
    {
        self tmp = *this;
        return tmp -= n;
    }

    difference_type operator-(const self& rhs) const
    {
        return static_cast<difference_type>(buf_size) * (node - rhs.node)
                + (cur - first) - (rhs.cur - rhs.first);
    }

    bool operator==(const self& rhs) const { return cur == rhs.cur; }
    bool operator!=(const self& rhs) const { return !(*this == rhs); }
    // 先判断是否在同一个缓冲区
    bool operator<(const self& rhs) const { return node == rhs.node ? cur < rhs.cur : node < rhs.node; }
    // !(*this < rhs)包含==
    bool operator>(const self& rhs) const { return rhs < *this; }
    bool operator<=(const self& rhs) const { return !(*this > rhs); }
    bool  operator>=(const self& rhs) const { return !(*this < rhs); }
};

template<typename T, typename Alloc = alloc>
class deque
{
public:
    using data_allocator =  simple_alloc<T, Alloc>;                      // 缓冲区内存分配器
    using map_allocator =  simple_alloc<T*, Alloc>;                     // deque_map内存分配器
    using value_type =  T;
    using pointer =  T*;
    using reference =  T&;
    using size_type =  size_t;
    using map_pointer =  T**;
    using difference_type =  ptrdiff_t;
    using iterator =  deque_iterator<T, T&, T*>;
    using const_iterator =  deque_iterator<T, const T&, const T*>;

private:
    const static size_type buf_size = deque_buf_size<T>::value;

/**
 * ----------------------
 *          start   finish
 *            ｜      ｜
 * map_ptr-｜｜｜p1｜p2｜p3｜｜｜
 *               ｜
 *            iterator(cur, first, last)
 * ----------------------
 *
 */
private:
    iterator start;     // 头部
    iterator finish;    // 尾部
    map_pointer map_ptr;
    size_type map_size;

public:
    deque() { fill_init(0, value_type()); }
    deque(int n, const value_type& value) { fill_init(n, value); }

public:
    iterator begin() { return start; }
    iterator end() { return finish; }
    size_type size() { return finish - start; }
    bool empty() {return begin() == end(); }
    value_type front() { return *begin(); }
    value_type back() { return *(end() - 1); }

private:
    void creat_map_and_nodes(size_type n);
    void reserve_map_at_back(size_type nodes_to_add = 1);
    void reserve_map_at_front(size_type nodes_to_add = 1);
    void reallocate_map(size_type nodes_to_add, bool add_at_front);

    void fill_init(size_type n, const T& value);

/******************************** 元素操作 ********************************/
private:
    void push_back_aux(const value_type& value);
    void push_front_aux(const value_type& value);
    void pop_back_aux();
    void pop_front_aux();
    iterator insert_aux(iterator position, const value_type& value);

public:
    void push_back(const value_type& value);
    void push_front(const value_type& value);
    void pop_back();
    void pop_front();

    void clear();
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);
    iterator insert(iterator position, const value_type& value);
};

template<typename T, typename Alloc>
void deque<T, Alloc>::creat_map_and_nodes(size_type n)
{
    // 申请map内存
    size_type node_size = n / buf_size + 1;
    map_size = mystl::max(node_size, static_cast<size_type>(DEQUE_MAP_INIT_SIZE));
    try
    {
        map_ptr = map_allocator::allocate(map_size);
    }
    catch (...)
    {
        map_ptr = nullptr;
        map_size = 0;
        throw "map init failure!";
    }

    // 让 nstart 和 nfinish 都指向 map_ 最中央的区域，方便向头尾扩充
    map_pointer nstart = map_ptr + (map_size - node_size) / 2;
    map_pointer nfinish = nstart + node_size - 1;

    // 申请每个map指针指向的buffer内存，建立map-buffer
    map_pointer cur;
    try
    {
        for(cur = nstart; cur <= nfinish; ++cur)
        {
            *cur = data_allocator::allocate(buf_size);
        }
    }
    catch (...)
    {
        while (cur != nstart)
        {
            --cur;
            data_allocator::deallocate(*cur, buf_size);
            *cur = nullptr;
        }
        throw "buffer init failure!";
    }
    // 设置迭代器位置
    start.set_node(nstart);
    start.cur = start.first;
    finish.set_node(nfinish);
    finish.cur = finish.first + n % buf_size;
}

template<typename T, typename Alloc>
void deque<T, Alloc>::reserve_map_at_front(size_type nodes_to_add)
{
    if(nodes_to_add > start.node - map_ptr)
    {
        reallocate_map(nodes_to_add, true);
    }
}

template<typename T, typename Alloc>
void deque<T, Alloc>::reserve_map_at_back(size_type nodes_to_add)
{
    if(nodes_to_add > map_size - (finish.node - map_ptr + 1))
    {
        reallocate_map(nodes_to_add, false);
    }
}

template<typename T, typename Alloc>
void deque<T, Alloc>::reallocate_map(size_type nodes_to_add, bool add_at_front)
{
    size_type old_nodes_num = finish.node - start.node + 1;
    size_type new_nodes_num = old_nodes_num + nodes_to_add;

    map_pointer new_start;
    if(map_size > 2 * new_nodes_num)
    {
        new_start = map_ptr + (map_size - new_nodes_num) / 2 + (add_at_front ? nodes_to_add : 0);
        if(new_start < start.node)
        {
            mystl::copy(start.node, finish.node + 1, new_start);
        }
        else
        {
            mystl::copy_backward(start.node, finish.node + 1, new_start + old_nodes_num);
        }
    }
    else
    {
        size_type new_map_size = map_size + mystl::max(map_size, nodes_to_add) + 2;
        map_pointer new_map_ptr = map_allocator::allocate(new_map_size);
        new_start = new_map_ptr + (new_map_size - new_nodes_num) / 2 + (add_at_front ? nodes_to_add : 0);
        copy(start.node, finish.node + 1, new_start);
        map_allocator::deallocate(map_ptr, map_size);
        map_ptr = new_map_ptr;
        map_size = new_map_size;
    }
    start.set_node(new_start);
    finish.set_node(new_start + old_nodes_num - 1);
}

template<typename T, typename Alloc>
void deque<T, Alloc>::fill_init(size_type n, const value_type& value)
{
    creat_map_and_nodes(n);
    map_pointer cur;
    for(cur = start.node; cur < finish.node; cur++)
    {
        mystl::uninitialized_fill(*cur, *cur + buf_size, value);
    }
    // 由于最后一个缓冲区可能不满，单独初始化
    mystl::uninitialized_fill(finish.first, finish.cur, value);
}

template<typename T, typename Alloc>
void deque<T, Alloc>::push_back(const value_type &value)
{
    // 缓冲区剩2个或更多元素位置
    if(finish.cur != finish.last - 1)
    {
        mystl::construct(finish.cur, value);    // 原地构造
        ++finish.cur;
    }
    // 缓冲区只剩下一个元素位置，添加并移动node指针
    else
    {
        push_back_aux(value);
    }
}

template<typename T, typename Alloc>
void deque<T, Alloc>::push_back_aux(const value_type &value)
{
    reserve_map_at_back();
    // 下一个map节点创建缓冲区
    *(finish.node + 1) = data_allocator::allocate(buf_size);
    try
    {
        mystl::construct(finish.cur, value);
        finish.set_node(finish.node + 1);
        finish.cur = finish.first;
    }
    catch(...)
    {
        mystl::destory(finish.cur);
        finish.set_node(finish.node - 1);
        finish.cur = finish.last - 1;
        data_allocator::deallocate(*(finish.node + 1), buf_size);
        throw "push_back failure!";
    }
}

template<typename T, typename Alloc>
void deque<T, Alloc>::push_front(const value_type &value)
{
    // 缓冲区剩元素位置
    if(start.cur != start.first)
    {
//            mystl::construct(start.cur - 1, value);
//            --start.cur;
        mystl::construct(--start.cur, value);
    }
    else
    {
        push_front_aux(value);
    }
}

template<typename T, typename Alloc>
void deque<T, Alloc>::push_front_aux(const value_type &value)
{
    reserve_map_at_front();
    *(start.node - 1) = data_allocator::allocate(buf_size);
    try
    {
        start.set_node(start.node - 1);
        start.cur = start.last - 1;
        mystl::construct(start.cur, value);
    }
    catch (...)
    {
        mystl::destory(start.cur);
        start.set_node(start.node + 1);
        start.cur = start.first;
        data_allocator::deallocate(*(start.node - 1), buf_size);
        throw "push_front failure!";
    }
}

template<typename T, typename Alloc>
void deque<T, Alloc>::pop_front()
{
    // 有两个或更多元素
    if(start.cur != start.last - 1)
    {
        mystl::destory(start.cur);
        ++start.cur;
    }
    else
    {
        pop_front_aux();
    }
}

template<typename T, typename Alloc>
void deque<T, Alloc>::pop_front_aux()
{
    mystl::destory(start.cur);
    start.set_node(start.node + 1);
    start.cur = start.first;
    data_allocator::deallocate(*(start.node - 1), buf_size);
}

template<typename T, typename Alloc>
void deque<T, Alloc>::pop_back()
{
    if(finish.cur != finish.first)
    {
        --finish.cur;
        mystl::destory(finish.cur);
    }
    else
    {
        pop_back_aux();
    }
}

template<typename T, typename Alloc>
void deque<T, Alloc>::pop_back_aux()
{
    finish.set_node(finish.node - 1);
    finish.cur = finish.last - 1;
    data_allocator::deallocate(*(finish.node + 1), buf_size);
    mystl::destory(finish.cur);
}

// 最终保留一个缓冲区
template<typename T, typename Alloc>
void deque<T, Alloc>::clear()
{
    // 头尾缓冲区可能不满，单独处理
    for(map_pointer node = start.node + 1; node != finish.node; ++node)
    {
        mystl::destory(*node, *node + buf_size);       // 析构元素
        data_allocator::deallocate(*node, buf_size);        // 销毁缓冲区
    }
    // 原map不少与两个缓冲区
    if(start.node != finish.node)
    {
        mystl::destory(start.cur, start.last);     // 析构队头元素
        mystl::destory(finish.first, finish.cur);  // 析构队尾元素
        data_allocator::deallocate(*finish.node, buf_size);
    }
    // 原map只有一个缓冲区
    else
    {
        mystl::destory(start.cur, finish.cur);
    }
    finish = start;
}

// *iterator = *iterator.cur
template<typename T, typename Alloc>
typename deque<T, Alloc>::iterator deque<T, Alloc>::erase(iterator position)
{
    iterator next = position + 1;
    difference_type elems_before = position - start;
    // 哪边移动元素少移动哪边
    // 左边少，左移
    if(elems_before < (size() >> 1))
    {
        mystl::copy_backward(start, position, next);
        pop_front();
    }
    // 右边少，右移
    else
    {
        mystl::copy(next, finish, position);
        pop_back();
    }
    return start + elems_before;
}

template<typename T, typename Alloc>
typename deque<T, Alloc>::iterator deque<T, Alloc>::erase(iterator first, iterator last)
{
    if(first == start && last == finish)
    {
        clear();
        return finish;
    }
    else
    {
        difference_type distance = last - first;
        difference_type elems_before = first - start;
        // 移动左边
        /**
         * 正向copy错误
         * ｜｜｜｜｜｜｜｜｜｜｜｜
         *   ｜｜｜｜｜｜｜｜｜｜｜
         */
        if(elems_before < ((size()- distance) >> 1))
        {
            mystl::copy_backward(start, first, last);
            // 更新start，并销毁删除元素和内存
            iterator new_start = start + distance;
            destory(start, new_start);
            for(iterator node = start.node; node < new_start.node; ++node)
            {
                data_allocator::deallocate(*node. buf_size);
            }
            start = new_start;
        }
        // 移动右边
        else
        {
            mystl::copy(last, finish, first);
            // 更新finish，并销毁删除元素和内存
            iterator new_finish = finish - distance;
            destory(new_finish, finish);
            for(iterator node = new_finish.node + 1; node < finish.node; ++node)
            {
                data_allocator::deallocate(*node, buf_size);
            }
            finish = new_finish;
        }
        return start + elems_before;
    }
}

template<typename T, typename Alloc>
typename deque<T, Alloc>::iterator deque<T, Alloc>::insert(iterator position, const value_type &value)
{
    if(position.cur == start.cur)
    {
        push_front(value);
        return start;
    }
    else if(position.cur == finish.cur)
    {
        push_back(value);
        iterator retq = finish;
        --retq;
        return retq;
    }
    else
    {
        return insert_aux(position, value);
    }
}

template<typename T, typename Alloc>
typename deque<T, Alloc>::iterator deque<T, Alloc>::insert_aux(iterator position, const value_type &value)
{
    difference_type elems_before = position - start;
    // 移动左边元素
    /**
     * 正向copy， 反向copy会导致错误
     *   first              last
     *     ｜｜｜｜｜｜｜｜｜｜｜｜
     *   ｜｜｜｜｜｜｜｜｜｜｜｜
     * result
     */
    if(elems_before < (size() >> 1))
    {
        // uninitialized_insert
        push_front(front());
        mystl::copy(start + 2, position + 1, start + 1);
    }
    else
    {
        // uninitialized_insert
        push_back(back());
        mystl::copy_backward(position, finish - 1, finish);
    }
    *position = value;
    return position;
}

}

#endif //MYTINYSTL_DEQUE_H
