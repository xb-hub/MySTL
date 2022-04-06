//
// Created by 许斌 on 2022/4/6.
//

#ifndef MYTINYSTL_QUEUE_H
#define MYTINYSTL_QUEUE_H
#include "deque.h"

namespace mystl
{
    template<class T, class Sequence = deque<T>>
    class queue
    {
    public:
        typedef typename Sequence::value_type value_type;
        typedef typename Sequence::size_type size_type;
        typedef typename Sequence::reference reference;
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
