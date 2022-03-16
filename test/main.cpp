// is_trivially_copy_assignable example
#include <iostream>
#include <type_traits>

struct A { };
struct B : A {};
struct C {  };

int main() {
    std::cout << std::boolalpha;
    std::cout << "is_trivially_copy_assignable:" << std::endl;
    std::cout << "int: " << std::is_trivially_copy_assignable<int>::value << std::endl;
    std::cout << "A: " << std::is_trivially_copy_assignable<A>::value << std::endl;
    std::cout << "B: " << std::is_trivially_copy_assignable<B>::value << std::endl;
    std::cout << "C: " << std::is_trivially_copy_assignable<C>::value << std::endl;
    return 0;
}