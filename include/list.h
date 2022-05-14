//
// Created by 许斌 on 2022/3/16.
//

#ifndef MYTINYSTL_LIST_H
#define MYTINYSTL_LIST_H
// #define __DEBUG

#include "allocator.h"
#include "alloc.h"
#include "iterator.h"

namespace mystl
{
    template<class T> struct list_base_node;
    template<class T> struct list_node;

    template<class T>
    struct node_traits
    {
        typedef list_base_node<T>* base_ptr;
        typedef list_node<T>* node_ptr;
    };

    template<class T>
    struct list_base_node
    {
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef typename node_traits<T>::node_ptr node_ptr;

        base_ptr prev;
        base_ptr next;
    };

    template<class T>
    struct list_node : public list_base_node<T>
    {
        T data;
    };

    template<class T>
    struct list_iterator : iterator<bidirectional_iterator_tag, T>
    {
        typedef bidirectional_iterator_tag iterator_category;
        typedef T value_type;
        typedef T* pointer;
        typedef T& reference;
        typedef typename node_traits<T>::node_ptr node_ptr;
        typedef typename node_traits<T>::base_ptr base_ptr;
        typedef list_iterator<T> self;

        base_ptr node;

        list_iterator(base_ptr p) : node(p) {}

        /******************************** 操作符重载 ********************************/
        reference operator*() { return static_cast<node_ptr>(node)->data; }
        pointer operator->() { return &(operator*()); }

        // ++i
        self operator++()
        {
            assert(node != nullptr);
            node = node->next;
            return *this;
        }

        // i++: 先存储当前值，使用++()进行移动，返回原来的指针位置
        self operator++(int)
        {
            self ret = *this;
            ++*this;
            return ret;
        }

        self operator--()
        {
            assert(node != nullptr);
            node = node->prev;
            return *this;
        }

        self operator--(int)
        {
            self ret = *this;
            --*this;
            return ret;
        }

        bool operator==(const self& rhs) { return rhs.node == node;}
        bool operator!=(const self& rhs) { return rhs.node != node; }
    };


    template<class T, class Alloc = alloc>
    class list
    {
    public:
        typedef typename node_traits<T>::node_ptr node_ptr;
        typedef typename node_traits<T>::base_ptr base_ptr;

        typedef simple_alloc<list_base_node<T> , Alloc> base_allocator;
        typedef simple_alloc<list_node<T>, Alloc> node_allocator;
        typedef T   value_type;
        typedef ptrdiff_t difference_type;
        typedef value_type*  pointer;
        typedef value_type&  reference;
        typedef list_iterator<T> iterator;
//        typedef const_list_iterator<T>  const_iterator;
        typedef size_t size_type;

    public:
        list() { empty_initialize(); }

    private:
        base_ptr node;

    public:
        iterator begin() { return node->next; }
        iterator end() { return node; }

        bool empty() { return node->next == node; }

        reference front() { return *begin(); }
        reference back() { return *(--end()); }

    private:
        void empty_initialize();
        node_ptr creat_node(const T& data)
        {
            node_ptr p = node_allocator::allocate();
            // 用data的地址来初始化
            construct(&p->data, data);
            return p;
        }

        void destory_node(node_ptr p)
        {
            destory(&p->data);
            node_allocator::deallocate(p);
        }

    /******************************** 元素操作 ********************************/
    public:
        iterator insert(iterator position, const T& value);
        void push_back(const T& value);
        void push_front(const T& value);

        iterator erase(const iterator position);
        void pop_back();
        void pop_front();

        void clear();
        void remove(const T& value);
        void unique();

        void splice(iterator position, list& l);
        void splice(iterator position, iterator first, iterator last);

        void merge(list& l);
        void sort();
        void reverse();

    private:
        void transfer(iterator position, iterator first, iterator last);
    };

    template<class T, class Alloc>
    void list<T, Alloc>::empty_initialize()
    {
#ifdef __DEBUG
        std::cout << "init" << std::endl;
#endif
        try
        {
            node = base_allocator::allocate();
            node->next = node;
            node->prev = node;
        }
        catch (...)
        {
            node = nullptr;
        }
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::insert(iterator position, const T &value)
    {
        node_ptr insert_node = creat_node(value);
        insert_node->prev = position.node->prev;
        insert_node->next = position.node;
        static_cast<node_ptr>(position.node->prev)->next = insert_node;
        position.node->prev = insert_node;
        return insert_node;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::push_back(const T& value)
    {
        insert(end(), value);
    }

    template<class T, class Alloc>
    void list<T, Alloc>::push_front(const T &value)
    {
        insert(begin(), value);
    }

    template<class T, class Alloc>
    typename list<T, Alloc>::iterator list<T, Alloc>::erase(const iterator position)
    {
        base_ptr next_node = position.node->next;
        position.node->next->prev = position.node->prev;
        position.node->prev->next = next_node;
        destory(position.node);
        return next_node;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::pop_back()
    {
        erase(--end());
    }

    template<class T, class Alloc>
    void list<T, Alloc>::pop_front()
    {
        erase(begin());
    }

    template<class T, class Alloc>
    void list<T, Alloc>::clear()
    {
        base_ptr cur = node->next;
        while (cur != node)
        {
            base_ptr tmp = cur;
            cur = (base_ptr)cur->next;
            destory_node(static_cast<node_ptr>(tmp));
        }
        node->next = node;
        node->prev = node;
    }

    template<class T, class Alloc>
    void list<T, Alloc>::remove(const T& value)
    {
        iterator first = begin();
        iterator last = end();
        while (first != last)
        {
            iterator tmp = first;
            ++tmp;
            if(*first == value) erase(first);
            first = tmp;
        }
    }

    template<class T, class Alloc>
    void list<T, Alloc>::unique()
    {
        iterator first = begin();
        iterator last = end();
        iterator cur = ++first;
        while (cur != last)
        {
            if(*cur == *first) erase(cur);
            else    first = cur;
            cur = ++first;
        }
    }

    /**
     * @param position 迁移位置
     * @param first-last 迁移范围[first, last)
     */
    template<class T, class Alloc>
    void list<T, Alloc>::transfer(iterator position, iterator first, iterator last)
    {
        if(position != last)
        {
            last.node->prev->next = position.node;
            first.node->prev->next = last.node;
            base_ptr tmp = last.node->prev;
            last.node->prev = first.node->prev;
            first.node->prev = position.node->prev;
            position.node->prev->next = first.node;
            position.node->prev = tmp;
        }
    }

    template<class T, class Alloc>
    void list<T, Alloc>::splice(iterator position, list<T, Alloc> &l)
    {
        assert(this != &l);
        transfer(position, l.begin(), l.end());
    }

    template<class T, class Alloc>
    void list<T, Alloc>::splice(iterator position, iterator first, iterator last)
    {
        transfer(position, first, last);
    }

    // 两个有序链表合并(小->大)
    template<class T, class Alloc>
    void list<T, Alloc>::merge(list<T, Alloc> &l)
    {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = l.begin();
        iterator last2 = l.end();

        while (first1 != last1 && first2 != last2)
        {
            if(*first2 < *first1)
            {
                iterator next = first2;
                transfer(first1, first2, ++next);
                first2 = next;
            }
            else
            {
                ++first1;
            }
        }
        if(first2 != last2) transfer(last1, first2, last2);
    }

    template<class T, class Alloc>
    void list<T, Alloc>::sort()
    {

    }

    template<class T, class Alloc>
    void list<T, Alloc>::reverse()
    {

    }
}

#endif //MYTINYSTL_LIST_H
