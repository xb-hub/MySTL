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
    using hashtable = mystl::hashtable<std::pair<Key, T>, HashFcn, KeyEqual, Alloc>;
    using data_type = std::pair<Key, T>;
    using value_type = T;
    using key_type = Key;
    using hasher = HashFcn;
    using equals = KeyEqual;

    using iterator = typename hashtable::iterator;
    using pointer = typename hashtable::pointer;
    using reference = typename hashtable::reference;
    using size_type = typename hashtable::size_type;

private:
    hashtable ht;

public:
    size_type size()    { return ht.element_size(); }

};

#endif //MYTINYSTL_UNORDERED_MAP_H
