//
// Created by 许斌 on 2022/3/30.
//
#include "list.h"
using namespace mystl;

int main()
{
    mystl::list<int> l;
    mystl::list<int> l1;
    l1.push_back(10);
    l1.push_back(9);
    l1.push_back(8);
    l1.push_back(7);
    l1.push_back(6);
    l.push_back(5);
    l.push_back(4);
    l.push_back(3);
    l.push_back(2);
    l.push_back(1);
    list<int>::iterator first = l.begin();
    list<int>::iterator last = l.end();
    list<int>::iterator l_pos = l1.begin();
    l1.splice(l_pos, l );
    list<int>::iterator l_first = l1.begin();
    list<int>::iterator l_last = l1.end();
    while (l_first != l_last)
    {
        std::cout << *(l_first++) << " - ";
    }
}

