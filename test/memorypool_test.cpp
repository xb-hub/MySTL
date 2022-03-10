//
// Created by 许斌 on 2022/3/7.
//
#include "alloc.h"
#include <iostream>
using namespace mystl;

int main()
{
    alloc *alloc_ = new alloc();
    int* point = (int*)alloc_->allocate(128);
    int* point1 = (int*)alloc_->allocate(128);
    int* point2 = (int*)alloc_->allocate(128);
}

