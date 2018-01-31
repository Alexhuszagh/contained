//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \synopsis
 // TODO: add list_facet
 *      template <class T, class Alloc = allocator<T> >
 *      class list
 *      {
 *      public:
 *          typedef T value_type;
 *          typedef Alloc allocator_type;
 *          typedef list_facet<T, typename std::allocator_traits<Alloc>::void_pointer> facet_type;
 *          typedef typename allocator_type::reference reference;
 *          typedef typename allocator_type::const_reference const_reference;
 *          typedef typename allocator_type::pointer pointer;
 *          typedef typename allocator_type::const_pointer const_pointer;
 *          typedef implementation-defined iterator;
 *          typedef implementation-defined const_iterator;
 *          typedef implementation-defined size_type;
 *          typedef implementation-defined difference_type;
 *          typedef reverse_iterator<iterator> reverse_iterator;
 *          typedef reverse_iterator<const_iterator> const_reverse_iterator;
 *          list() noexcept;
 *          explicit list(const allocator_type& a);
 *          explicit list(size_type n);
 *          explicit list(size_type n, const allocator_type& a);
 *          list(size_type n, const value_type& value);
 *          list(size_type n, const value_type& value, const allocator_type& a);
 *          template <class Iter>
 *              list(Iter first, Iter last);
 *          template <class Iter>
 *              list(Iter first, Iter last, const allocator_type& a);
 *          list(const list& x);
 *          list(const list&, const allocator_type& a);
 *          list(list&& x) noexcept;
 *          list(list&&, const allocator_type& a);
 *          list(initializer_list<value_type>);
 *          list(initializer_list<value_type>, const allocator_type& a);
 *          ~list();
 *          list& operator=(const list& x);
 *          list& operator=(list&& x)
 *              noexcept(
 *                   allocator_type::propagate_on_container_move_assignment::value &&
 *                   is_nothrow_move_assignable<allocator_type>::value);
 *          list& operator=(initializer_list<value_type>);
 *          template <class Iter>
 *              void assign(Iter first, Iter last);
 *          void assign(size_type n, const value_type& t);
 *          void assign(initializer_list<value_type>);
 *          allocator_type get_allocator() const noexcept;
 *          iterator begin() noexcept;
 *          const_iterator begin() const noexcept;
 *          iterator end() noexcept;
 *          const_iterator end() const noexcept;
 *          reverse_iterator rbegin() noexcept;
 *          const_reverse_iterator rbegin() const noexcept;
 *          reverse_iterator rend() noexcept;
 *          const_reverse_iterator rend() const noexcept;
 *          const_iterator cbegin() const noexcept;
 *          const_iterator cend() const noexcept;
 *          const_reverse_iterator crbegin() const noexcept;
 *          const_reverse_iterator crend() const noexcept;
 *          reference front();
 *          const_reference front() const;
 *          reference back();
 *          const_reference back() const;
 *          bool empty() const noexcept;
 *          size_type size() const noexcept;
 *          size_type max_size() const noexcept;
 *          template <class... Args>
 *              reference emplace_front(Args&&... args);
 *          void pop_front();
 *          template <class... Args>
 *              reference emplace_back(Args&&... args);
 *          void pop_back();
 *          void push_front(const value_type& x);
 *          void push_front(value_type&& x);
 *          void push_back(const value_type& x);
 *          void push_back(value_type&& x);
 *          template <class... Args>
 *              iterator emplace(const_iterator position, Args&&... args);
 *          iterator insert(const_iterator position, const value_type& x);
 *          iterator insert(const_iterator position, value_type&& x);
 *          iterator insert(const_iterator position, size_type n, const value_type& x);
 *          template <class Iter>
 *              iterator insert(const_iterator position, Iter first, Iter last);
 *          iterator insert(const_iterator position, initializer_list<value_type> il);
 *          iterator erase(const_iterator position);
 *          iterator erase(const_iterator position, const_iterator last);
 *          void resize(size_type sz);
 *          void resize(size_type sz, const value_type& c);
 *          void swap(list&) noexcept;
 *          void clear() noexcept;
 *          void splice(const_iterator position, list& x);
 *          void splice(const_iterator position, list&& x);
 *          void splice(const_iterator position, list& x, const_iterator i);
 *          void splice(const_iterator position, list&& x, const_iterator i);
 *          void splice(const_iterator position, list& x, const_iterator first,
 *                                                        const_iterator last);
 *          void splice(const_iterator position, list&& x, const_iterator first,
 *                                                        const_iterator last);
 *          void remove(const value_type& value);
 *          template <class Pred> void remove_if(Pred pred);
 *          void unique();
 *          template <class BinaryPredicate>
 *              void unique(BinaryPredicate binary_pred);
 *          void merge(list& x);
 *          void merge(list&& x);
 *          template <class Compare>
 *              void merge(list& x, Compare comp);
 *          template <class Compare>
 *              void merge(list&& x, Compare comp);
 *          void sort();
 *          template <class Compare>
 *              void sort(Compare comp);
 *          void reverse() noexcept;
 *      };
 *      template <class T, class Alloc>
 *          bool operator==(const list<T,Alloc>& x, const list<T,Alloc>& y);
 *      template <class T, class Alloc>
 *          bool operator< (const list<T,Alloc>& x, const list<T,Alloc>& y);
 *      template <class T, class Alloc>
 *          bool operator!=(const list<T,Alloc>& x, const list<T,Alloc>& y);
 *      template <class T, class Alloc>
 *          bool operator> (const list<T,Alloc>& x, const list<T,Alloc>& y);
 *      template <class T, class Alloc>
 *          bool operator>=(const list<T,Alloc>& x, const list<T,Alloc>& y);
 *      template <class T, class Alloc>
 *          bool operator<=(const list<T,Alloc>& x, const list<T,Alloc>& y);
 *      template <class T, class Alloc>
 *          void swap(list<T,Alloc>& x, list<T,Alloc>& y) noexcept;
 */

#pragma once

