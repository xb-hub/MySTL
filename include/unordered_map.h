//
// Created by 许斌 on 2022/8/25.
//

#ifndef MYTINYSTL_UNORDERED_MAP_H
#define MYTINYSTL_UNORDERED_MAP_H

#include "hashtable.h"

template<typename Key, typename T, typename HashFcn = mystl::hash<Key>, typename KeyEqual = mystl::equal_to<Key>, typename Alloc = mystl::alloc>
class unordered_map
{
public:
    typedef mystl::hashtable<std::pair<Key, T>, HashFcn, KeyEqual, Alloc> hashtable;
    typedef std::pair<Key, T>   data_type;
    typedef T                   value_type;
    typedef Key                 key_type;
    typedef HashFcn             hasher;
    typedef KeyEqual            equals;

    typedef typename hashtable::iterator iterator;
    typedef typename hashtable::pointer  pointer;
    typedef typename hashtable::reference reference;
    typedef typename hashtable::size_type size_type;

private:
    hashtable ht;

public:
    size_type size()    { return ht.element_size(); }

};

#endif //MYTINYSTL_UNORDERED_MAP_H
