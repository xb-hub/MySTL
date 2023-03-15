#ifndef MYTINYSTL_RBTREE_H
#define MYTINYSTL_RBTREE_H
//#define TREE_DEBUG

#include "iterator.h"
#include "alloc.h"
#include "construct.h"
#include "type_traits.h"
#include "function.h"

namespace mystl
{

template <class T> struct rb_tree_node_base;
template <class T> struct rb_tree_node;

template <class T> struct rb_tree_iterator;
template <class T> struct rb_tree_const_iterator;

// set
template<typename T, bool>
struct rb_tree_value_traits_imp
{
    using value_type = T;
    using key_type = T;
    using data_type = T;

    template<typename Ty>
    static const key_type& getKey(const Ty& data)    { return data; }

    template<typename Ty>
    static const data_type& getValue(const Ty& data) { return data; }
};

// map
template<typename T>
struct rb_tree_value_traits_imp<T, true>
{
    using key_type = typename std::remove_cv<typename T::first_type>::type;
    using value_type = typename T::second_type;
    using data_type = T;
    
    template<typename Ty>
    static const key_type& getKey(const Ty& data)    { return data.first; }

    template<typename Ty>
    static const data_type& getValue(const Ty& data) { return data; }
};

template<typename T>
struct rb_tree_value_traits
{
    static constexpr bool is_map = mystl::is_pair<T>::value;

    using value_traits_type = rb_tree_value_traits_imp<T, is_map>;

    using key_type = typename value_traits_type::key_type;
    using value_type = typename value_traits_type::value_type;
    using data_type = typename value_traits_type::data_type;

    template<typename Ty>
    static const key_type& getKey(const Ty& data)    { return value_traits_type::getKey(data); }

    template<typename Ty>
    static const data_type& getValue(const Ty& data) { return value_traits_type::getValue(data); }
};

enum class rb_tree_color_type : bool
{ 
    RED = false,
    BLACK = true
};

template<typename T>
struct rb_tree_node_base
{
    using color_type = rb_tree_color_type;
    using base_ptr = rb_tree_node_base<T>*;
    using node_ptr = rb_tree_node<T>*;

    base_ptr parent;
    base_ptr lchild;
    base_ptr rchild;
    color_type color;
};

template<typename T>
struct rb_tree_node : public rb_tree_node_base<T>
{
    using  base_ptr = rb_tree_node_base<T>*;
    using node_ptr = rb_tree_node<T>*;

    T data;

    rb_tree_node(const T& rhs)  :
            data(rhs)
    {}

    base_ptr getBasePtr() const { return static_cast<base_ptr>(this); }
    node_ptr getNodePtr() const { return this; }
};

template<typename T>
struct rb_tree_traits
{
    using key_type = typename rb_tree_value_traits<T>::key_type;
    using value_type = typename rb_tree_value_traits<T>::value_type;
    using data_type = typename rb_tree_value_traits<T>::data_type;
    using pointer = data_type*;
    using reference = data_type&;
    using const_pointer = const data_type*;
    using const_reference = const data_type&;
    using base_type = rb_tree_node_base<T>;
    using node_type = rb_tree_node<T>;
    using base_ptr = base_type*;
    using node_ptr = node_type*;
};

template<typename T>
struct rb_tree_base_iterator : public mystl::iterator<mystl::bidirectional_iterator_tag, T>
{
    using base_ptr = typename rb_tree_traits<T>::base_ptr ;

    base_ptr node;

    rb_tree_base_iterator() : 
            node(nullptr)
    {}

    /**  最左节点(最小值) (header,end()) 
     *     left(begin())<-----O----->right(最右节点(最大值))
     *                       | |(互为父节点)
     *                        O(root)
     */
      
    void increment()
    {
        #ifdef TREE_DEBUG
            std::cout << "inc" << std::endl;
        #endif
        if(node->rchild != nullptr)
        {
            node = node->rchild;
            while(node->lchild != nullptr)
            {
                node = node->lchild;
            }
            
        }
        else
        {
            base_ptr parent = node->parent;
            while(parent->rchild == node)
            {
                node = parent;
                parent = node->parent;
            }
            /**
             * @brief node指向根节点，并且没有右孩子，导致header->rchild == node，
             * 最终node=header，parent=root，结果为node，不需要执行node = parent。
             *     ------ O ------
             *     |     | |     |
             *     |      O <-----
             *     |     /
             *     ---> O 
             */
            if(node->rchild != parent)  node = parent;
        }
    }