#include <algorithm>
#include <cassert>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <limits>
#include <memory>
#include <type_traits>
#include <utility>
#include <contained/detail/allocator_destructor.h>
#include <contained/detail/compressed_pair.h>
#include <contained/detail/swap_allocator.h>
#include <contained/detail/util.h>


namespace contained
{
// FORWARD
// -------

template <typename T, typename VoidPtr>
struct list_node;

template <typename T, typename VoidPtr>
struct list_node_base;

// OBJECTS
// -------

// LIST NODE POINTER TRAITS

template <typename T, typename VoidPtr>
struct list_node_pointer_traits
{
    using node = list_node<T, VoidPtr>;
    using node_pointer = typename std::pointer_traits<VoidPtr>::template rebind<node>;
    using base = list_node_base<T, VoidPtr>;
    using base_pointer = typename std::pointer_traits<VoidPtr>::template rebind<base>;
    using link_pointer = base_pointer;
    using non_link_pointer = typename std::conditional<
        std::is_same<link_pointer, node_pointer>::value,
        base_pointer,
        node_pointer
    >::type;

    static
    link_pointer
    unsafe_link_pointer_cast(link_pointer p)
    {
        return p;
    }

    static
    link_pointer
    unsafe_link_pointer_cast(non_link_pointer p)
    {
        return static_cast<link_pointer>(static_cast<VoidPtr>(p));
    }
};

// LIST NODE BASE

template <typename T, typename VoidPtr>
struct list_node_base
{
    using node_traits = list_node_pointer_traits<T, VoidPtr>;
    using node_pointer = typename node_traits::node_pointer;
    using base_pointer = typename node_traits::base_pointer;
    using link_pointer = typename node_traits::link_pointer;

    link_pointer prev_;
    link_pointer next_;

    list_node_base():
        prev_(node_traits::unsafe_link_pointer_cast(self())),
        next_(node_traits::unsafe_link_pointer_cast(self()))
    {}

    base_pointer
    self()
    {
        return std::pointer_traits<base_pointer>::pointer_to(*this);
    }

    node_pointer
    as_node()
    {
        return static_cast<node_pointer>(self());
    }
};

// LIST NODE

template <typename T, typename VoidPtr>
struct list_node: list_node_base<T, VoidPtr>
{
    T value_;

    using base = list_node_base<T, VoidPtr>;
    using link_pointer = typename base::link_pointer;

    link_pointer as_link()
    {
        return static_cast<link_pointer>(base::self());
    }
};

// LIST ITERATOR

template <typename T, typename VoidPtr>
class list_iterator
{
    using node_traits = list_node_pointer_traits<T, VoidPtr>;
    using link_pointer = typename node_traits::link_pointer;

    link_pointer ptr_;

    explicit
    list_iterator(link_pointer p)
    noexcept:
        ptr_(p)
    {}

    template <typename, typename> friend class list_facet;
    template <typename, typename> friend class list;
    template <typename, typename> friend class list_const_iterator;

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using reference = value_type&;
    using pointer = typename std::pointer_traits<VoidPtr>::template rebind<value_type>;
    using difference_type = typename std::pointer_traits<pointer>::difference_type;

    list_iterator()
    noexcept:
        ptr_(nullptr)
    {}

    reference
    operator*()
    const
    {
        return ptr_->as_node()->value_;
    }

    pointer
    operator->()
    const
    {
        return std::pointer_traits<pointer>::pointer_to(ptr_->as_node()->value_);
    }

    list_iterator&
    operator++()
    {
        ptr_ = ptr_->next_;
        return *this;
    }

    list_iterator
    operator++(int)
    {
        list_iterator t(*this);
        ++(*this);
        return t;
    }

    list_iterator&
    operator--()
    {
        ptr_ = ptr_->prev_;
        return *this;
    }

    list_iterator
    operator--(int)
    {
        list_iterator t(*this);
        --(*this);
        return t;
    }

    friend
    bool
    operator==(
        const list_iterator& x,
        const list_iterator& y
    )
    {
        return x.ptr_ == y.ptr_;
    }

    friend
    bool
    operator!=(
        const list_iterator& x,
        const list_iterator& y
    )
    {
        return !(x == y);
    }
};

// LIST CONST ITERATOR

template <typename T, typename VoidPtr>
class list_const_iterator
{
    using node_traits = list_node_pointer_traits<T, VoidPtr>;
    using link_pointer = typename node_traits::link_pointer;

    link_pointer ptr_;

    explicit
    list_const_iterator(link_pointer p)
    noexcept:
        ptr_(p)
    {}

    template <typename, typename> friend class list_facet;
    template <typename, typename> friend class list;

public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using reference = value_type&;
    using pointer = typename std::pointer_traits<VoidPtr>::template rebind<const value_type>;
    using difference_type = typename std::pointer_traits<pointer>::difference_type;

    list_const_iterator()
    noexcept:
        ptr_(nullptr)
    {}

    list_const_iterator(const list_iterator<T, VoidPtr>& p)
    noexcept:
        ptr_(p.ptr_)
    {}

    reference
    operator*()
    const
    {
        return ptr_->as_node()->value_;
    }

    pointer
    operator->()
    const
    {
        return std::pointer_traits<pointer>::pointer_to(ptr_->as_node()->value_);
    }

    list_const_iterator&
    operator++()
    {
        ptr_ = ptr_->next_;
        return *this;
    }

    list_const_iterator
    operator++(int)
    {
        list_const_iterator t(*this);
        ++(*this);
        return t;
    }

    list_const_iterator&
    operator--()
    {
        ptr_ = ptr_->prev_;
        return *this;
    }

    list_const_iterator
    operator--(int)
    {
        list_const_iterator t(*this);
        --(*this);
        return t;
    }

    friend
    bool
    operator==(
        const list_const_iterator& x,
        const list_const_iterator& y
    )
    {
        return x.ptr_ == y.ptr_;
    }

