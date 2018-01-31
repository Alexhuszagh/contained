//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \synopsis
 // TODO: implement.
 // Need the facet
 *      template <class T, class Allocator = allocator<T>>
 *      class vector
 *      {
 *      public:
 *          typedef T                                        value_type;
 *          typedef Allocator                                allocator_type;
 *          typedef typename allocator_type::reference       reference;
 *          typedef typename allocator_type::const_reference const_reference;
 *          typedef implementation-defined                   iterator;
 *          typedef implementation-defined                   const_iterator;
 *          typedef typename allocator_type::size_type       size_type;
 *          typedef typename allocator_type::difference_type difference_type;
 *          typedef typename allocator_type::pointer         pointer;
 *          typedef typename allocator_type::const_pointer   const_pointer;
 *          typedef std::reverse_iterator<iterator>          reverse_iterator;
 *          typedef std::reverse_iterator<const_iterator>    const_reverse_iterator;
 *          vector() noexcept;
 *          explicit vector(const allocator_type&);
 *          explicit vector(size_type n);
 *          explicit vector(size_type n, const allocator_type&);
 *          vector(size_type n, const value_type& value, const allocator_type& = allocator_type());
 *          template <class InputIterator>
 *              vector(InputIterator first, InputIterator last, const allocator_type& = allocator_type());
 *          vector(const vector& x);
 *          vector(vector&& x) noexcept;
 *          vector(initializer_list<value_type> il);
 *          vector(initializer_list<value_type> il, const allocator_type& a);
 *          ~vector();
 *          vector& operator=(const vector& x);
 *          vector& operator=(vector&& x) noexcept;
 *          vector& operator=(initializer_list<value_type> il);
 *          template <class InputIterator>
 *              void assign(InputIterator first, InputIterator last);
 *          void assign(size_type n, const value_type& u);
 *          void assign(initializer_list<value_type> il);
 *          allocator_type get_allocator() const noexcept;
 *          iterator               begin() noexcept;
 *          const_iterator         begin()   const noexcept;
 *          iterator               end() noexcept;
 *          const_iterator         end()     const noexcept;
 *          reverse_iterator       rbegin() noexcept;
 *          const_reverse_iterator rbegin()  const noexcept;
 *          reverse_iterator       rend() noexcept;
 *          const_reverse_iterator rend()    const noexcept;
 *          const_iterator         cbegin()  const noexcept;
 *          const_iterator         cend()    const noexcept;
 *          const_reverse_iterator crbegin() const noexcept;
 *          const_reverse_iterator crend()   const noexcept;
 *          size_type size() const noexcept;
 *          size_type max_size() const noexcept;
 *          size_type capacity() const noexcept;
 *          bool empty() const noexcept;
 *          void reserve(size_type n);
 *          void shrink_to_fit() noexcept;
 *          reference       operator[](size_type n);
 *          const_reference operator[](size_type n) const;
 *          reference       at(size_type n);
 *          const_reference at(size_type n) const;
 *          reference       front();
 *          const_reference front() const;
 *          reference       back();
 *          const_reference back() const;
 *          value_type*       data() noexcept;
 *          const value_type* data() const noexcept;
 *          void push_back(const value_type& x);
 *          void push_back(value_type&& x);
 *          template <class... Args>
 *              reference emplace_back(Args&&... args);
 *          void pop_back();
 *          template <class... Args> iterator emplace(const_iterator position, Args&&... args);
 *          iterator insert(const_iterator position, const value_type& x);
 *          iterator insert(const_iterator position, value_type&& x);
 *          iterator insert(const_iterator position, size_type n, const value_type& x);
 *          template <class InputIterator>
 *              iterator insert(const_iterator position, InputIterator first, InputIterator last);
 *          iterator insert(const_iterator position, initializer_list<value_type> il);
 *          iterator erase(const_iterator position);
 *          iterator erase(const_iterator first, const_iterator last);
 *          void clear() noexcept;
 *          void resize(size_type sz);
 *          void resize(size_type sz, const value_type& c);
 *          void swap(vector&) noexcept;
 *      };
 */

#pragma once

#include <contained/detail/split_buffer.h>
#include <cassert>
#include <functional>
#include <initializer_list>
#include <limits>
#include <stdexcept>
#include <utility>

namespace contained
{
// OBJECTS
// -------

// VECTOR FACET

template <typename T, typename VoidPtr = void*>
class vector_facet
{
public:
    using value_type = T;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = typename std::pointer_traits<VoidPtr>::template rebind<value_type>;
    using const_pointer = typename std::pointer_traits<VoidPtr>::template rebind<const value_type>;
    using difference_type = typename std::pointer_traits<VoidPtr>::difference_type;
    using size_type = typename std::make_unsigned<difference_type>::type;
    using iterator = pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // Constructors
    vector_facet():
        begin_(nullptr),
        end_(nullptr),
        end_cap_(nullptr)
    {}
    // TODO: add iterator, other methods

