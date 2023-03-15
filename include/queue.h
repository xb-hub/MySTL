//
// Created by 许斌 on 2022/4/6.
//

#ifndef MYTINYSTL_QUEUE_H
#define MYTINYSTL_QUEUE_H
#include "deque.h"

namespace mystl
{
template<typename T, typename Sequence = deque<T>>
class queue
{
public:
    using value_type = typename Sequence::value_type;
    using size_type = typename Sequence::size_type;
    using reference = typename Sequence::reference;
private:
    Sequence c;
public:
    bool empty(); { return c.empty(); }
    size_type size() { return c.size(); }
    void push(const T& value) { c.push_back(value); }
    void pop() { c.pop_front(); }
};

}

#endif //MYTINYSTL_QUEUE_H
