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
    mystl::vector<int> array(15, 1);
    mystl::vector<int> a;
    array.push_back(2);
    array.push_back(3);
    for(int i = 0; i < 17; i++)
    {
        std::cout << array[i] << "-";
    }
    array.insert(array.begin(), 10, 5);
    std::cout << std::endl;
    array.erase(array.end() - 1);
    for(int i = 0; i < 26; i++)
    {
        std::cout << array[i] << "-";
    }
    std::cout << std::endl;
    array.pop_back();
    array.push_back(4);
    array.clear();
    std::cout << array.size() << std::endl;
    std::cout << array.capacity() << std::endl;
    array.reserve(50);
    std::cout << array.size() << std::endl;
    std::cout << array.capacity() << std::endl;
//    for(int i = 0; i < 17; i++)
//    {
//        std::cout << array[i] << std::endl;
//    }
}