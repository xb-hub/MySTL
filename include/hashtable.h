#ifndef MYTINYSTL_HASHTABLE_H
#define MYTINYSTL_HASHTABLE_H

#include "alloc.h"
#include "vector.h"
#include "iterator.h"
#include "function.h"
#include "stddef.h"

namespace mystl
{

template<typename T>
struct hashtable_node
{
    hashtable_node* next;
    T value;

    hashtable_node() :
        next(nullptr),
        value(0)
    {}

    hashtable_node(const T& val) :
        next(nullptr),
        value(val)
    {}

    ~hashtable_node()
    {
        free(next);
    }
};

template<typename T, bool>
struct hashtable_value_traits_imp
{
    typedef T value_type;
    typedef T key_type;
    typedef T data_type;

    template<typename Ty>
    static const key_type& getKey(const Ty& data)    { return data; }

    template<typename Ty>
    static const data_type& getValue(const Ty& data) { return data; }
};

template<typename T>
struct hashtable_value_traits_imp<T, true>
{
    typedef typename std::remove_cv<typename T::first_type>::type   key_type;
    typedef typename T::second_type                                 value_type;
    typedef T                                                       data_type;

    template<typename Ty>
    static const key_type& getKey(const Ty& data)    { return data.first; }

    template<typename Ty>
    static const data_type& getValue(const Ty& data) { return data; }
};

template<typename T>
struct hashtable_value_traits
{
    static constexpr bool is_map = mystl::is_pair<T>::value;

    typedef hashtable_value_traits_imp<T, is_map>   value_traits;

    typedef typename value_traits::value_type value_type;
    typedef typename value_traits::data_type data_type;
    typedef typename value_traits::key_type key_type;

    typedef data_type*  pointer;
    typedef data_type&  reference;
    typedef const data_type* const_pointer;
    typedef const data_type& const_reference;

    typedef hashtable_node<T>   node_type;
    typedef hashtable_node<T>*  node_pointer;

    template<class Ty>
    static const key_type& getKey(const Ty& data)   { return value_traits::getKey(data); }

    template<class Ty>
    static const data_type& getValue(const Ty& data)    { return value_traits::getValue(data); }
};

template<typename T, typename HashFcn, typename KeyEqual, typename Alloc>
class hashtable;

template<typename T, typename HashFcn, typename KeyEqual, typename Alloc>
class hashtable_iterator
{
public:
    typedef hashtable_node<T>   node_type;
    typedef hashtable_node<T>*  node_pointer;

    typedef hashtable<T, HashFcn, KeyEqual, Alloc>  hashtable;
    typedef hashtable*                              hashtable_pointer;

    typedef hashtable_iterator<T, HashFcn, KeyEqual, Alloc>         iterator;

    typedef mystl::forward_iterator_tag iterator_category;
    typedef hashtable_value_traits<T>   value_traits;
    typedef typename value_traits::value_type value_type;
    typedef typename value_traits::data_type  data_type;
    typedef typename value_traits::key_type   key_type;
    typedef size_t                            size_type;
    typedef ptrdiff_t                         difference_type;
    typedef data_type*                        pointer;
    typedef data_type&                        reference;

public:
    hashtable_iterator(node_pointer n, hashtable_pointer h)
    {
        cur = n;
        ht = h;
    }
    hashtable_iterator(const iterator& rhs)
    {
        cur = rhs.cur;
        ht = rhs.ht;
    }
    iterator operator=(const iterator& rhs) const
    {
        if(this != &rhs)
        {
            cur = rhs.cur;
            ht = rhs.ht;
        }
        return *this;
    }
    ~hashtable_iterator() {}

public:
    bool operator==(const iterator& rhs)    { return cur == rhs.cur; }
    bool operator!=(const iterator& rhs)    { return cur != rhs.cur; }

    reference operator*() const  { return cur->value; }
    pointer operator->()    { return &(operator*()); }

    iterator& operator++()
    {
        assert(cur != nullptr);
        const node_pointer old = cur;
        cur = cur->next;
        if(cur == nullptr)
        {
            size_type index = ht->bkt_num(old->value);
            while(!cur && ++index < ht->bucket_size()) cur = ht->buckets[index];
        }
        return *this;
    }

