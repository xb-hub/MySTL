#include "heap_algo.h"
#include <iostream>
#include <vector>

int main()
{
    std::vector<int> heap = {0, 1, 3, 4, 8, 9, 3, 5};
    mystl::make_heap(heap.begin(), heap.end());
    for(auto it : heap) std::cout << it << " ";
    std::cout << std::endl;
    heap.push_back(7);
    mystl::push_heap(heap.begin(), heap.end());
    for(auto it : heap) std::cout << it << " ";
    std::cout << std::endl;
    mystl::pop_heap(heap.begin(), heap.end());
    std::cout << heap.back() << std::endl;
    heap.pop_back();
    mystl::sort_heap(heap.begin(), heap.end());
    for(auto it : heap) std::cout << it << " ";
    std::cout << std::endl;
}