    // Iterators
    iterator
    begin()
    noexcept
    {
        return iterator(begin_);
    }

    const_iterator
    begin()
    const noexcept
    {
        return const_iterator(begin_);
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
        return iterator(end_);
    }

    const_iterator
    end()
    const noexcept
    {
        return const_iterator(end_);
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

    // Element access
    reference
    at(
        size_type n
    )
    {
        if (n >= size()) {
            throw std::out_of_range("vector");
        }
        return (*this)[n];
    }

    const_reference
    at(
        size_type n
    ) const
    {
        if (n >= size()) {
            throw std::out_of_range("vector");
        }
        return (*this)[n];
    }

    reference
    operator[](
        size_type n
    )
    {
        assert(n < size() && "vector[] index out of bounds");
        return begin_[n];
    }

    const_reference
    operator[](
        size_type n
    ) const
    {
        assert(n < size() && "vector[] index out of bounds");
        return begin_[n];
    }

    reference
    front()
    {
        assert(!empty() && "front() called for empty vector");
        return *begin_;
    }

    const_reference
    front()
    const
    {
        assert(!empty() && "front() called for empty vector");
        return *begin_;
    }

    reference
    back()
    {
        assert(!empty() && "back() called for empty vector");
        return *(end_ - 1);
    }

    const_reference
    back()
    const
    {
        assert(!empty() && "back() called for empty vector");
        return *(end_ - 1);
    }

    value_type*
    data()
    noexcept
    {
        return to_raw_pointer(begin_);
    }

    const value_type*
    data()
    const noexcept
    {
        return to_raw_pointer(begin_);
    }

    // Capacity
    CONTAINED_CPP17_NODISCARD
    bool
    empty()
    const noexcept
    {
        return begin_ == end_;
    }

    size_type
    size()
    const noexcept
    {
        return static_cast<size_type>(end_ - begin_);
    }

    size_type
    max_size()
    const noexcept
    {
        // guaranteed to be constexpr
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    size_type
    capacity()
    const noexcept
    {
        return static_cast<size_type>(end_cap_ - begin_);
    }

    // TODO: add more methods

protected:
    pointer begin_;
    pointer end_;
    pointer end_cap_;

    // Constructors
    vector_facet(const vector_facet&) = delete;

    vector_facet(vector_facet&& x)
    noexcept:
        begin_(std::move(x.begin_)),
        end_(std::move(x.end_)),
        end_cap_(std::move(x.end_cap_))
    {
        x.begin_ = x.end_ = x.end_cap_ = nullptr;
    }

    // Assignment
    vector_facet& operator=(const vector_facet&) = delete;

    vector_facet&
    operator=(vector_facet&& x)
    {
        swap(x);
        return *this;
    }

    // Modifiers
    void
    swap(vector_facet& x)
    {
        std::swap(begin_, x.begin_);
        std::swap(end_, x.end_);
        std::swap(end_cap_, x.end_cap_);
    }

    // TODO: private constructors, modifiers, etc...
};

// VECTOR

template <
    typename T,
    typename Allocator = std::allocator<T>
>
class vector
{
protected:
    using alloc_traits = std::allocator_traits<Allocator>;
    using void_pointer = typename alloc_traits::void_pointer;

public:
    using value_type = T;
    using allocator_type = Allocator;
    using buffer_type = split_buffer<value_type, void_pointer>;
    using facet_type = vector_facet<value_type, void_pointer>;
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

    // TODO: add here...

    // Constructors
    vector()
    noexcept
    {}

    vector(const allocator_type& alloc)
    noexcept:
        data_(alloc)
    {}

    explicit
    vector(size_type n):
        vector()
    {
        if (n > 0) {
            allocate(n);
            construct_at_end(n);
        }
    }

    explicit
    vector(size_type n, const allocator_type& alloc):
        vector(alloc)
    {
        if (n > 0) {
            allocate(n);
            construct_at_end(n);
        }
    }

    vector(size_type n, const_reference x):
        vector()
    {
        if (n > 0) {
            allocate(n);
            construct_at_end(n, x);
        }
    }

    vector(size_type n, const_reference x, const allocator_type& alloc):
        vector(alloc)
    {
        if (n > 0) {
            allocate(n);
            construct_at_end(n, x);
        }
    }

    template <typename InputIter, enable_input_iterator_t<InputIter>* = nullptr>
    vector(InputIter first, InputIter last):
        vector()
    {
        for (; first != last; ++first) {
            emplace_back(*first);
        }
    }

    template <typename InputIter, enable_input_iterator_t<InputIter>* = nullptr>
    vector(InputIter first, InputIter last, const allocator_type& alloc):
        vector(alloc)
    {
        for (; first != last; ++first) {
            emplace_back(*first);
        }
    }

    template <typename ForwardIter, enable_forward_iterable_t<ForwardIter>* = nullptr>
    vector(ForwardIter first, ForwardIter last):
        vector()
    {
        size_type n = static_cast<size_type>(std::distance(first, last));
        if (n > 0) {
            allocate(n);
            construct_at_end(first, last, n);
        }
    }

    template <typename ForwardIter, enable_forward_iterable_t<ForwardIter>* = nullptr>
    vector(ForwardIter first, ForwardIter last, const allocator_type& alloc):
        vector(alloc)
    {
        size_type n = static_cast<size_type>(std::distance(first, last));
        if (n > 0) {
            allocate(n);
            construct_at_end(first, last, n);
        }
    }

    vector(std::initializer_list<value_type> il):
        vector()
    {
        size_type n = il.size();
        if (n > 0) {
            allocate(n);
            construct_at_end(il.begin(), il.end(), n);
        }
    }

    vector(std::initializer_list<value_type> il, const allocator_type& alloc):
        vector(alloc)
    {
        size_type n = il.size();
        if (n > 0) {
            allocate(n);
            construct_at_end(il.begin(), il.end(), n);
        }
    }

    // Copy Constructors
    vector(const vector& x):
        vector(alloc_traits::select_on_container_copy_construction(x.alloc()))
    {
        size_type n = x.size();
        if (n > 0) {
            allocate(n);
            construct_at_end(x.facet().begin_, x.facet().end_, n);
        }
    }

    vector(const vector& x, const allocator_type& alloc):
        vector(alloc)
    {
        size_type n = x.size();
        if (n > 0) {
            allocate(n);
            construct_at_end(x.facet().begin_, x.facet().end_, n);
        }
    }

    // Move Constructors
    vector(vector&& x)
    noexcept:
        data_(std::move(x.alloc()))
    {
        facet().begin_ = x.begin_;
        facet().end_ = x.end_;
        facet().end_cap_ = x.end_cap_;
        x.facet().begin_ = x.facet().end_ = x.facet().end_cap_ = nullptr;
    }

    vector(vector&& x, const allocator_type& alloc):
        data_(alloc)
    {
        if (alloc == x.alloc()) {
            facet().begin_ = x.begin_;
            facet().end_ = x.end_;
            facet().end_cap_ = x.end_cap_;
            x.facet().begin_ = x.facet().end_ = x.facet().end_cap_ = nullptr;
        } else {
            using iter = std::move_iterator<iterator>;
            assign(iter(x.begin()), iter(x.end()));
        }
    }

    // Destructors
    ~vector()
    {
        if (facet().begin_ != nullptr) {
            clear();
            alloc_traits::deallocate(alloc(), facet().begin_, capacity());
        }
    }

    // Assignment
    // vector& operator=(const vector& x);
    // vector& operator=(vector&& x);
    // vector& operator=(std::initializer_list<value_type> il)

    // Assign
    // template <typename InputIter, enable_input_iterator_t<InputIter>* = nullptr>
    // void assign(InputIter first, InputIter last);

    // template <typename ForwardIter, enable_forward_iterable_t<ForwardIter>* = nullptr>
    // void assign(ForwardIter first, ForwardIter last);

    // void assign(size_type __n, const_reference __u);
    // void assign(std::initializer_list<value_type> il)

    // Observers
    allocator_type
    get_allocator()
    const noexcept
    {
        return allocator_type(alloc());
    }

    // TODO: Iterator one...
    // TODO

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

    reverse_iterator
    rbegin()
    noexcept
    {
        return facet().rbegin();
    }

    const_reverse_iterator
    rbegin()
    const noexcept
    {
        return facet().rbegin();
    }

    const_reverse_iterator
    crbegin()
    const noexcept
    {
        return facet().crbegin();
    }

    reverse_iterator
    rend()
    noexcept
    {
        return facet().rend();
    }

    const_reverse_iterator
    rend()
    const noexcept
    {
        return facet().rend();
    }

    const_reverse_iterator
    crend()
    const noexcept
    {
        return facet().crend();
    }

    // Capacity
    CONTAINED_CPP17_NODISCARD
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

    size_type
    capacity()
    const noexcept
    {
        return facet().capacity();
    }

    // reserve
    // shrink_to_fit

    // Element access
    reference
    at(
        size_type n
    )
    {
        return facet().at(n);
    }

    const_reference
    at(
        size_type n
    ) const
    {
        return facet().at(n);
    }

    reference
    operator[](
        size_type n
    )
    {
        return facet()[n];
    }

    const_reference
    operator[](
        size_type n
    ) const
    {
        return facet()[n];
    }

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

    value_type*
    data()
    noexcept
    {
        return facet().data();
    }

    const value_type*
    data()
    const noexcept
    {
        return facet().data();
    }

    // Modifiers
    void
    clear()
    noexcept
    {
        destruct_at_end(facet().begin_);
    }
    // TODO: insert
    // TODO: push_back
    // TODO: pop_back
    // TODO: emplace
    // TODO: resize
    // TODO: swap
    // TODO: erase

    template <typename ... Ts>
    iterator
    emplace(const_iterator pos, Ts&&... ts)
    {
        pointer p = facet().begin_ + (pos - begin());
        if (facet().end_ < facet().end_cap_) {
            if (p == facet().end_) {
                alloc_traits::construct(alloc(), to_raw_pointer(facet().end_), std::forward<Ts>(ts)...);
                ++facet().end_;
            } else {
                relocate_n(p, facet().end_, p+1);
                ++facet().end_;
                alloc_traits::construct(alloc(), to_raw_pointer(p), std::forward<Ts>(ts)...);
            }
        } else {
            allocator_type& a = alloc();
            buffer_type v(recommend(size() + 1), p - facet().begin_, a);
            v.emplace_back(std::forward<Ts>(ts)...);
            p = swap_out_circular_buffer(v, p);
        }
        return p;
    }

    iterator
    erase(const_iterator pos)
    {
        assert(pos != end() && "vector::erase(iterator) called with a non-dereferenceable iterator");
        pointer p = facet().begin_ + (pos - begin());
        relocate(p + 1, facet().end_, p);
        destruct_at_end(facet().end_ - 1);
        return p;
    }

    iterator
    erase(const_iterator first, const_iterator last)
    {
        assert(first <= last && "vector::erase(first, last) called with invalid range");
        pointer p = facet().begin_ + (first - begin());
        if (first != last) {
            difference_type ds = last - first;
            relocate(p + ds, facet().end_, p);
            destruct_at_end(facet().end_ - ds);
        }
        return p;
    }

    void
    push_back(const_reference x)
    {
        if (facet().end_ < facet().end_cap_) {
            alloc_traits::construct(alloc(), to_raw_pointer(facet().end_), x);
        } else {
            push_back_slow(x);
        }
    }

    void
    push_back(value_type&& x)
    {
        if (facet().end_ < facet().end_cap_) {
            alloc_traits::construct(alloc(), to_raw_pointer(facet().end_), move(x));
        } else {
            push_back_slow(move(x));
        }
    }

    template <typename ... Ts>
    reference
    emplace_back(Ts&&... ts)
    {
        if (facet().end_ < facet().end_cap_) {
            alloc_traits::construct(alloc(), to_raw_pointer(facet().end_), std::forward<Ts>(ts)...);
        } else {
            emplace_back_slow(std::forward<Ts>(ts)...);
        }
        return back();
    }

    void
    pop_back()
    {
        assert(!empty() && "vector::pop_back called for empty vector");
        destruct_at_end(facet().end_ - 1);
    }

    void
    resize(size_type sz)
    {
        size_type cs = size();
        if (cs < sz) {
            append(sz - cs);
        } else if (cs > sz) {
            destruct_at_end(facet().begin_ + sz);
        }
    }

    void
    resize(size_type sz, const_reference v)
    {
        size_type cs = size();
        if (cs < sz) {
            append(sz - cs, v);
        } else if (cs > sz) {
            destruct_at_end(facet().begin_ + sz);
        }
    }

    void
    swap(vector& x)
    noexcept
    {
        facet().swap(x.facet());
        swap_allocator(alloc(), x.alloc());
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
    compressed_pair<facet_type, allocator_type> data_;

    // Allocator
    allocator_type&
    alloc()
    noexcept
    {
        return get<1>(data_);
    }

    const allocator_type&
    alloc()
    const noexcept
    {
        return get<1>(data_);
    }

    // Allocation
    void
    allocate(size_type n)
    {
        if (n > max_size()) {
            throw std::length_error("vector");
        }
        facet().begin_ = facet().end_ = alloc_traits::allocate(alloc(), n);
        facet().end_cap_ = facet().begin_ + n;
    }

    size_type
    recommend(size_type new_size)
    {
        // get ratio properties
        constexpr unsigned num = CONTAINED_GROWTH_FACTOR_NUMERATOR;
        constexpr unsigned den = CONTAINED_GROWTH_FACTOR_DENOMINATOR;
        constexpr double ratio = static_cast<double>(num) / den;

        // check max size
        size_type ms = max_size();
        if (new_size > ms) {
            throw std::length_error("vector");
        }

        // check with ideal growth rate
        const size_type cap = capacity();
        if (cap >= ms / ratio) {
            return ms;
        }
        return std::max<size_type>(ratio*cap, new_size);
    }

    // TODO: need assign allocs...

    // Object destruction
    void
    destruct_at_end(pointer new_last, std::true_type)
    noexcept
    {
        facet().end_ = new_last;
    }

    void
    destruct_at_end(pointer new_last, std::false_type)
    noexcept
    {
        pointer soon_to_be_end = facet().end_;
        while (new_last != soon_to_be_end) {
            alloc_traits::destroy(alloc(), to_raw_pointer(--soon_to_be_end));
        }
        facet().end_ = new_last;
    }

    void
    destruct_at_end(pointer new_last)
    noexcept
    {
        using bool_type = std::integral_constant<
            bool,
            std::is_trivial<value_type>::value || std::is_empty<value_type>::value
        >;
        destruct_at_end(new_last, bool_type());
    }

    // Object construction
    void
    construct_at_end(size_type n)
    {
        allocator_type& a = alloc();
        do {
            alloc_traits::construct(a, to_raw_pointer(facet().end_));
            ++facet().end_;
            --n;
        } while (n > 0);
    }

    void
    construct_at_end(size_type n, const_reference v)
    {
        allocator_type& a = alloc();
        do {
            alloc_traits::construct(a, to_raw_pointer(facet().end_), v);
            ++facet().end_;
            --n;
        } while (n > 0);
    }

    void
    append(size_type n)
    {
        if (static_cast<size_type>(facet().end_cap_ - facet().end_) >= n) {
            construct_at_end(n);
        } else {
            allocator_type& a = alloc();
            buffer_type v(recommend(size() + n), size(), a);
            v.construct_at_end(n);
            swap_out_circular_buffer(v);
        }
    }

    void
    append(size_type n, const_reference v)
    {
        if (static_cast<size_type>(facet().end_cap_ - facet().end_) >= n) {
            construct_at_end(n, v);
        } else {
            allocator_type& a = alloc();
            buffer_type v(recommend(size() + n), size(), a);
            v.construct_at_end(n, v);
            swap_out_circular_buffer(v);
        }
    }

    // Swap out circular buffer
    void
    swap_out_circular_buffer(buffer_type& v)
    {
        // construct backward calls memmove, if relocatable
        alloc_traits::construct_backward(alloc(), facet().begin_, facet().end_, v.facet().begin_);
        std::swap(facet().begin_, v.facet().begin_);
        std::swap(facet().end_, v.facet().end_);
        std::swap(facet().end_cap_, v.facet().end_cap_);
        v.first_ = v.begin_;
        alloc_traits::deallocate(alloc(), v.first_, v.capacity());
    }

    pointer
    swap_out_circular_buffer(buffer_type& v, pointer p)
    {
        // construct backward calls memmove, if relocatable
        pointer r = v.facet().begin_;
        alloc_traits::construct_backward(alloc(), facet().begin_, p, v.facet().begin_);
        alloc_traits::construct_forward(alloc(), p, facet().end_, v.facet().end_);
        std::swap(facet().begin_, v.facet().begin_);
        std::swap(facet().end_, v.facet().end_);
        std::swap(facet().end_cap_, v.facet().end_cap_);
        v.first_ = v.begin_;
        alloc_traits::deallocate(alloc(), v.first_, v.capacity());
        return r;
    }

    // Push back
    template <typename U>
    void
    push_back_slow(U&& x)
    {
        allocator_type& a = alloc();
        buffer_type v(recommend(size() + 1), size(), a);
        alloc_traits::construct(a, to_raw_pointer(v.facet().end_), std::forward<U>(x));
        v.facet().end_++;
        swap_out_circular_buffer(v);
    }

    template <typename ... Ts>
    void
    emplace_back_slow(Ts&&... ts)
    {
        allocator_type& a = alloc();
        buffer_type v(recommend(size() + 1), size(), a);
        alloc_traits::construct(a, to_raw_pointer(v.facet().end_), std::forward<Ts>(ts)...);
        v.facet().end_++;
        swap_out_circular_buffer(v);
    }
};

}   /* contained */