    iterator operator++(int)
    {
        iterator tmp = *this;
        ++*this;
        return tmp;
    }

private:
    node_pointer cur;
    hashtable_pointer ht;
};

template<typename T>
class hashtable_const_iterator
{
private:
    /* data */
public:
    hashtable_const_iterator(/* args */);
    ~hashtable_const_iterator();
};

static const int ht_prime_nums = 99;
static constexpr size_t ht_prime_list[] = {
        101ull, 173ull, 263ull, 397ull, 599ull, 907ull, 1361ull, 2053ull, 3083ull,
        4637ull, 6959ull, 10453ull, 15683ull, 23531ull, 35311ull, 52967ull, 79451ull,
        119179ull, 178781ull, 268189ull, 402299ull, 603457ull, 905189ull, 1357787ull,
        2036687ull, 3055043ull, 4582577ull, 6873871ull, 10310819ull, 15466229ull,
        23199347ull, 34799021ull, 52198537ull, 78297827ull, 117446801ull, 176170229ull,
        264255353ull, 396383041ull, 594574583ull, 891861923ull, 1337792887ull,
        2006689337ull, 3010034021ull, 4515051137ull, 6772576709ull, 10158865069ull,
        15238297621ull, 22857446471ull, 34286169707ull, 51429254599ull, 77143881917ull,
        115715822899ull, 173573734363ull, 260360601547ull, 390540902329ull,
        585811353559ull, 878717030339ull, 1318075545511ull, 1977113318311ull,
        2965669977497ull, 4448504966249ull, 6672757449409ull, 10009136174239ull,
        15013704261371ull, 22520556392057ull, 33780834588157ull, 50671251882247ull,
        76006877823377ull, 114010316735089ull, 171015475102649ull, 256523212653977ull,
        384784818980971ull, 577177228471507ull, 865765842707309ull, 1298648764060979ull,
        1947973146091477ull, 2921959719137273ull, 4382939578705967ull, 6574409368058969ull,
        9861614052088471ull, 14792421078132871ull, 22188631617199337ull, 33282947425799017ull,
        49924421138698549ull, 74886631708047827ull, 112329947562071807ull, 168494921343107851ull,
        252742382014661767ull, 379113573021992729ull, 568670359532989111ull, 853005539299483657ull,
        1279508308949225477ull, 1919262463423838231ull, 2878893695135757317ull, 4318340542703636011ull,
        6477510814055453699ull, 9716266221083181299ull, 14574399331624771603ull, 18446744073709551557ull
};

inline size_t next_prime(size_t n)
{
    const size_t* first = ht_prime_list;
    const size_t* last = ht_prime_list + ht_prime_nums;
    const size_t* pos = mystl::lower_bound(first, last, n);
    return pos == last ? *(last - 1) : *pos;
}

template<typename T, typename HashFcn, typename KeyEqual, typename Alloc = alloc>
class hashtable
{
friend class mystl::hashtable_iterator<T, HashFcn, KeyEqual, Alloc>;

public:
    typedef hashtable_node<T> node;
    typedef hashtable_value_traits<T>   value_traits;
    typedef typename value_traits::value_type    value_type;
    typedef typename value_traits::data_type     data_type;     // T
    typedef typename value_traits::key_type      key_type;
    typedef typename value_traits::pointer       pointer;
    typedef typename value_traits::reference     reference;
    typedef typename value_traits::const_pointer const_pointer;
    typedef typename value_traits::const_reference const_reference;
    typedef typename value_traits::node_pointer  node_pointer;
    typedef typename value_traits::node_type     node_type;

    typedef size_t size_type;
    typedef HashFcn   hasher;
    typedef KeyEqual key_equal;
    typedef hashtable_iterator<T, HashFcn, KeyEqual, Alloc> iterator;

    typedef simple_alloc<data_type, Alloc>              data_allocator;
    typedef simple_alloc<node_type, Alloc>              node_allocator;

    typedef mystl::vector<node_pointer>                 bucket_type;

public:
    hashtable(size_type n, const hasher& hf, const key_equal& eql);
    ~hashtable() { clear(); }

public:
    size_type element_size() const  { return num_elements; }
    size_type bucket_size() const  { return buckets_size; }

    std::pair<iterator, bool> insert_unique(const data_type& data);
    size_type new_size(size_type n) const   { return next_prime(n); }

    size_type bkt_num(const data_type& data) const  { return bkt_num_key(value_traits::getKey(data)); }
    size_type bkt_num(const data_type& data, size_type n) const { return bkt_num_key(value_traits::getKey(data), n); }

    iterator begin() noexcept
    {
        for(size_type i = 0; i < bucket_size(); i++)
            if(buckets[i] != nullptr)
                return iterator(buckets[i], this);
        return iterator(nullptr, this);
    }