    friend
    bool
    operator!=(
        const list_const_iterator& x,
        const list_const_iterator& y
    )
    {
        return !(x == y);
    }
};

// LIST FACET

template <
    typename T,
    typename VoidPtr = void*
>
class list_facet
{
protected:
    using void_pointer = VoidPtr;
    using node = list_node<T, void_pointer>;
    using node_base = list_node_base<T, void_pointer>;
    using node_pointer = typename std::pointer_traits<void_pointer>::template rebind<node>;
    using node_base_pointer = typename std::pointer_traits<void_pointer>::template rebind<node_base>;
    using node_pointer_traits = list_node_pointer_traits<T, void_pointer>;
    using link_pointer = typename node_pointer_traits::link_pointer;

    link_pointer
    end_as_link()
    const noexcept
    {
        return node_pointer_traits::unsafe_link_pointer_cast(const_cast<node_base&>(end_).self());
    }

    node_base_pointer
    begin_node_base()
    noexcept
    {
        return end_.next_;
    }

    node_base_pointer
    begin_node_base()
    const noexcept
    {
        node_base& r = const_cast<node_base&>(end_);
        return r.next_;
    }

    node_pointer
    begin_node()
    noexcept
    {
        return begin_node_base()->as_node();
    }

    node_pointer
    begin_node()
    const noexcept
    {
        return begin_node_base()->as_node();
    }

    node_base_pointer
    rbegin_node_base()
    noexcept
    {
        return end_.prev_;
    }

    node_base_pointer
    rbegin_node_base()
    const noexcept
    {
        node_base& r = const_cast<node_base&>(end_);
        return r.prev_;
    }

    node_pointer
    rbegin_node()
    noexcept
    {
        return rbegin_node_base()->as_node();
    }

    node_pointer
    rbegin_node()
    const noexcept
    {
        return rbegin_node_base()->as_node();
    }

    // Constructors
    list_facet(const list_facet&) = delete;

    list_facet(list_facet&& x)
    noexcept:
        end_(move(x.end_)),
        size_(move(x.size_))
    {
        x.end_ = node_base();
        x.size_ = 0;
    }

    // Assignment
    list_facet& operator=(const list_facet&) = delete;

    list_facet&
    operator=(list_facet&& x)
    {
        swap(x);
        return *this;
    }

    // Modifiers
    void
    swap(list_facet& x)
    noexcept
    {
        std::swap(size_, x.size_);
        std::swap(end_, x.end_);
        if (size_() == 0) {
            end_.next_ = end_.prev_ = end_as_link();
        } else {
            end_.prev_->next_ = end_.next_->prev_ = end_as_link();
        }
        if (x.size_() == 0) {
            x.end_.next_ = x.end_.prev_ = x.end_as_link();
        } else {
            x.end_.prev_->next_ = x.end_.next_->prev_ = x.end_as_link();
        }
    }

    template <typename, typename> friend class list;

public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::pointer_traits<void_pointer>::template rebind<value_type>;
    using const_pointer = typename std::pointer_traits<void_pointer>::template rebind<const value_type>;
    using difference_type = typename std::pointer_traits<void_pointer>::difference_type;
    using size_type = typename std::make_unsigned<difference_type>::type;
    using iterator = list_iterator<value_type, void_pointer>;
    using const_iterator = list_const_iterator<value_type, void_pointer>;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // Constructors
    list_facet()
    noexcept:
        size_(0)
    {}

    // Element access
    reference
    front()
    {
        assert(!empty() && "list::front() called on empty container.");
        return begin_node()->value_;
    }

    const_reference
    front()
    const
    {
        assert(!empty() && "list::front() called on empty container.");
        return begin_node()->value_;
    }

    reference
    back()
    {
        assert(!empty() && "list::back() called on empty container.");
        return rbegin_node()->value_;
    }

    const_reference
    back()
    const
    {
        assert(!empty() && "list::back() called on empty container.");
        return rbegin_node()->value_;
    }

    // Iterators
    iterator
    begin()
    noexcept
    {
        return iterator(begin_node_base());
    }

    const_iterator
    begin()
    const noexcept
    {
        return const_iterator(begin_node_base());
    }

    const_iterator
    cbegin()
    const noexcept
    {
        return begin();
    }

    iterator
    end()
    noexcept
    {
        return iterator(end_as_link());
    }

    const_iterator
    end()
    const noexcept
    {
        return const_iterator(end_as_link());
    }

    const_iterator
    cend()
    const noexcept
    {
        return end();
    }

    reverse_iterator
    rbegin()
    noexcept
    {
        return reverse_iterator(end());
    }

    const_reverse_iterator
    rbegin()
    const noexcept
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator
    crbegin()
    const noexcept
    {
        return rbegin();
    }

    reverse_iterator
    rend()
    noexcept
    {
        return reverse_iterator(begin());
    }

    const_reverse_iterator
    rend()
    const noexcept
    {
        return const_reverse_iterator(begin());
    }

    const_reverse_iterator
    crend()
    const noexcept
    {
        return rend();
    }

    // Capacity
    bool
    empty()
    const noexcept
    {
        return size() == 0;
    }

    size_type
    size()
    const noexcept
    {
        return size_;
    }

