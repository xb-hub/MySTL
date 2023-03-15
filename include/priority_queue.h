#ifndef MYTINYSTL_PRIORITY_QUEUE_H
#define MYTINYSTL_PRIORITY_QUEUE_H

#include "vector.h"
#include "heap_algo.h"
#include "function.h"

namespace mystl
{
template<typename T, typename Sequence = mystl::vector<T>, typename Compare = mystl::less<typename Sequence::value_type>>
class priority_queue
{
private:
    Sequence c;
    Compare comp;

public:
    using value_type = typename Sequence::value_type;
    using size_type = typename Sequence::size_type;
    using reference = typename Sequence::reference;
    using const_reference = typename Sequence::const_reference;

    priority_queue() 
        : c()
    {}
    // 禁止单参数构造函数隐式转换
    explicit priority_queue(const Compare& fun)
        : c(), comp(fun)
    {}
    template<typename InputIterator>
    priority_queue(InputIterator first, InputIterator last, const Compare& fun)
        : c(first, last), comp(fun)
    { make_heap(c.begin(), c.end(), comp); }
    template<typename InputIterator>
    priority_queue(InputIterator first, InputIterator last)
        : c(first, last)
    { make_heap(c.begin(), c.end(), comp); }

    bool empty() const { return c.empty(); }
    size_type size() const { return c.size(); }
    const_reference top() const { return c.front(); }

    void push(const value_type& value)
    {
        try
        {
            c.push_back(value);
            mystl::push_heap(c.begin(), c.end(), comp);
        }
        catch(...)
        {
            c.clear();
        }
        
    }
    void pop()
    {
        try
        {
            mystl::pop_heap(c.begin(), c.end(), comp);
            c.pop_back();
        }
        catch(...)
        {
            c.clear();
        }   
    }
};

} // namespace mystl


#endif