    void decrement()
    {
        // 当node指向header，--操作后node指向最右边（最大值）处
        if(node->color == rb_tree_color_type::RED && node->parent->parent == node)   node = node->rchild;
        if(node->lchild != nullptr)
        {
            node = node->lchild;
            while(node->rchild != nullptr)
            {
                node = node->rchild;
            }
        }
        else
        {
            base_ptr parent = node->parent;
            while (parent->lchild == node)
            {
                node = parent;
                parent = node->parent;
            }
            /**
             * @brief node指向根节点，并且没有左孩子孩子，node为begin(),无法在后退。
             *     ------ O ------
             *     |     | |     |
             *     -----> O      |
             *             \     |
             *              O <---
             */
            node = parent;
        }
    }

    bool operator==(const rb_tree_base_iterator& rhs) { return node == rhs.node; }
    bool operator!=(const rb_tree_base_iterator& rhs) { return node != rhs.node; }
};

template<typename T>
struct rb_tree_const_iterator : public rb_tree_base_iterator<T>
{
    using value_type = typename rb_tree_traits<T>::value_type;
    using pointer = typename rb_tree_traits<T>::pointer;
    using reference = typename rb_tree_traits<T>::reference;
    using base_ptr = typename rb_tree_traits<T>::base_ptr;
    using node_ptr = typename rb_tree_traits<T>::node_ptr;

    using iterator = rb_tree_iterator<T>;
    using const_iterator = rb_tree_const_iterator<T>;
    using self = const_iterator;

    rb_tree_const_iterator() {};
    rb_tree_const_iterator(base_ptr ptr) { this->node = ptr; }
    rb_tree_const_iterator(node_ptr ptr) { this->node = ptr; }
    rb_tree_const_iterator(const const_iterator& rhs) { this->node = rhs.node; }
    rb_tree_const_iterator(const iterator& rhs) { this->node = rhs.node; }

    reference operator*() { return reinterpret_cast<node_ptr>(this->node)->data; }
    pointer operator->() { return &(operator*()); }

    self& operator=(const rb_tree_const_iterator<T>& rhs) 
    {
        this->node = rhs.node;
        return *this;
    }

    self& operator--()
    {
        this->decrement();
        return *this;
    }
    self operator--(int)
    {
        self tmp = *this;
        this->decrement();
        return tmp;
    }
    self operator++()
    {
        this->increment();
        return *this;
    }
    self operator++(int)
    {
        self tmp = *this;
        this->increment();
        return tmp;
    }

    // bool operator==(const const_iterator& rhs) const { return rhs.node == this->node; }
    // bool operator!=(const const_iterator& rhs) const { return rhs.node != this->node; }
};

template<typename T>
struct rb_tree_iterator : public rb_tree_base_iterator<T>
{
    using value_type = typename rb_tree_traits<T>::value_type;
    using pointer = typename rb_tree_traits<T>::pointer;
    using reference = typename rb_tree_traits<T>::reference;
    using base_ptr = typename rb_tree_traits<T>::base_ptr;
    using node_ptr = typename rb_tree_traits<T>::node_ptr;

    using iterator = rb_tree_iterator<T>;
    using const_iterator = rb_tree_const_iterator<T>;
    using self = iterator;

    rb_tree_iterator() {};
    rb_tree_iterator(base_ptr ptr) { this->node = ptr; }
    rb_tree_iterator(node_ptr ptr) { this->node = ptr; }
    rb_tree_iterator(const iterator& rhs) { this->node = rhs.node; }
    rb_tree_iterator(const_iterator& rhs) { this->node = rhs.node; }

    reference operator*() { return reinterpret_cast<node_ptr>(this->node)->data; }
    pointer operator->() { return &(operator*()); }

    self& operator=(const rb_tree_const_iterator<T>& rhs) 
    {
        this->node = rhs.node;
        return *this;
    }

    self& operator--()
    {
        this->decrement();
        return *this;
    }
    self operator--(int)
    {
        self tmp = *this;
        this->decrement();
        return tmp;
    }
    self operator++()
    {
        this->increment();
        return *this;
    }
    self operator++(int)
    {
        self tmp = *this;
        this->increment();
        return tmp;
    }

