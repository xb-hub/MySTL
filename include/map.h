#ifndef MYTINYSTL_MAP_H
#define MYTINYSTL_MAP_H

#include "rb_tree.h"

namespace mystl
{
template<typename Key, typename T, typename Compare = mystl::less<T>, typename Alloc = alloc>
class map
{
public:
    typedef Key                 key_type;
    typedef T                   value_type;
    typedef std::pair<Key, T>   data_type;
    typedef Compare             key_compare;

    typedef rb_tree<data_type, Compare, Alloc>      rb_tree_type;
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
    map() : tree(Compare()) {}
    explicit map(const Compare& comp) : tree(comp) {}
    ~map() {}

public:
    iterator begin() const { return tree.begin(); }
    iterator end() const { return tree.end(); }
    bool empty() const { return tree.empty(); }
    size_type size() const { return tree.size(); }

//    value_type operator[](const key_type& key)
//    {
//        iterator it =
//    }

public:
    std::pair<iterator, bool> insert(const data_type& data) { return tree.insert_unique(data); }

private:
    rb_tree_type tree;
};


template<typename Key, typename T, typename Compare = mystl::less<T>, typename Alloc = alloc>
class multimap
{
public:
    typedef Key                 key_type;
    typedef T                   value_type;
    typedef std::pair<Key, T>   data_type;
    typedef Compare             key_compare;

    typedef rb_tree<data_type, Compare, Alloc>      rb_tree_type;
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
    multimap() : tree(Compare()) {}
    explicit multimap(const Compare& comp) : tree(comp) {}
    ~multimap() {}

public:
    iterator begin() const { return tree.begin(); }
    iterator end() const { return tree.end(); }
    bool empty() const { return tree.empty(); }
    size_type size() const { return tree.size(); }

public:
    iterator insert(const data_type& data) { return tree.insert_equal(data); }

private:
    rb_tree_type tree;
};

} // namespace mystl


#endif