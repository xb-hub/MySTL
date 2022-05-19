//
// Created by 许斌 on 2022/4/6.
//

#ifndef MYTINYSTL_STACK_H
#define MYTINYSTL_STACK_H
#include "deque.h"

namespace mystl
{
template<typename T, typename Sequence = deque<T>>
class stack
{
public:
    typedef typename Sequence::value_type value_type;
    typedef typename Sequence::size_type size_type;
    typedef typename Sequence::reference reference;

private:
    Sequence c;

public:
    stack() = default;
    stack(size_type n) :
        c(n)
    {}
    stack(size_type n, const T& value) :
        c(n, value)
    {}

    stack(const Sequence& s) :
        c(s)
    {}

public:
    bool empty() { return c.empty(); }
    size_type size() { return c.size(); }
    T top() { return c.back(); }
    void push(const T& value) { c.push_back(value); }
    void pop() { c.pop_back(); }
};

}

#endif //MYTINYSTL_STACK_H
