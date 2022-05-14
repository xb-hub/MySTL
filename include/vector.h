//
// Created by 许斌 on 2022/3/10.
//

#ifndef MYTINYSTL_VECTOR_H
#define MYTINYSTL_VECTOR_H
// #define __DEBUG

#include <cstddef>
#include "allocator.h"
#include "alloc.h"
#include "uninitialized.h"
#include "algobase.h"

namespace mystl
{
    template<class T, class Alloc = alloc>
    class vector
    {
    public:
        typedef simple_alloc<T, Alloc> data_allocator;
        typedef T   value_type;
        typedef ptrdiff_t difference_type;
        typedef value_type*  pointer;
        typedef value_type&  reference;
        typedef const value_type& const_reference;
        typedef value_type*  iterator;
        typedef const value_type*  const_iterator;
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
        vector(const vector& rhs) { range_init(); }
        // 移动构造直接偷取临时变量的元素，将指针指向临时变量数据，偷取后销毁临时变量
        vector(vector&& rhs) :
                begin_(rhs.begin_),
                end_(rhs.end_),
                cap_(rhs.cap_)
        {
            rhs.begin_ = nullptr;
            rhs.end_ = nullptr;
            rhs.cap_ = nullptr;
        }

        // 赋值需要考虑cap不足的情况
        vector& operator=(const vector& rhs);
        vector& operator=(const vector&& rhs);
        // 列表初始化
        vector& operator=(std::initializer_list<value_type> rhs) {}

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

        reference front() { return *begin(); }
        reference back() { return *(end() - 1); }
        const_reference front() const { return *begin_; }
        const_reference back() const { return *(end_ - 1); }

        iterator erase(iterator first, iterator last);
        iterator erase(iterator element);

        void insert(iterator position, size_type n, const T& value);
        void clear() { erase(begin(), end()); }

        bool empty() const { return begin_ == end_; }
        size_type size() const { return end_ - begin_; }
        size_type capacity() const { return cap_ - begin_; }

        iterator begin() { return begin_; }
        iterator end() { return end_; }

    private:
        void try_init() noexcept;
        void fill_init(size_type n, const T& value) noexcept;

        template<class Iterator>
        void range_init(Iterator first, Iterator last);

        void init_space(size_type n, size_type init_size);

        void realloc_insert(iterator end, const T& value);
        void realloc_insert_n(iterator position, const size_type n, const T& value);
    };

    template<class T, class Alloc>
    vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc> &rhs)
    {
        if(*this == rhs)    return *this;
        // 如果当前capacity不足
        if(rhs.size() > this->capacity())
        {

        }
        else if(size() >= rhs.size())
        {
            auto iter = mystl::copy(rhs.begin_, rhs.end_, begin_);
            mystl::destory(iter, end_);
            end_ = iter;
        }
        else
        {
            auto iter = mystl::copy(rhs.begin_, rhs.begin_ + size(), begin_);
            mystl::uninitialized_copy(rhs.begin_ + size(), rhs.end_, iter);
            end_ = begin_ + rhs.size();
        }
        return *this;
    }

    template<class T, class Alloc>
    vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc> &&rhs)
    {
        // 析构对象
        mystl::destory(begin_, end_);
        // 收回分配内存
        data_allocator::deallocate(begin_, capacity());
        begin_ = rhs.begin_;
        end_ = rhs.end_;
        cap_ = rhs.cap_;
        rhs.begin_ = nullptr;
        rhs.end_ = nullptr;
        rhs.cap_ = nullptr;
        return *this;
    }

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
    template<class Iterator>
    void vector<T, Alloc>::range_init(Iterator first, Iterator last)
    {
        size_type init_size = mystl::max(static_cast<size_type>(last - first), static_cast<size_type>(16));
        init_space(static_cast<size_type>(last - first), init_size);
        mystl::uninitialized_copy(first, last, begin_);
    }

    /**
     * @param n 存放元素个数
     * @param init_size 申请内存容量
     */
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
    void vector<T, Alloc>::realloc_insert_n(iterator position, const size_type n, const T &value)
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
//            mystl::construct(new_end, value);
//            new_end++;
            new_end = mystl::uninitialized_fill_n(new_end, n, value);
            // insert后将后面数据加上
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
    void vector<T, Alloc>::realloc_insert(iterator position,const T &value)
    {
        realloc_insert_n(position, 1, value);
//#ifdef __DEBUG
//        std::cout << "realloc!" << std::endl;
//#endif
//        size_type old_size = capacity();
//        size_type new_size = old_size == 0 ? 1 : 2 * old_size;
//        auto new_begin = data_allocator::allocate(new_size);
//        auto new_end = new_begin;
//        try
//        {
//            new_end = mystl::uninitialized_copy(begin_, position, new_begin);
//            mystl::construct(new_end, value);
//            new_end++;
//            // insert后将后面数据加上
//            new_end = mystl::uninitialized_copy(position, end_, new_end);
//        }
//        catch (...)
//        {
//            mystl::destory(new_begin, new_end);
//            data_allocator::deallocate(new_begin, new_size);
//            throw "bad realloc!";
//        }
//
//        // 释放原vector
//        mystl::destory(begin_, end_);
//        data_allocator::deallocate(begin_, capacity());
//
//        begin_ = new_begin;
//        end_ = new_end;
//        cap_ =  new_begin + new_size;
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
        assert(element >= begin() && element < end());
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
        assert(first >= begin() && last <= end() && !(last < first));
        if(last != end_)
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
        assert(position >= begin() && position < end());
        if(n > 0)
        {
            const size_type remain = static_cast<size_type>(cap_ - end_);
            // 如果剩余存储空间足够插入
            if(remain >= n)
            {
                // 原来的元素后移一部分使用初始化后的位置，使用fill填充，一部分不使用为初始化的位置，使用uninitialized_copy初始化
                const size_type shift = static_cast<size_type>(end_ - position);  // 需要移动的元素数量
                iterator old_end_ = end_;
                if(shift >= n)
                {
                    // end后元素为进行初始化，调用uninitialized_copy初始化
                    mystl::uninitialized_copy(end_ - n, end_, end_);
                    end_ += n;
                    mystl::copy_backward(position, old_end_ - n, old_end_);
                    mystl::fill(position, position + n, value);
                }
                // 所有元素使用为初始化的位置，position后元素使用uninitialized_copy进行初始化移动，
                // 插入值使用fill填充已初始化部分，使用uninitialized_fill_n填充未初始化部分
                else
                {
                    mystl::uninitialized_copy(position, old_end_, position + n);
                    end_ += n;
                    mystl::uninitialized_fill_n(old_end_, n - shift, value);
                    mystl::fill(position, old_end_, value);
                }
            }
            else
            {
                realloc_insert_n(position, n, value);
            }
        }
    }

} // namespace mystl

#endif //MYTINYSTL_VECTOR_H
