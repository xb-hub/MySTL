//
// Created by 许斌 on 2022/4/6.
//
#include "stack.h"
using namespace mystl;

int main()
{
    stack<int> s;
    std::cout << s.empty() << std::endl;
    s.push(1);
    std::cout << s.top() << std::endl;
    std::cout << s.empty() << "--" << s.size() << std::endl;
    s.pop();
    std::cout << s.empty() << "--" << s.size() << std::endl;
}