    // bool operator==(const iterator& rhs) const { return rhs.node == this->node; }
    // bool operator!=(const iterator& rhs) const { return rhs.node != this->node; }
};

template<typename NodePtr>
inline void rb_tree_rotate_left(NodePtr x, NodePtr& root)
{
    auto y = x->rchild;
    x->rchild = y->lchild;
    if(y->lchild != nullptr)    y->lchild->parent = x;
    y->parent = x->parent;

    if(x == root)   root = y;
    else if(x == x->parent->lchild) x->parent->lchild = y;
    else    x->parent->rchild = y;

    y->lchild = x;
    x->parent = y;
}

template<typename NodePtr>
inline void rb_tree_rotate_right(NodePtr x, NodePtr& root)
{
    auto y = x->lchild;
    x->lchild = y->rchild;
    if(y->rchild != nullptr)    y->rchild->parent = x;   
    y->parent = x->parent;
    if(x == root)   root = y;
    else if(x == x->parent->rchild)  x->parent->rchild = y;
    else    x->parent->lchild = y;

    y->rchild = x;
    x->parent = y;
}

template <typename NodePtr>
inline void rb_tree_rebalance(NodePtr x, NodePtr& root)
{
    x->color = rb_tree_color_type::RED;
    while(x != root && x->parent->color == rb_tree_color_type::RED)
    {
        if (x->parent == x->parent->parent->lchild)
        {
            
            auto uncle = x->parent->parent->rchild;
            if(uncle != nullptr && uncle->color == rb_tree_color_type::RED)
            {
                #ifdef TREE_DEBUG
                    std::cout << "case 1 : parent lchild, uncle RED" << std::endl;
                #endif
                x->parent->color = rb_tree_color_type::BLACK;
                uncle->color = rb_tree_color_type::BLACK;
                x->parent->parent->color = rb_tree_color_type::RED;
                x = x->parent->parent;
            }
            else
            {
                #ifdef TREE_DEBUG
                    std::cout << "case 2 : parent lchild, uncle BLACK" << std::endl;
                #endif
                if(x == x->parent->rchild)
                {
                    x = x->parent;
                    rb_tree_rotate_left(x, root);
                }
                x->parent->color = rb_tree_color_type::BLACK;
                x->parent->parent->color = rb_tree_color_type::RED;
                rb_tree_rotate_right(x->parent->parent, root);
                break;
            }
        }
        else
        {
            auto uncle = x->parent->parent->lchild;
            if(uncle != nullptr && uncle->color == rb_tree_color_type::RED)
            {
                #ifdef TREE_DEBUG
                    std::cout << "case 3 : parent rchild, uncle RED" << std::endl;
                #endif
                x->parent->color = rb_tree_color_type::BLACK;
                uncle->color = rb_tree_color_type::BLACK;
                x->parent->parent->color = rb_tree_color_type::RED;
                x = x->parent->parent;
            }
            else
            {
                #ifdef TREE_DEBUG
                    std::cout << "case 4 : parent rchild, uncle BLACK" << std::endl;
                #endif
                if(x == x->parent->lchild)
                {
                    x = x->parent;
                    rb_tree_rotate_right(x, root);
                }
                x->parent->color = rb_tree_color_type::BLACK;
                x->parent->parent->color = rb_tree_color_type::RED;
                rb_tree_rotate_left(x->parent->parent, root);
                break;
            }
        }
    }
    root->color = rb_tree_color_type::BLACK;
}

template<typename T, typename Compare = mystl::less<typename rb_tree_traits<T>::key_type>, typename Alloc = alloc>
class rb_tree
{
public:
    using tree_traits =  rb_tree_traits<T>;
    using value_traits =  rb_tree_value_traits<T>;;
    using base_type =  typename tree_traits::base_type;
    using base_ptr =  typename tree_traits::base_ptr;
    using node_type =  typename tree_traits::node_type;
    using node_ptr =  typename tree_traits::node_ptr;
    using key_type =  typename tree_traits::key_type;
    using value_type =  typename tree_traits::value_type;
    using data_type =  typename tree_traits::data_type;
    using key_compare =  Compare;
    using pointer =  typename tree_traits::pointer;
    using reference =  typename tree_traits::reference;
    using const_pointer =  typename tree_traits::const_pointer;
    using const_reference =  typename tree_traits::const_reference;
    using size_type =  size_t;
    using difference_type =  ptrdiff_t;
    using iterator =  rb_tree_iterator<T>;
    using const_iterator =  rb_tree_const_iterator<T>;
    using allocator_type =  simple_alloc<T, Alloc>;
    using data_allocator =  simple_alloc<T, Alloc>;
    using base_allocator =  simple_alloc<base_type, Alloc>;
    using node_allocator =  simple_alloc<node_type, Alloc>;

    
    // using reverse_iterator = mystl::reverse_iterator<iterator>;
    // using const_reverse_iterator = mystl::reverse_iterator<const_iterator>;

public:
    rb_tree(const Compare& comp = Compare()) :
            node_count(0),
            header(nullptr),
            key_comp(comp)
    { init(); }
    ~rb_tree()
    {
        
    }

private:
    base_ptr& root() const { return header->parent; }
    base_ptr& left_most() const { return header->lchild; }
    base_ptr& right_most() const { return header->rchild; }

public:
    node_ptr m_begin() const { return static_cast<node_ptr>(left_most()); }
    node_ptr m_end() const { return static_cast<node_ptr>(header); }
    iterator begin() const { return left_most(); }
    iterator end() const { return header; }
    const_iterator cbegin() const { return begin(); }
    const_iterator cend() const { return end(); }

