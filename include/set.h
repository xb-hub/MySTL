#ifndef MYTINYSTL_SET_H
#define MYTINYSTL_SET_H

#include "rb_tree.h"

namespace mystl
{

template<typename T, typename Compare = mystl::less<T>, typename Alloc = alloc>
class set
{
public:
    typedef T   key_type;
    typedef T   value_type;
    typedef T   data_type;
    typedef Compare key_compare;

    typedef rb_tree<T, Compare, Alloc>              rb_tree_type;
    typedef typename rb_tree_type::value_traits     value_traits;
    typedef typename rb_tree_type::pointer          pointer;
    typedef typename rb_tree_type::reference        reference;
    typedef typename rb_tree_type::const_pointer    const_pointer;
    typedef typename rb_tree_type::const_reference  const_reference;
    // 因为set的key==value，所以不能随意更改value值，iterator由const_iterator定义
    typedef typename rb_tree_type::const_iterator   iterator;
    typedef typename rb_tree_type::const_iterator   const_iterator;
    typedef typename rb_tree_type::size_type        size_type;
    typedef typename rb_tree_type::difference_type  difference_type;

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
    typedef T   key_type;
    typedef T   value_type;
    typedef T   data_type;
    typedef Compare key_compare;

    typedef rb_tree<T, Compare, Alloc>              rb_tree_type;
    typedef typename rb_tree_type::value_traits     value_traits;
    typedef typename rb_tree_type::pointer          pointer;
    typedef typename rb_tree_type::reference        reference;
    typedef typename rb_tree_type::const_pointer    const_pointer;
    typedef typename rb_tree_type::const_reference  const_reference;
    // 因为set的key==value，所以不能随意更改value值，iterator由const_iterator定义
    typedef typename rb_tree_type::const_iterator   iterator;
    typedef typename rb_tree_type::const_iterator   const_iterator;
    typedef typename rb_tree_type::size_type        size_type;
    typedef typename rb_tree_type::difference_type  difference_type;

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