//
// Created by 许斌 on 2022/8/24.
//
#include "hashtable.h"
using namespace mystl;

int main()
{
//    hashtable<int, hash<int>, equal_to<int>> h(50, hash<int>(), equal_to<int>());
    hashtable<std::pair<int, std::string>, hash<int>, equal_to<int>> h(101, hash<int>(), equal_to<int>());
    std::cout << h.element_size() << std::endl;
    h.insert_unique(std::make_pair(4, "1"));
    h.insert_unique(std::make_pair(104, "3"));
    h.insert_unique(std::make_pair(103, "2"));
    h.insert_unique(std::make_pair(105, "4"));
    h.insert_unique(std::make_pair(310, "1"));
    h.insert_unique(std::make_pair(870, "3"));
    h.insert_unique(std::make_pair(240, "2"));
    h.insert_unique(std::make_pair(150, "4"));
    h.insert_unique(std::make_pair(270, "1"));
    h.insert_unique(std::make_pair(490, "3"));
    h.insert_unique(std::make_pair(560, "2"));
    h.insert_unique(std::make_pair(340, "4"));
    std::cout << h.element_size() << std::endl;
//    h.insert_unique(1);
//    h.insert_unique(2);
//    h.insert_unique(3);
//    h.insert_unique(4);
    std::cout << h.bucket_size() << std::endl;
//    hashtable<int, hash<int>, equal_to<int>>::iterator it = h.begin();
    hashtable<std::pair<int, std::string>, hash<int>, equal_to<int>>::iterator it = h.begin();
    for(size_t i = 0; i < h.element_size(); ++it, i++)
    {
        std::cout << it->first << " " << it->second << std::endl;
    }
}