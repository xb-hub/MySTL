//
// Created by 许斌 on 2022/3/10.
//

#include "vector.h"
using namespace mystl;
struct student
{
    int no;
    char* name;
};

int main()
{
//    vector<student> array(10, student());
//    for(int i = 0; i < 12; i++)
//    {
//        std::cout << array[i].no << std::endl;
//    }
    vector<int> array(15, 1);
    array.push_back(2);
    array.push_back(3);
    for(int i = 0; i < 17; i++)
    {
        std::cout << array[i] << std::endl;
    }
    array.pop_back();
    array.push_back(4);
    array.clear();
    std::cout << array.size() << std::endl;
//    for(int i = 0; i < 17; i++)
//    {
//        std::cout << array[i] << std::endl;
//    }
}