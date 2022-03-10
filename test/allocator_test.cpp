//
// Created by 许斌 on 2022/3/10.
//

#include "allocator.h"
using namespace mystl;
int main()
{
    allocator<int> *alloc = new allocator<int>();
    const int& x = 5;
    alloc->construct(alloc->allocate(), x);
}