    bool empty() const { return node_count == 0; }
    size_type size() const { return node_count; }

private:
    node_ptr creat_node(const data_type& data)
    {
        node_ptr node = node_allocator::allocate();
        try
        {
            mystl::construct(node, data);
        }
        catch(...)
        {
            node_allocator::deallocate(node);
            throw "allocate failed!";
        }
        return node;
    }

    node_ptr clone_node(node_ptr node)
    {
        node_ptr new_node = creat_node(node->data);
        new_node->color = node->color;
        new_node->lchild = nullptr;
        new_node->rchild = nullptr;
        new_node->parent = nullptr;
        return new_node;
    }

    void destory(node_ptr node)
    {
        mystl::destory(&node->data);
        node_allocator::deallocate(node);
    }

public:

    void clear()
    {
        __erase(m_begin());
    }

private:
    void init()
    {
        header = node_allocator::allocate();;
        header->color = rb_tree_color_type::RED;
        root() = nullptr;
        header->lchild = header;
        header->rchild = header;
    }

    std::pair<base_ptr, base_ptr> get_insert_unique_pos(const key_type& k);
    iterator __insert(base_ptr x, base_ptr y, node_ptr z);
    node_ptr __copy(node_ptr x, node_ptr p);
    void __erase(node_ptr x);

public:
    iterator insert_equal(const data_type& value);
    std::pair<iterator, bool> insert_unique(const data_type& value);

private:
    key_type base_key(base_ptr x)   { return value_traits::getKey(static_cast<node_ptr>(x)->data); }
    key_type node_key(node_ptr x)   { return value_traits::getKey(x->data); }

private:
    size_type node_count;
    base_ptr header;
    Compare key_comp;
};

template<typename T, typename Compare, typename Alloc>
std::pair<typename rb_tree<T, Compare, Alloc>::base_ptr, typename rb_tree<T, Compare, Alloc>::base_ptr> rb_tree<T, Compare, Alloc>::get_insert_unique_pos(const key_type& k)
{
    using Res = std::pair<base_ptr, base_ptr>;
    base_ptr x = root();
    base_ptr y = header;
    bool comp = true;
#ifdef TREE_DEBUG
    std::cout << x << "---" << y << std::endl;
#endif
    while(x != nullptr)
    {
        y = x;
        comp = key_comp(k, base_key(x));
#ifdef TREE_DEBUG
        std::cout << "key:   " << k << "---" << base_key(x) << "---" << comp << std::endl;
#endif
        x = comp ? x->lchild : x->rchild;
    }
    iterator j = iterator(y);
    if(comp)
    {
        if(j == begin())    return Res(x, y);
        else    --j;
    }
    if(key_comp(base_key(j.node), k))   return Res(x, y);
    return Res(nullptr, nullptr);
}

template<typename T, typename Compare, typename Alloc>
typename rb_tree<T, Compare, Alloc>::iterator rb_tree<T, Compare, Alloc>::__insert(base_ptr x, base_ptr y, node_ptr z)
{
    if(x != 0 || y == header || key_comp(node_key(z), base_key(y)))
    {
        y->lchild = z;
        if(y == header)
        {
#ifdef TREE_DEBUG
            std::cout << "set root: " << z << std::endl;
#endif
            root() = z;
            right_most() = z;
        }
        else if(y == left_most())   left_most() = z;
    }
    else
    {
        y->rchild = z;
        if(y == right_most())    right_most() = z;
    }
    z->parent = y;
    rb_tree_rebalance(static_cast<base_ptr>(z), root());
    ++node_count;
    return iterator(z);
}

template<typename T, typename Compare, typename Alloc>
typename rb_tree<T, Compare, Alloc>::iterator rb_tree<T, Compare, Alloc>::insert_equal(const data_type& value)
{
    using Res = std::pair<iterator, bool>;
    base_ptr x = root();
    base_ptr y = header;
    while(x != nullptr)
    {
        y = x;
        x = (key_comp(value_traits::getKey(value), base_key(x)) ? x->lchild : x->rchild);
    }
    node_ptr z = creat_node(value);
    return __insert(x, y, z);
}

template<typename T, typename Compare, typename Alloc>
std::pair<typename rb_tree<T, Compare, Alloc>::iterator, bool> rb_tree<T, Compare, Alloc>::insert_unique(const data_type& value)
{
    using Res = std::pair<iterator, bool>;
    node_ptr z = creat_node(value);
    auto res = get_insert_unique_pos(value_traits::getKey(value));
    if(res.second != nullptr)   return Res(__insert(res.first, res.second, z), true);
    destory(z);
    return Res(iterator(res.first), false);
}

template<typename T, typename Compare, typename Alloc>
void rb_tree<T, Compare, Alloc>::__erase(node_ptr x)
{
    while(x != nullptr)
    {
        __erase(x->rchild);
        node_ptr left = x->lchild;
        destory(x);
        x = left;
    }
}


} // namespace mystl
#endif