    size_type
    max_size()
    const noexcept
    {
        // guaranteed to be constexpr
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    // Operations
    void
    merge(list_facet&& x)
    {
        merge(x, std::less<value_type>());
    }

    template <typename Compare>
    void
    merge(list_facet&& x, Compare comp)
    {
        merge(x, move(comp));
    }

    void
    merge(list_facet& x)
    {
        merge(x, std::less<value_type>());
    }

    template <typename Compare>
    void
    merge(list_facet& x, Compare comp)
    {
        if (this != &x) {
            merge_list(x, comp);
        }
    }

    void
    splice(const_iterator p, list_facet& x)
    {
        assert(this != &x && "list::splice(iterator, list) called with this == &list");
        if (!x.empty()) {
            link_pointer f = x.begin_node_base();
            link_pointer l = x.rbegin_node_base();
            unlink_nodes(f, l);
            link_nodes(p.ptr_, f, l);
            size_ += x.size();
            x.size_ = 0;
        }
    }

    void
    splice(const_iterator p, list_facet&& x)
    {
        splice(p, x);
    }

    void
    splice(const_iterator p, list_facet&& x, const_iterator i)
    {
        splice(p, x, i);
    }

    void
    splice(
        const_iterator p,
        list_facet&& x,
        const_iterator f,
        const_iterator l
    )
    {
        splice(p, x, f, l);
    }

    void splice(const_iterator p, list_facet& x, const_iterator i)
    {
        if (p.ptr_ != i.ptr_ && p.ptr_ != i.ptr_->next_) {
            link_pointer f = i.ptr_;
            unlink_nodes(f, f);
            link_nodes(p.ptr_, f, f);
            --x.size_;
            ++size_;
        }
    }

    void
    splice(
        const_iterator p,
        list_facet& x,
        const_iterator f,
        const_iterator l
    )
    {
        if (f != l) {
            link_pointer first = f.ptr_;
            --l;
            link_pointer last = l.ptr_;
            if (this != &x) {
                size_type s = distance(f, l) + 1;
                x.size_ -= s;
                size_ += s;
            }
            unlink_nodes(first, last);
            link_nodes(p.ptr_, first, last);
        }
    }

    void
    reverse()
    noexcept
    {
        if (size() > 1) {
            iterator e = end();
            for (iterator i = begin(); i.ptr_ != e.ptr_;) {
                std::swap(i.ptr_->prev_, i.ptr_->next_);
                i.ptr_ = i.ptr_->prev_;
            }
            std::swap(e.ptr_->prev_, e.ptr_->next_);
        }
    }

    void
    sort()
    {
        sort(std::less<value_type>());
    }

    template <typename Compare>
    void
    sort(Compare comp)
    {
        sort_list(begin(), end(), size(), comp);
    }

private:
    node_base end_;
    size_type size_;

    static
    void
    unlink_nodes(link_pointer f, link_pointer l)
    noexcept
    {
        f->prev_->next_ = l->next_;
        l->next_->prev_ = f->prev_;
    }

    static
    void
    link_nodes(link_pointer p, link_pointer f, link_pointer l)
    noexcept
    {
        p->prev_->next_ = f;
        f->prev_ = p->prev_;
        p->prev_ = l;
        l->next_ = p;
    }

    void
    link_nodes_at_front(link_pointer f, link_pointer l)
    noexcept
    {
        f->prev_ = end_as_link();
        l->next_ = end_.next_;
        l->next_->prev_ = l;
        end_.next_ = f;
    }

    void
    link_nodes_at_back(link_pointer f, link_pointer l)
    noexcept
    {
        l->next_ = end_as_link();
        f->prev_ = end_.prev_;
        f->prev_->next_ = f;
        end_.prev_ = l;
    }

    template <typename Compare>
    void
    merge_list(list_facet& x, Compare& comp)
    {
        iterator f1 = begin();
        iterator e1 = end();
        iterator f2 = x.begin();
        iterator e2 = x.end();
        while (f1 != e1 && f2 != e2) {
            if (comp(*f2, *f1)) {
                size_type ds = 1;
                iterator m2 = std::next(f2);
                for (; m2 != e2 && comp(*m2, *f1); ++m2, ++ds)
                    ;
                size() += ds;
                x.size() -= ds;
                link_pointer f = f2.ptr_;
                link_pointer l = m2.ptr_->prev_;
                f2 = m2;
                unlink_nodes(f, l);
                m2 = std::next(f1);
                link_nodes(f1.ptr_, f, l);
                f1 = m2;
            } else {
                ++f1;
            }
        }
        splice(e1, x);
    }

    template <typename Compare>
    static
    iterator
    sort_list(iterator f1, iterator l2, size_type n, Compare& comp)
    {
        switch (n) {
            case 0:
            case 1:
                return f1;
            case 2:
                if (comp(*--l2, *f1)) {
                    link_pointer f = l2.ptr_;
                    unlink_nodes(f, f);
                    link_nodes(f1.ptr_, f, f);
                    return l2;
                }
                return f1;
        }
        size_type n2 = n / 2;
        iterator e1 = std::next(f1, n2);
        iterator  r = f1 = sort_list(f1, e1, n2, comp);
        iterator f2 = e1 = sort_list(e1, l2, n - n2, comp);
        if (comp(*f2, *f1)) {
            iterator m2 = std::next(f2);
            for (; m2 != l2 && comp(*m2, *f1); ++m2)
                ;
            link_pointer f = f2.ptr_;
            link_pointer l = m2.ptr_->prev_;
            r = f2;
            e1 = f2 = m2;
            unlink_nodes(f, l);
            m2 = std::next(f1);
            link_nodes(f1.ptr_, f, l);
            f1 = m2;
        } else {
            ++f1;
        }
        while (f1 != e1 && f2 != l2) {
            if (comp(*f2, *f1)) {
                iterator m2 = std::next(f2);
                for (; m2 != l2 && comp(*m2, *f1); ++m2)
                    ;
                link_pointer f = f2.ptr_;
                link_pointer l = m2.ptr_->prev_;
                if (e1 == f2) {
                    e1 = m2;
                }
                f2 = m2;
                unlink_nodes(f, l);
                m2 = std::next(f1);
                link_nodes(f1.ptr_, f, l);
                f1 = m2;
            } else {
                ++f1;
            }
        }
        return r;
    }
};

template <typename T, typename VoidPtr>
inline
bool
operator==(
    const list_facet<T, VoidPtr>& x,
    const list_facet<T, VoidPtr>& y
)
{
    return x.size() == y.size() && std::equal(x.begin(), x.end(), y.begin());
}

template <typename T, typename VoidPtr>
inline
bool
operator!=(
    const list_facet<T, VoidPtr>& x,
    const list_facet<T, VoidPtr>& y
)
{
    return !(x == y);
}

template <typename T, typename VoidPtr>
inline
bool
operator<(
    const list_facet<T, VoidPtr>& x,
    const list_facet<T, VoidPtr>& y
)
{
    return std::lexicographical_compare(x.begin(), x.end(), y.begin(), y.end());
}

template <typename T, typename VoidPtr>
inline
bool
operator>(
    const list_facet<T, VoidPtr>& x,
    const list_facet<T, VoidPtr>& y
)
{
    return y < x;
}

template <typename T, typename VoidPtr>
inline
bool
operator>=(
    const list_facet<T, VoidPtr>& x,
    const list_facet<T, VoidPtr>& y
)
{
    return !(x < y);
}

template <typename T, typename VoidPtr>
inline
bool
operator<=(
    const list_facet<T, VoidPtr>& x,
    const list_facet<T, VoidPtr>& y
)
{
    return !(y < x);
}

// LIST

template <
    typename T,
    typename Allocator = std::allocator<T>
>
class list
{
public:
    using value_type = T;
    using allocator_type = Allocator;
    using facet_type = list_facet<
        value_type,
        typename std::allocator_traits<allocator_type>::void_pointer
    >;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename facet_type::pointer;
    using const_pointer = typename facet_type::const_pointer;
    using size_type = typename facet_type::size_type;
    using difference_type = typename facet_type::difference_type;
    using iterator = typename facet_type::iterator;
    using const_iterator = typename facet_type::const_iterator;
    using reverse_iterator = typename facet_type::reverse_iterator;
    using const_reverse_iterator = typename facet_type::const_reverse_iterator;

protected:
    static_assert(
        std::is_same<typename allocator_type::value_type, value_type>::value,
        "Allocator::value_type must be same type as value_type"
    );

