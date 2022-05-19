#include "priority_queue.h"

int main()
{
    mystl::priority_queue<int, mystl::vector<int>, mystl::greater<int>> q;
    q.push(2);
    q.push(0);
    q.push(1);
    q.push(3);
    q.push(5);
    q.push(4);
    while (!q.empty())
    {
        std::cout << q.top() << " ";
        q.pop();
    } 
    return 0;
}