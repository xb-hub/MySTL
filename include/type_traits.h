//
// Created by 许斌 on 2022/3/11.
//

#ifndef MYTINYSTL_TYPE_TRAITS_H
#define MYTINYSTL_TYPE_TRAITS_H
#include <cstddef>
#include <new>

namespace mystl
{
template<typename T, T v> 
struct m_integral_constant
{
    /**
     * static constexpr必须在类内声明初始化，类外定义。在类内声明初始化后式真正的const
     * static const 只有整型数据可以在类内声明初始化，不是整型只能在类外初始化
     * static 只能在类内声明，类外定义初始化
     * 
     * const 具有双重语义：1.只读变量，本质还是变量(可能通过引用更改值) 2.常量     constexpr：常量，编译器在编译程序时可以顺带将其结果计算出来，而无需等到程序运行阶段，这样的优化极大地提高了程序的执行效率。
     * 凡是表达“只读”语义的场景都使用const，表达“常量”语义的场景都使用 constexpr
     */
    static constexpr T  value = v;
    using value_type = T;
    using type = m_integral_constant<T, v>;
    // 隐式转换成T类型
    constexpr operator value_type() const noexcept { return value; }
};

using m_fasle_type = m_integral_constant<bool, false>;
using m_true_type = m_integral_constant<bool, true>;

// is_POD_type
template<typename T>
struct is_POD_type : mystl::m_fasle_type {};
template<>
struct is_POD_type<char> : mystl::m_true_type {};
template<>
struct is_POD_type<signed char> : mystl::m_true_type {};
template<>
struct is_POD_type<unsigned char> : mystl::m_true_type {};
template<>
struct is_POD_type<short> : mystl::m_true_type {};
template<>
struct is_POD_type<unsigned short> : mystl::m_true_type {};
template<>
struct is_POD_type<int> : mystl::m_true_type {};
template<>
struct is_POD_type<unsigned int> : mystl::m_true_type {};
template<>
struct is_POD_type<long> : mystl::m_true_type {};
template<>
struct is_POD_type<unsigned long> : mystl::m_true_type {};
template<>
struct is_POD_type<float> : mystl::m_true_type {};
template<>
struct is_POD_type<double> : mystl::m_true_type {};
template<>
struct is_POD_type<long double> : mystl::m_true_type {};
template<typename T>
struct is_POD_type<T*> : mystl::m_true_type {};

// has_trivial_destructor
template<typename T>
struct has_trivial_destructor : mystl::m_fasle_type {};
template<>
struct has_trivial_destructor<char> : mystl::m_true_type {};
template<>
struct has_trivial_destructor<signed char> : mystl::m_true_type {};
template<>
struct has_trivial_destructor<unsigned char> : mystl::m_true_type {};
template<>
struct has_trivial_destructor<short> : mystl::m_true_type {};
template<>
struct has_trivial_destructor<unsigned short> : mystl::m_true_type {};
template<>
struct has_trivial_destructor<int> : mystl::m_true_type {};
template<>
struct has_trivial_destructor<unsigned int> : mystl::m_true_type {};
template<>
struct has_trivial_destructor<long> : mystl::m_true_type {};
template<>
struct has_trivial_destructor<unsigned long> : mystl::m_true_type {};
template<>
struct has_trivial_destructor<float> : mystl::m_true_type {};
template<>
struct has_trivial_destructor<double> : mystl::m_true_type {};
template<>
struct has_trivial_destructor<long double> : mystl::m_true_type {};
template<typename T>
struct has_trivial_destructor<T*> : mystl::m_true_type {};

// has_trivial_default_constructor
template<typename T>
struct has_trivial_default_constructor : mystl::m_fasle_type {};
template<>
struct has_trivial_default_constructor<char> : mystl::m_true_type {};
template<>
struct has_trivial_default_constructor<signed char> : mystl::m_true_type {};
template<>
struct has_trivial_default_constructor<unsigned char> : mystl::m_true_type {};
template<>
struct has_trivial_default_constructor<short> : mystl::m_true_type {};
template<>
struct has_trivial_default_constructor<unsigned short> : mystl::m_true_type {};
template<>
struct has_trivial_default_constructor<int> : mystl::m_true_type {};
template<>
struct has_trivial_default_constructor<unsigned int> : mystl::m_true_type {};
template<>
struct has_trivial_default_constructor<long> : mystl::m_true_type {};
template<>
struct has_trivial_default_constructor<unsigned long> : mystl::m_true_type {};
template<>
struct has_trivial_default_constructor<float> : mystl::m_true_type {};
template<>
struct has_trivial_default_constructor<double> : mystl::m_true_type {};
template<>
struct has_trivial_default_constructor<long double> : mystl::m_true_type {};
template<typename T>
struct has_trivial_default_constructor<T*> : mystl::m_true_type {};

// has_trivial_copy_constructor
template<typename T>
struct has_trivial_copy_constructor : mystl::m_fasle_type {};
template<>
struct has_trivial_copy_constructor<char> : mystl::m_true_type {};
template<>
struct has_trivial_copy_constructor<signed char> : mystl::m_true_type {};
template<>
struct has_trivial_copy_constructor<unsigned char> : mystl::m_true_type {};
template<>
struct has_trivial_copy_constructor<short> : mystl::m_true_type {};
template<>
struct has_trivial_copy_constructor<unsigned short> : mystl::m_true_type {};
template<>
struct has_trivial_copy_constructor<int> : mystl::m_true_type {};
template<>
struct has_trivial_copy_constructor<unsigned int> : mystl::m_true_type {};
template<>
struct has_trivial_copy_constructor<long> : mystl::m_true_type {};
template<>
struct has_trivial_copy_constructor<unsigned long> : mystl::m_true_type {};
template<>
struct has_trivial_copy_constructor<float> : mystl::m_true_type {};
template<>
struct has_trivial_copy_constructor<double> : mystl::m_true_type {};
template<>
struct has_trivial_copy_constructor<long double> : mystl::m_true_type {};
template<typename T>
struct has_trivial_copy_constructor<T*> : mystl::m_true_type {};

// this_dummy_member_must_be_first
template<typename T>
struct this_dummy_member_must_be_first : mystl::m_fasle_type {};
template<>
struct this_dummy_member_must_be_first<char> : mystl::m_true_type {};
template<>
struct this_dummy_member_must_be_first<signed char> : mystl::m_true_type {};
template<>
struct this_dummy_member_must_be_first<unsigned char> : mystl::m_true_type {};
template<>
struct this_dummy_member_must_be_first<short> : mystl::m_true_type {};
template<>
struct this_dummy_member_must_be_first<unsigned short> : mystl::m_true_type {};
template<>
struct this_dummy_member_must_be_first<int> : mystl::m_true_type {};
template<>
struct this_dummy_member_must_be_first<unsigned int> : mystl::m_true_type {};
template<>
struct this_dummy_member_must_be_first<long> : mystl::m_true_type {};
template<>
struct this_dummy_member_must_be_first<unsigned long> : mystl::m_true_type {};
template<>
struct this_dummy_member_must_be_first<float> : mystl::m_true_type {};
template<>
struct this_dummy_member_must_be_first<double> : mystl::m_true_type {};
template<>
struct this_dummy_member_must_be_first<long double> : mystl::m_true_type {};
template<typename T>
struct this_dummy_member_must_be_first<T*> : mystl::m_true_type {};

// is_pair
template<typename T>
struct is_pair : std::false_type {};
// is_pair偏特化
template<typename T1, typename T2>
struct is_pair<std::pair<T1, T2>> : std::true_type {};
}

#endif //MYTINYSTL_TYPE_TRAITS_H
