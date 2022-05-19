#include "set.h"

int main()
{
    mystl::set<int, mystl::greater<int>> s;
    s.insert(20);
    s.insert(10);
    s.insert(15);
    s.insert(20);
    s.insert(10);
    s.insert(15);
    mystl::multiset<int, mystl::greater<int>> mus;
    mus.insert(20);
    mus.insert(10);
    mus.insert(15);
    mus.insert(20);
    mus.insert(10);
    mus.insert(15);
    for(auto it = s.begin(); it != s.end(); it++)
    {
        std::cout << *it << std::endl;
    }
    std::cout << "---------------------" << std::endl;
    for(auto it = mus.begin(); it != mus.end(); it++)
    {
        std::cout << *it << std::endl;
    }
}