    using alloc_traits = std::allocator_traits<allocator_type>;
    using node = typename facet_type::node;
    using node_pointer = typename facet_type::node_pointer;
    using node_allocator = typename alloc_traits::template rebind_alloc<node>;
    using node_traits = std::allocator_traits<node_allocator>;
    using node_base = typename facet_type::node_base;
    using node_base_pointer = typename facet_type::node_base_pointer;
    using node_base_allocator = typename alloc_traits::template rebind_alloc<node_base>;
    using node_base_traits = std::allocator_traits<node_base_allocator>;
    using link_pointer = typename facet_type::link_pointer;

public:
    // Constructors
    list()
    noexcept
    {}

    explicit
    list(const allocator_type& alloc)
    noexcept:
        data_(alloc)
    {}

    explicit
    list(size_type n):
        list()
    {
        for (; n != 0; --n) {
            emplace_back();
        }
    }

    explicit
    list(size_type n, const allocator_type& alloc):
        list(alloc)
    {
        for (; n != 0; --n) {
            emplace_back();
        }
    }

    list(size_type n, const value_type& x):
        list()
    {
        for (; n != 0; --n) {
            push_back(x);
        }
    }

    list(size_type n, const value_type& x, const allocator_type& alloc):
        list(alloc)
    {
        for (; n != 0; --n) {
            push_back(x);
        }
    }

    template <typename InputIter, enable_input_iterable_t<InputIter>* = nullptr>
    list(InputIter f,InputIter l):
        list()
    {
        for (; f != l; ++f) {
            emplace_back(*f);
        }
    }

    template <typename InputIter, enable_input_iterable_t<InputIter>* = nullptr>
    list(InputIter f, InputIter l, const allocator_type& alloc):
        list(alloc)
    {
        for (; f != l; ++f) {
            emplace_back(*f);
        }
    }

    list(const list& x):
        list(x, allocator_type(node_traits::select_on_container_copy_construction(x.alloc())))
    {}

    list(const list& x, const allocator_type& alloc):
        list(alloc)
    {
        for (const_iterator f = x.begin(), l = x.end(); f != l; ++f) {
            push_back(*f);
        }
    }

    list(std::initializer_list<value_type> il):
        list()
    {
        for (auto f = il.begin(), l = il.end(); f != l; ++f) {
            push_back(*f);
        }
    }

    list(std::initializer_list<value_type> il, const allocator_type& alloc):
        list(alloc)
    {
        for (auto f = il.begin(), l = il.end(); f != l; ++f) {
            push_back(*f);
        }
    }

    list(list&& x)
    noexcept:
        list()
    {
        splice(end(), x);
    }

    list(list&& x, const allocator_type& alloc):
        list(alloc)
    {
        if (alloc == x.get_allocator()) {
            splice(end(), x);
        } else {
            using iter = std::move_iterator<iterator>;
            assign(iter(x.begin()), iter(x.end()));
        }
    }

    // Assignment
    list&
    operator=(const list& x)
    {
        if (this != &x) {
            copy_assign_alloc(x);
            assign(x.begin(), x.end());
        }
        return *this;
    }

    list&
    operator=(list&& x)
    noexcept
    {
        move_assign(x);
        return *this;
    }

    list& operator=(std::initializer_list<value_type> il)
    {
        assign(il.begin(), il.end());
        return *this;
    }

    // Assign
    void assign(std::initializer_list<value_type> il)
    {
        assign(il.begin(), il.end());
    }

    template <typename InputIter, enable_input_iterable_t<InputIter>* = nullptr>
    void
    assign(InputIter f, InputIter l)
    {
        iterator i = begin();
        iterator e = end(i);
        for (; f != l && i != e; ++f, ++i) {
            *i = *f;
        }
        if (i == e) {
// TODO:
//            insert(e, f, l);
        } else {
// TODO:
//            erase(i, e);
        }
    }

    void assign(size_type n, const value_type& x)
    {
        iterator i = begin();
        iterator e = end();
        for (; n > 0 && i != e; --n, ++i) {
            *i = x;
        }
        if (i == e) {
// TODO:
//            insert(e, n, x);
        } else {
// TODO:
//            erase(i, e);
        }
    }

    // Destructors
    ~list()
    {
        clear();
    }

    // Observers
    allocator_type
    get_allocator()
    const noexcept
    {
        return allocator_type(alloc());
    }

    // Element access
    reference
    front()
    {
        return facet().front();
    }

    const_reference
    front()
    const
    {
        return facet().front();
    }

    reference
    back()
    {
        return facet().back();
    }

    const_reference
    back()
    const
    {
        return facet().back();
    }

    // Iterators
    iterator
    begin()
    noexcept
    {
        return facet().begin();
    }

    const_iterator
    begin()
    const noexcept
    {
        return facet().begin();
    }

