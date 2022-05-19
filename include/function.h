#ifndef MYTINYSTL_FUNCTION_H
#define MYTINYSTL_FUNCTION_H

namespace mystl
{
template<typename Arg1, typename Arg2, typename Result>
struct binary_function
{
    typedef Arg1    first_argument_type;
    typedef Arg2    second_argument_type;
    typedef Result  result_type;
};

template<typename T>
struct less : public binary_function<T, T, bool>
{
    bool operator()(const T& x, const T& y) const
    {
        return x < y;
    }
};

template<typename T>
struct greater : public binary_function<T, T, bool>
{
    bool operator()(const T& x, const T& y) const
    {
        return x > y;
    }
};

} // namespace mystl

#endif
