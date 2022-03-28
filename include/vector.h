//
// Created by 许斌 on 2022/3/10.
//

#ifndef MYTINYSTL_VECTOR_H
#define MYTINYSTL_VECTOR_H
#define __DEBUG
#include <cstddef>
#include "allocator.h"
#include "alloc.h"
#include "uninitialized.h"
#include "algobase.h"

namespace mystl
{
    template<class T, class Alloc = allocator<T>>
    class vector
    {
    public:
        typedef Alloc data_allocator;
        typedef T   value_type;
        typedef ptrdiff_t difference_type;
        typedef value_type*  pointer;
        typedef value_type&  reference;
        typedef value_type*  iterator;
        typedef const value_type *  const_iterator;
        typedef size_t size_type;
        // reverse_iterator

    private:
        iterator begin_;
        iterator end_;
        iterator cap_;

    public:
        // 构造函数
        vector() { try_init(); }
        vector(size_type n) { fill_init(n, value_type()); }
        vector(size_type n, const T& value) { fill_init(n, value); }

        // 拷贝构造函数，移动构造函数
        vector(const vector&) {}
        vector(vector&&) {}

        /******************************** 操作符重载 ********************************/
        reference operator[](size_type n)
        {
            assert(n < size());
            return *(begin_ + n);
        }

        /******************************** 元素操作 ********************************/
        void push_back(const T& value);
        void pop_back();
        void emplace_back(const T& value);

        iterator erase(iterator first, iterator last);
        iterator erase(iterator element);

        void insert(iterator position, size_type n, const T& value);
        void clear() { erase(begin(), end()); }

        size_type size() const { return end_ - begin_; }
        size_type capacity() const { return cap_ - begin_; }

        iterator begin() { return begin_; }
        iterator end() { return end_; }

    private:
        void try_init() noexcept;
        void fill_init(size_type n, const T& value) noexcept;

        template<class Iterator>
        void range_init();

        void init_space(size_type n, size_type init_size);

        void realloc_insert(iterator end, const T& value);
    };

    /******************************** init ********************************/
    template<class T, class Alloc>
    void vector<T, Alloc>::try_init() noexcept
    {
        try
        {
            begin_ = data_allocator::allocate(16);
            end_ = begin_;
            cap_ = begin_ + 16;
        }
        catch (...)
        {
            begin_ = nullptr;
            end_ = nullptr;
            cap_ = nullptr;
            throw "bad init!";
        }
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::fill_init(size_type n, const T &value) noexcept
    {
        const size_type init_size = mystl::max(static_cast<size_type>(16), n);
        init_space(n, init_size);
        mystl::uninitialized_fill_n(begin_, n, value);
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::init_space(size_type n, size_type init_size)
    {
        try
        {
            begin_ = static_cast<iterator>(data_allocator::allocate(init_size));
            end_ = begin_ + n;
            cap_ = begin_ + init_size;
#ifdef __DEBUG
//            std::cout << "begin: " << begin_ << std::endl;
//            std::cout << "end: " << end_ << std::endl;
//            std::cout << "cap: " << cap_ << std::endl;
            printf("%x\n%x\n%x\n", begin_, end_, cap_);
#endif
        }
        catch (...)
        {
            begin_ = nullptr;
            end_ = nullptr;
            cap_ = nullptr;
            throw "bad init!";
        }
    }

    /******************************** 元素操作 ********************************/
    template<class T, class Alloc>
    void vector<T, Alloc>::push_back(const T &value)
    {
#ifdef __DEBUG
        std::cout << "push_back: " << value << std::endl;
#endif
        if(end_ != cap_)
        {
            mystl::construct(end_, value);
            ++end_;
        }
        else
        {
            realloc_insert(end_, value);
        }
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::realloc_insert(iterator position,const T &value)
    {
#ifdef __DEBUG
        std::cout << "realloc!" << std::endl;
#endif
        size_type old_size = capacity();
        size_type new_size = old_size == 0 ? 1 : 2 * old_size;
        auto new_begin = data_allocator::allocate(new_size);
        auto new_end = new_begin;
        try
        {
            new_end = mystl::uninitialized_copy(begin_, position, new_begin);
            mystl::construct(new_end, value);
            new_end++;
            // ???
            new_end = mystl::uninitialized_copy(position, end_, new_end);
        }
        catch (...)
        {
            mystl::destory(new_begin, new_end);
            data_allocator::deallocate(new_begin, new_size);
            throw "bad realloc!";
        }

        // 释放原vector
        mystl::destory(begin_, end_);
        data_allocator::deallocate(begin_, capacity());

        begin_ = new_begin;
        end_ = new_end;
        cap_ =  new_begin + new_size;
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::pop_back()
    {
        end_--;
        mystl::destory(end_);
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator element)
    {
        if(element + 1 != end_)
        {
            mystl::copy(element + 1, end_, element);
        }
        destory(end_);
        end_--;
        return element;
    }

    template<class T, class Alloc>
    typename vector<T, Alloc>::iterator vector<T, Alloc>::erase(iterator first, iterator last)
    {
        if(last + 1 != end_)
        {
            mystl::copy(last, end_, first);
        }
        end_ = end_ - (last - first);
        destory(first, last);
        return first;
    }

    template<class T, class Alloc>
    void vector<T, Alloc>::insert(iterator position, size_type n, const T &value)
    {
        
    }

} // namespace mystl

#endif //MYTINYSTL_VECTOR_H