    const_iterator
    cbegin()
    const noexcept
    {
        return facet().cbegin();
    }

    iterator
    end()
    noexcept
    {
        return facet().end();
    }

    const_iterator
    end()
    const noexcept
    {
        return facet().end();
    }

    const_iterator
    cend()
    const noexcept
    {
        return facet().cend();
    }

    iterator
    rbegin()
    noexcept
    {
        return facet().rbegin();
    }

    const_iterator
    rbegin()
    const noexcept
    {
        return facet().rbegin();
    }

    const_iterator
    crbegin()
    const noexcept
    {
        return facet().crbegin();
    }

    iterator
    rend()
    noexcept
    {
        return facet().rend();
    }

    const_iterator
    rend()
    const noexcept
    {
        return facet().rend();
    }

    const_iterator
    crend()
    const noexcept
    {
        return facet().crend();
    }

    // Capacity
    bool
    empty()
    const noexcept
    {
        return facet().empty();
    }

    size_type
    size()
    const noexcept
    {
        return facet().size();
    }

    size_type
    max_size()
    const noexcept
    {
        return facet().max_size();
    }

    // Modifiers
    void
    clear()
    noexcept
    {
        if (!empty()) {
            node_allocator& a = alloc();
            link_pointer f = facet().begin_node_base();
            link_pointer l = facet().end_as_link();
            facet().unlink_nodes(f, l->prev_);
            facet().size_ = 0;
            while (f != l) {
                node_pointer np = f->as_node();
                f = f->next_;
                node_traits::destroy(a, addressof(np->value_));
                node_traits::deallocate(a, np, 1);
            }
        }
    }

    void
    push_front(const value_type& x)
    {
        node_allocator& a = alloc();
        auto hold = allocate_node(a);
        node_traits::construct(a, std::addressof(hold->value_), x);
        link_pointer nl = hold->as_link();
        link_nodes_at_front(nl, nl);
        ++facet().size_;
        hold.release();
    }

    void
    push_front(value_type&& x)
    {
        node_allocator& a = alloc();
        auto hold = allocate_node(a);
        node_traits::construct(a, std::addressof(hold->value_), std::move(x));
        link_pointer nl = hold->as_link();
        link_nodes_at_front(nl, nl);
        ++facet().size_;
        hold.release();
    }

    void
    push_back(const value_type& x)
    {
        node_allocator& a = alloc();
        auto hold = allocate_node(a);
        node_traits::construct(a, std::addressof(hold->value_), x);
        link_pointer nl = hold->as_link();
        link_nodes_at_back(nl, nl);
        ++facet().size_;
        hold.release();
    }

    void
    push_back(value_type&& x)
    {
        node_allocator& a = alloc();
        auto hold = allocate_node(a);
        node_traits::construct(a, std::addressof(hold->value_), std::move(x));
        link_pointer nl = hold->as_link();
        link_nodes_at_back(nl, nl);
        ++facet().size_;
        hold.release();
    }

    template <typename ... Ts>
    reference
    emplace_front(Ts&&... ts)
    {
        node_allocator& a = alloc();
        auto hold = allocate_node(a);
        node_traits::construct(a, std::addressof(hold->value_), std::forward<Ts>(ts)...);
        link_pointer nl = hold->as_link();
        link_nodes_at_front(nl, nl);
        ++facet().size_;
        return hold.release()->value_;
    }

    template <typename ... Ts>
    reference
    emplace_back(Ts&&... ts)
    {
        node_allocator& a = alloc();
        auto hold = allocate_node(a);
        node_traits::construct(a, std::addressof(hold->value_), std::forward<Ts>(ts)...);
        link_pointer nl = hold->as_link();
        link_nodes_at_back(nl, nl);
        ++facet().size_;
        return hold.release()->value_;
    }

    template <typename ... Ts>
    iterator
    emplace(const_iterator p, Ts&&... ts)
    {
        node_allocator& a = alloc();
        hold_pointer h = allocate_node(a);
        node_traits::construct(a, std::addressof(h->value_), std::forward<Ts>(ts)...);
        link_pointer nl = h.get()->as_link();
        link_nodes(p.ptr_, nl, nl);
        ++facet().size_;
        h.release();
        return iterator(nl);
    }

    iterator
    insert(const_iterator p, const value_type& x)
    {
        node_allocator& a = alloc();
        hold_pointer h = allocate_node(a);
        node_traits::construct(a, std::addressof(h->value_), x);
        link_nodes(p.ptr_, h->as_link(), h->as_link());
        ++facet().size_;
        return iterator(h.release()->as_link());
    }

    iterator
    insert(const_iterator p, value_type&& x)
    {
        return emplace(p, std::move(x));
    }

    iterator
    insert(const_iterator p, std::initializer_list<value_type> il)
    {
        return insert(p, il.begin(), il.end());
    }

    iterator
    insert(const_iterator p, size_type n, const value_type& x)
    {
        iterator r(p.ptr_);
        if (n > 0) {
            size_type ds = 0;
            node_allocator& a = alloc();
            hold_pointer h = allocate_node(a);
            node_traits::construct(a, std::addressof(h->value_), x);
            ++ds;
            r = iterator(h->as_link());
            h.release();
            iterator e = r;
            try {
                for (--n; n != 0; --n, ++e, ++ds) {
                    h.reset(node_traits::allocate(a, 1));
                    node_traits::construct(a, std::addressof(h->value_), x);
                    e.ptr_->next_ = h->as_link();
                    h->prev_ = e.ptr_;
                    h.release();
                }
            } catch (...) {
                while (true) {
                    node_traits::destroy(a, std::addressof(*e));
                    link_pointer prev = e.ptr_->prev_;
                    node_traits::deallocate(a, e.ptr_->as_node(), 1);
                    if (prev == 0) {
                        break;
                    }
                    e = iterator(prev);
                }
                throw;
            }
            link_nodes(p.ptr_, r.ptr_, e.ptr_);
            facet().size_ += ds;
        }
        return r;
    }

