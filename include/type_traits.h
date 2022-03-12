//
// Created by 许斌 on 2022/3/11.
//

#ifndef MYTINYSTL_TYPE_TRAITS_H
#define MYTINYSTL_TYPE_TRAITS_H
#include <cstddef>
#include <new>

namespace mystl
{
    template<class type>
    struct type_traits
    {
        typedef std::false_type this_dummy_member_must_be_first;
        typedef std::false_type has_trivial_default_constructor;
        typedef std::false_type has_trivial_copy_constructor;
        typedef std::false_type has_trivial_destructor;
        typedef std::false_type is_POD_type;
    };

    template<>
    struct type_traits<char>
    {
        typedef std::true_type this_dummy_member_must_be_first;
        typedef std::true_type has_trivial_default_constructor;
        typedef std::true_type has_trivial_copy_constructor;
        typedef std::true_type has_trivial_destructor;
        typedef std::true_type is_POD_type;
    };

    template<>
    struct type_traits<signed char>
    {
        typedef std::true_type this_dummy_member_must_be_first;
        typedef std::true_type has_trivial_default_constructor;
        typedef std::true_type has_trivial_copy_constructor;
        typedef std::true_type has_trivial_destructor;
        typedef std::true_type is_POD_type;
    };

    template<>
    struct type_traits<unsigned char>
    {
        typedef std::true_type this_dummy_member_must_be_first;
        typedef std::true_type has_trivial_default_constructor;
        typedef std::true_type has_trivial_copy_constructor;
        typedef std::true_type has_trivial_destructor;
        typedef std::true_type is_POD_type;
    };

    template<>
    struct type_traits<short>
    {
        typedef std::true_type this_dummy_member_must_be_first;
        typedef std::true_type has_trivial_default_constructor;
        typedef std::true_type has_trivial_copy_constructor;
        typedef std::true_type has_trivial_destructor;
        typedef std::true_type is_POD_type;
    };

    template<>
    struct type_traits<unsigned short>
    {
        typedef std::true_type this_dummy_member_must_be_first;
        typedef std::true_type has_trivial_default_constructor;
        typedef std::true_type has_trivial_copy_constructor;
        typedef std::true_type has_trivial_destructor;
        typedef std::true_type is_POD_type;
    };

    template<>
    struct type_traits<int>
    {
        typedef std::true_type this_dummy_member_must_be_first;
        typedef std::true_type has_trivial_default_constructor;
        typedef std::true_type has_trivial_copy_constructor;
        typedef std::true_type has_trivial_destructor;
        typedef std::true_type is_POD_type;
    };

    template<>
    struct type_traits<unsigned int>
    {
        typedef std::true_type this_dummy_member_must_be_first;
        typedef std::true_type has_trivial_default_constructor;
        typedef std::true_type has_trivial_copy_constructor;
        typedef std::true_type has_trivial_destructor;
        typedef std::true_type is_POD_type;
    };

    template<>
    struct type_traits<long>
    {
        typedef std::true_type this_dummy_member_must_be_first;
        typedef std::true_type has_trivial_default_constructor;
        typedef std::true_type has_trivial_copy_constructor;
        typedef std::true_type has_trivial_destructor;
        typedef std::true_type is_POD_type;
    };

    template<>
    struct type_traits<unsigned long>
    {
        typedef std::true_type this_dummy_member_must_be_first;
        typedef std::true_type has_trivial_default_constructor;
        typedef std::true_type has_trivial_copy_constructor;
        typedef std::true_type has_trivial_destructor;
        typedef std::true_type is_POD_type;
    };

    template<>
    struct type_traits<float>
    {
        typedef std::true_type this_dummy_member_must_be_first;
        typedef std::true_type has_trivial_default_constructor;
        typedef std::true_type has_trivial_copy_constructor;
        typedef std::true_type has_trivial_destructor;
        typedef std::true_type is_POD_type;
    };

    template<>
    struct type_traits<double>
    {
        typedef std::true_type this_dummy_member_must_be_first;
        typedef std::true_type has_trivial_default_constructor;
        typedef std::true_type has_trivial_copy_constructor;
        typedef std::true_type has_trivial_destructor;
        typedef std::true_type is_POD_type;
    };

    template<>
    struct type_traits<long double>
    {
        typedef std::true_type this_dummy_member_must_be_first;
        typedef std::true_type has_trivial_default_constructor;
        typedef std::true_type has_trivial_copy_constructor;
        typedef std::true_type has_trivial_destructor;
        typedef std::true_type is_POD_type;
    };

    template<class T>
    struct type_traits<T*>
    {
        typedef std::true_type this_dummy_member_must_be_first;
        typedef std::true_type has_trivial_default_constructor;
        typedef std::true_type has_trivial_copy_constructor;
        typedef std::true_type has_trivial_destructor;
        typedef std::true_type is_POD_type;
    };
}

#endif //MYTINYSTL_TYPE_TRAITS_H
