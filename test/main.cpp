#include <iostream>
#include <cstdlib>

union FreeList
{
    union FreeList* free_list_link;
    char* address[1];
};

// typename.cpp
template<class T>
class X
{
public:
    typename T::Y m_y;   // treat Y as a type
};

template<typename T>
class Z
{
public:
    typedef T Y;
};

int main()
{
    X<Z<int>> x;
    std::cout << typeid(x.m_y).name() << std::endl;
}