    template <typename InputIter, enable_input_iterable_t<InputIter>* = nullptr>
    iterator
    insert(const_iterator p, InputIter f, InputIter l)
    {
        iterator r(p.ptr_);
        if (f != l)
        {
            size_type ds = 0;
            node_allocator& a = alloc();
            hold_pointer h = allocate_node(a);
            node_traits::construct(a, std::addressof(h->value_), *f);
            ++ds;
            r = iterator(h.get()->as_link());
            h.release();
            iterator e = r;
            try {
                for (++f; f != l; ++f, (void) ++e, (void) ++ds) {
                    h.reset(node_traits::allocate(a, 1));
                    node_traits::construct(a, std::addressof(h->value_), *f);
                    e.ptr_->next_ = h.get()->as_link();
                    h->prev_ = e.ptr_;
                    h.release();
                }
            } catch (...) {
                while (true) {
                    node_traits::destroy(a, std::addressof(*e));
                    link_pointer prev = e.ptr_->prev_;
                    node_traits::deallocate(a, e.ptr_->as_node(), 1);
                    if (prev == 0) {
                        break;
                    }
                    e = iterator(prev);
                }
                throw;
            }
            link_nodes(p.ptr_, r.ptr_, e.ptr_);
            facet().size_ += ds;
        }
        return r;
    }

    iterator
    erase(const_iterator p)
    {
        assert(p != end() && "list::erase(iterator) called with a non-dereferenceable iterator");
        node_allocator& a = alloc();
        link_pointer n = p.ptr_;
        link_pointer r = n->next_;
        facet().unlink_nodes(n, n);
        --facet().size_;
        node_pointer np = n->as_node();
        node_traits::destroy(a, std::addressof(np->value_));
        node_traits::deallocate(a, np, 1);

        return iterator(r);
    }

    iterator
    erase(const_iterator f, const_iterator l)
    {
        if (f != l) {
            node_allocator& a = alloc();
            facet().unlink_nodes(f.ptr_, l.ptr_->prev_);
            while (f != l)
            {
                link_pointer n = f.ptr_;
                ++f;
                --facet().size_;
                node_pointer np = n->as_node();
                node_traits::destroy(a, std::addressof(np->value_));
                node_traits::deallocate(a, np, 1);
            }
        }
        return iterator(l.ptr_);
    }

    void
    resize(size_type n)
    {
        if (n < facet().size_) {
            erase(iterator_impl(n), end());
        } else if (n > facet().size_)
        {
            n -= facet().size_;
            size_type d = 0;
            node_allocator& a = alloc();
            hold_pointer h = allocate_node(a);
            node_traits::construct(a, std::addressof(h->value_));
            ++d;
            iterator r = iterator(h.release()->as_link());
            iterator e = r;
            try {
                for (--n; n != 0; --n, ++e, ++d) {
                    h.reset(node_traits::allocate(a, 1));
                    node_traits::construct(a, std::addressof(h->value_));
                    e.ptr_->next_ = h.get()->as_link();
                    h->prev_ = e.ptr_;
                    h.release();
                }
            }
            catch (...) {
                while (true) {
                    node_traits::destroy(a, std::addressof(*e));
                    link_pointer prev = e.ptr_->prev_;
                    node_traits::deallocate(a, e.ptr_->as_node(), 1);
                    if (prev == 0)
                        break;
                    e = iterator(prev);
                }
                throw;
            }
            facet().link_nodes_at_back(r.ptr_, e.ptr_);
            facet().size_ += d;
        }
    }

    void
    resize(size_type n, const value_type& x)
    {
        if (n < facet().size_) {
            erase(iterator_impl(n), end());
        } else if (n > facet().size_) {
            n -= facet().size_;
            size_type ds = 0;
            node_allocator& na = alloc();
            hold_pointer h = allocate_node(na);
            node_traits::construct(na, std::addressof(h->value_), x);
            ++ds;
            link_pointer nl = h.release()->as_link();
            iterator r = iterator(nl);
            iterator e = r;
            try {
                for (--n; n != 0; --n, ++e, ++ds) {
                    h.reset(node_traits::allocate(na, 1));
                    node_traits::construct(na, std::addressof(h->value_), x);
                    e.ptr_->next_ = h.get()->as_link();
                    h->prev_ = e.ptr_;
                    h.release();
                }
            } catch (...) {
                while (true) {
                    node_traits::destroy(na, std::addressof(*e));
                    link_pointer prev = e.ptr_->prev_;
                    node_traits::deallocate(na, e.ptr_->as_node(), 1);
                    if (prev == 0) {
                        break;
                    }
                    e = iterator(prev);
                }
                throw;
            }
            facet().link_nodes(facet().end_as_link(), r.ptr_, e.ptr_);
            facet().size_ += ds;
        }
    }

    void
    pop_front()
    {
        assert(!empty() && "list::pop_front() called with empty list");
        node_allocator& a = alloc();
        link_pointer n = facet().end_.next_;
        facet().unlink_nodes(n, n);
        --facet().size_;
        node_pointer np = n->as_node();
        node_traits::destroy(a, std::addressof(np->value_));
        node_traits::deallocate(a, np, 1);
    }

    void
    pop_back()
    {
        assert(!empty() && "list::pop_back() called with empty list");
        node_allocator& a = alloc();
        link_pointer n = facet().end_.prev_;
        facet().unlink_nodes(n, n);
        --facet().size_;
        node_pointer np = n->as_node();
        node_traits::destroy(a, std::addressof(np->value_));
        node_traits::deallocate(a, np, 1);
    }

    void
    swap(list& x)
    noexcept
    {
        facet().swap(x.facet());
        swap_allocator(alloc(), x.alloc());
    }

    // Operations
    void
    merge(list&& x)
    {
        facet().merge(move(x.facet()));
    }

    template <typename Compare>
    void
    merge(list&& x, Compare comp)
    {
        facet().merge(move(x.facet()), move(comp));
    }

    void
    merge(list& x)
    {
        facet().merge(x.facet());
    }

    template <typename Compare>
    void
    merge(list& x, Compare comp)
    {
        facet().merge(x.facet(), move(comp));
    }

