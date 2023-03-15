#ifndef MYTINYSTL_SET_H
#define MYTINYSTL_SET_H

#include "rb_tree.h"

namespace mystl
{

template<typename T, typename Compare = mystl::less<T>, typename Alloc = alloc>
class set
{
public:
    using key_type = T;
    using value_type = T;
    using data_type = T;
    using key_compare = Compare;

    using rb_tree_type = rb_tree<T, Compare, Alloc>;
    using value_traits = typename rb_tree_type::value_traits;
    using pointer = typename rb_tree_type::pointer;
    using reference = typename rb_tree_type::reference;
    using const_pointer = typename rb_tree_type::const_pointer;
    using const_reference = typename rb_tree_type::const_reference;
    // 因为set的key==value，所以不能随意更改value值，iterator由const_iterator定义
    using iterator = typename rb_tree_type::const_iterator;
    using const_iterator = typename rb_tree_type::const_iterator;
    using size_type = typename rb_tree_type::size_type;
    using difference_type = typename rb_tree_type::difference_type;

public:
    set() : tree(Compare()) {}
    explicit set(const Compare& comp) : tree(comp) {}
    ~set() {}

public:
    iterator begin() const { return tree.begin(); }
    iterator end() const { return tree.end(); }
    bool empty() const { return tree.empty(); }
    size_type size() const { return tree.size(); }

public:
    std::pair<iterator, bool>  insert(const data_type& data) { return tree.insert_unique(data); }

private:
    rb_tree_type tree;
};


template<typename T, typename Compare = mystl::less<T>, typename Alloc = alloc>
class multiset
{
public:
    using key_type = T;
    using value_type = T;
    using data_type = T;
    using key_compare = Compare;

    using rb_tree_type = rb_tree<T, Compare, Alloc>;
    using value_traits = typename rb_tree_type::value_traits;
    using pointer = typename rb_tree_type::pointer;
    using reference = typename rb_tree_type::reference;
    using const_pointer = typename rb_tree_type::const_pointer;
    using const_reference = typename rb_tree_type::const_reference;
    // 因为set的key==value，所以不能随意更改value值，iterator由const_iterator定义
    using iterator = typename rb_tree_type::const_iterator;
    using const_iterator = typename rb_tree_type::const_iterator;
    using size_type = typename rb_tree_type::size_type;
    using difference_type = typename rb_tree_type::difference_type;

public:
    multiset() : tree(Compare()) {}
    explicit multiset(const Compare& comp) : tree(comp) {}
    ~multiset() {}

public:
    iterator begin() const { return tree.begin(); }
    iterator end() const { return tree.end(); }
    bool empty() const { return tree.empty(); }
    size_type size() const { return tree.size(); }

public:
    iterator  insert(const data_type& data) { return tree.insert_equal(data); }

private:
    rb_tree_type tree;
};

} // namespace mystl


#endif