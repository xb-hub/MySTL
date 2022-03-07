//
// Created by 许斌 on 2022/3/7.
//
#include "alloc.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
using namespace mystl;

int main()
{
    alloc *alloc_ = new alloc();
    int* point = (int*)alloc_->allocate(128);
    std::cout << malloc_usable_size(point) << std::endl;
    int* point1 = (int*)alloc_->allocate(128);
    std::cout << malloc_usable_size(point1) << std::endl;
}