    void
    splice(const_iterator p, list& x)
    {
        facet().splice(p, x.facet());
    }

    void
    splice(const_iterator p, list&& x)
    {
        facet().splice(p, move(x.facet()));
    }

    void
    splice(const_iterator p, list&& x, const_iterator i)
    {
        facet().splice(p, move(x.facet()), i);
    }

    void
    splice(
        const_iterator p,
        list&& x,
        const_iterator f,
        const_iterator l
    )
    {
        facet().splice(p, move(x.facet()), f, l);
    }

    void
    splice(const_iterator p, list& x, const_iterator i)
    {
        facet().splice(p, x.facet(), i);
    }

    void
    splice(
        const_iterator p,
        list& x,
        const_iterator f,
        const_iterator l
    )
    {
        facet().splice(p, x.facet(), f, l);
    }

    void
    remove(const_reference v)
    {
        remove_if([&v](const_reference value) -> bool {
            return value == v;
        });
    }

    template <typename Predicate>
    void
    remove_if(Predicate pred)
    {
        for (iterator i = begin(), e = end(); i != e;) {
            if (pred(*i)) {
                iterator j = std::next(i);
                for (; j != e && pred(*j); ++j)
                    ;
                i = erase(i, j);
                if (i != e) {
                    ++i;
                }
            } else {
                ++i;
            }
        }
    }

    void
    reverse()
    noexcept
    {
        facet().reverse();
    }

    void
    unique()
    {
        unique(std::equal_to<value_type>());
    }

    template <typename Predicate>
    void
    unique(Predicate pred)
    {
        for (iterator i = begin(), e = end(); i != e;) {
            iterator j = std::next(i);
            for (; j != e && pred(*i, *j); ++j)
                ;
            if (++i != j) {
                erase_after(i, j);
            }
            i = j;
        }
    }

    void
    sort()
    {
        facet().sort();
    }

    template <typename Compare>
    void
    sort(Compare comp)
    {
        facet().sort(move(comp));
    }

    // Facet
    facet_type&
    facet()
    noexcept
    {
        return get<0>(data_);
    }

    const facet_type&
    facet()
    const noexcept
    {
        return get<0>(data_);
    }

private:
    compressed_pair<facet_type, node_allocator> data_;
    using deleter = allocator_destructor<node_allocator, 1>;
    using hold_pointer = std::unique_ptr<node, deleter>;

    // Allocator
    node_allocator&
    alloc()
    noexcept
    {
        return get<1>(data_);
    }

    const node_allocator&
    alloc()
    const noexcept
    {
        return get<1>(data_);
    }

    // Copy Assign Alloc
    void
    copy_assign_alloc(const list& x, std::true_type)
    {
        if (alloc() != x.alloc()) {
            clear();
        }
        alloc() = x.alloc();
    }

    void
    copy_assign_alloc(const list&, std::false_type)
    {}

    void
    copy_assign_alloc(const list& x)
    {
        constexpr bool propagate = node_traits::propagate_on_container_copy_assignment::value;
        copy_assign_alloc(x, std::integral_constant<bool, propagate>());
    }

    // Move Assign Alloc
    void
    move_assign_alloc(list& x, std::true_type)
    noexcept
    {
        alloc() = move(x.alloc());
    }

    void
    move_assign_alloc(list&, std::false_type)
    noexcept
    {}

    void
    move_assign_alloc(list& x)
    noexcept
    {
        constexpr bool propagate = node_traits::propagate_on_container_move_assignment::value;
        move_assign_alloc(x, std::integral_constant<bool, propagate>());
    }

    // Move Assign
    void
    move_assign(list& x, std::true_type)
    {
        clear();
        move_assign_alloc(x);
        facet().begin_pointer() = x.facet().begin_pointer();
        x.facet().begin_pointer() = nullptr;
    }

    void
    move_assign(list& x, std::false_type)
    {
        if (alloc() == x.alloc()) {
            move_assign(x, std::true_type());
        } else {
            using iter = std::move_iterator<iterator>;
            assign(iter(x.begin()), iter(x.end()));
        }
    }

    void
    move_assign(list& x)
    {
        constexpr bool propagate = node_traits::propagate_on_container_move_assignment::value;
        move_assign(x, std::integral_constant<bool, propagate>());
    }

    // Allocate Node
    hold_pointer
    allocate_node(node_allocator& alloc)
    {
        node_pointer p = node_traits::allocate(alloc, 1);
        p->prev_ = nullptr;
        return hold_pointer(p, deleter(alloc));
    }

    //Iterator impl
    iterator
    iterator_impl(size_type n)
    {
        if (n <= facet().size_ / 2) {
            return std::next(begin(), n);
        }
        std::prev(end(), facet().size_ - n);
    }

};

template <typename T, typename Allocator>
inline
bool
operator==(
    const list<T, Allocator>& x,
    const list<T, Allocator>& y
)
{
    return x.facet() == y.facet();
}

template <typename T, typename Allocator>
inline
bool
operator!=(
    const list<T, Allocator>& x,
    const list<T, Allocator>& y
)
{
    return x.facet() != y.facet();
}

template <typename T, typename Allocator>
inline
bool
operator<(
    const list<T, Allocator>& x,
    const list<T, Allocator>& y
)
{
    return x.facet() < y.facet();
}

template <typename T, typename Allocator>
inline
bool
operator>(
    const list<T, Allocator>& x,
    const list<T, Allocator>& y
)
{
    return x.facet() > y.facet();
}

template <typename T, typename Allocator>
inline
bool
operator>=(
    const list<T, Allocator>& x,
    const list<T, Allocator>& y
)
{
    return x.facet() >= y.facet();
}

template <typename T, typename Allocator>
inline
bool
operator<=(
    const list<T, Allocator>& x,
    const list<T, Allocator>& y
)
{
    return x.facet() <= y.facet();
}

template <typename T, typename Allocator>
inline
void
swap(
    list<T, Allocator>& x,
    list<T, Allocator>& y
)
noexcept
{
    x.swap(y);
}

}   /* contained */
