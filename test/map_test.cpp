#include "map.h"

int main()
{
    mystl::map<int, int, mystl::greater<int>> m;
    m.insert({3, 2});
    m.insert({2, 3});
    m.insert({4, 1});
    m.insert({1, 4});
    m.insert({3, 2});
    m.insert({2, 3});
    m.insert({4, 1});
    m.insert({1, 4});
    mystl::multimap<int, int, mystl::greater<int>> mum;
    mum.insert({3, 2});
    mum.insert({2, 3});
    mum.insert({4, 1});
    mum.insert({1, 4});
    mum.insert({3, 2});
    mum.insert({2, 3});
    mum.insert({4, 1});
    mum.insert({1, 4});
    for(auto it = m.begin(); it != m.end(); it++)
    {
        std::cout << "{" << it->first << ", " << it->second << "}" << std::endl;
    }
    std::cout << "---------------------" << std::endl;
    for(auto it = mum.begin(); it != mum.end(); it++)
    {
        std::cout << "{" << it->first << ", " << it->second << "}" << std::endl;
    }
}