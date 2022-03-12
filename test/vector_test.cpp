//
// Created by 许斌 on 2022/3/10.
//

#include "vector.h"
using namespace mystl;

int main()
{
    vector<int> array(10, 1);
    for(int i = 0; i < 10; i++)
    {
        std::cout << array[i] << std::endl;
    }
}