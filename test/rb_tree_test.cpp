#include "rb_tree.h"

int main()
{
    mystl::rb_tree<std::pair<int, int> > tree;
    tree.insert_unique({5, 1});
    tree.insert_unique({1, 2});
    tree.insert_unique({3, 3});
    tree.insert_unique({2, 4});
    tree.insert_unique({4, 5});
    tree.insert_unique({6, 7});
    tree.insert_unique({10, 10});
    tree.insert_unique({5, 1});
    tree.insert_unique({1, 2});
    tree.insert_unique({3, 3});
    tree.insert_unique({2, 4});
    tree.insert_unique({4, 5});
    tree.insert_unique({6, 7});
    tree.insert_unique({10, 10});
    for(auto it = tree.begin(); it != tree.end(); it++)
    {
        std::cout << it->first << "---" << it->second << std::endl;
    }
    return 0;
}