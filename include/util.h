//
// Created by 许斌 on 2022/3/8.
//

#ifndef MYTINYSTL_UTIL_H
#define MYTINYSTL_UTIL_H
#include <type_traits>

namespace mystl
{
template<typename T>
typename std::remove_reference<T>::type move(T&& arg) noexcept
{
    return static_cast<typename std::remove_reference<T>::type&&>(arg);
}

template<typename T>
T&& forward(typename std::remove_reference<T>::type& arg) noexcept
{
    return static_cast<T&&>(arg);
}

template<typename T>
T&& forward(typename std::remove_reference<T>::type&& arg) noexcept
{
    static_assert(!std::is_lvalue_reference<T>::value, "bad forward");
    return static_cast<T&&>(arg);
}

} // namespace mystl

#endif //MYTINYSTL_UTIL_H