    iterator end() noexcept { return iterator(nullptr, this); }

private:
    size_type bkt_num_key(const key_type& key, size_type n)  const { return hash(key) % n; }
    size_type bkt_num_key(const key_type& key) const    { return bkt_num_key(key, bucket_size()); }

    node_pointer new_node(const data_type& data);
    void delete_node(node_pointer p);
    void init(size_type n);
    void clear();

    void resize(size_type n);
    std::pair<iterator, bool> insert_unique_noresize(const data_type& data);

private:
    hasher hash;
    key_equal equals;
    bucket_type buckets;
    size_type buckets_size;
    size_type num_elements;
};

template<typename T, typename HashFcn, typename KeyEqual, typename Alloc>
hashtable<T, HashFcn, KeyEqual, Alloc>::hashtable(
        size_type n, const hasher &hf, const key_equal &eql) :
        hash(hf),
        equals(eql),
        num_elements(0)
{
    init(n);
}

template<typename T, typename HashFcn, typename KeyEqual, typename Alloc>
void hashtable<T, HashFcn, KeyEqual, Alloc>::init(size_type n)
{
    const size_type n_buckets = next_prime(n);
    buckets.reserve(n_buckets);
    buckets.insert(buckets.begin(), n_buckets, nullptr);
    buckets_size = n;
}

template<typename T, typename HashFcn, typename KeyEqual, typename Alloc>
typename hashtable<T, HashFcn, KeyEqual, Alloc>::node_pointer hashtable<T, HashFcn, KeyEqual, Alloc>::new_node(
        const data_type &data)
{
    node_pointer node = node_allocator::allocate();
    try
    {
        construct(node, data);
    }
    catch(...)
    {
        throw "allocate failed!";
        node_allocator::deallocate(node);
    }
    return node;
}

template<typename T, typename HashFcn, typename KeyEqual, typename Alloc>
void hashtable<T, HashFcn, KeyEqual, Alloc>::delete_node(node_pointer node)
{
    mystl::destory(&node->value);
    node_allocator::deallocate(node);
}

template<typename T, typename HashFcn, typename KeyEqual, typename Alloc>
void hashtable<T, HashFcn, KeyEqual, Alloc>::clear()
{
    if(bucket_size() != 0)
    {
        for(size_type i = 0; i < bucket_size(); ++i)
        {
            node_pointer cur = buckets[i];
            while(cur != nullptr)
            {
                node_pointer next = cur->next;
                delete_node(cur);
                cur = next;
            }
            buckets[i] = nullptr;
        }
    }
    buckets_size = 0;
    num_elements = 0;
}

template<typename T, typename HashFcn, typename KeyEqual, typename Alloc>
void hashtable<T, HashFcn, KeyEqual, Alloc>::resize(size_type n)
{
    size_type old_n = bucket_size();
    size_type new_n = new_size(n);
    if(new_n > old_n)
    {
        bucket_type new_buckets(new_n);
        for(size_type i = 0; i < old_n; i++)
        {
            node_pointer node = buckets[i];
            while(node)
            {
                size_type index = bkt_num(node->value, n);
                buckets[i] = node->next;
                node->next = new_buckets[index];
                new_buckets[index] = node;
                node = buckets[i];
            }
        }
        buckets.swap(new_buckets);
        buckets_size = new_n;
    }
}

template<typename T, typename HashFcn, typename KeyEqual, typename Alloc>
std::pair<typename hashtable<T, HashFcn, KeyEqual, Alloc>::iterator, bool> hashtable<T, HashFcn, KeyEqual, Alloc>::insert_unique_noresize(
        const data_type &data)
{
    size_type index = bkt_num(data);
    node_pointer cur = buckets[index];
    node_pointer tmp = new_node(data);
    if(cur == nullptr)
    {
        buckets[index] = tmp;
        ++num_elements;
        return std::make_pair<iterator, bool>(iterator(tmp, this), true);
    }
    while(cur != nullptr)
    {
        if(equals(value_traits::getKey(cur->value), value_traits::getKey(data)))
            return std::make_pair<iterator, bool>(iterator(cur, this), false);
        cur = cur->next;
    }
    tmp->next = buckets[index];
    buckets[index] = tmp;
    ++num_elements;
    return std::make_pair<iterator, bool>(iterator(tmp, this), true);
}

template<typename T, typename HashFcn, typename KeyEqual, typename Alloc>
std::pair<typename hashtable<T, HashFcn, KeyEqual, Alloc>::iterator, bool> hashtable<T, HashFcn, KeyEqual, Alloc>::insert_unique(
        const data_type &data)
{
    resize(num_elements + 1);
    return insert_unique_noresize(data);
}

} // namespace mystl


#endif