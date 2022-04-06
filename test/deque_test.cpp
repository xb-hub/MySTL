//
// Created by 许斌 on 2022/3/31.
//
#include <iostream>
#include "deque.h"

int main()
{
    mystl::deque<int> q;
    for(int i = 0; i < 20; i++)
    {
        q.push_front(i);
    }
//    q.pop_front();
//    q.pop_back();
//    q.clear();
//    std::cout << q.begin().cur << "---" <<  (q.begin() + 1).cur << std::endl;
    q.insert(q.begin() + 13, 100);
    for(mystl::deque<int>::iterator it = q.begin(); it != q.end(); it++)
    {
        std::cout << *it << std::endl;
    }
}