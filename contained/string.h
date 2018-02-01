//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \synopsis
 // TODO: implement.
 */

#pragma once

#include <contained/string_view.h>
#include <contained/detail/compressed_pair.h>
#include <contained/detail/growth.h>
#include <contained/detail/swap_allocator.h>
#include <contained/detail/util.h>
#include <string>
#include <type_traits>

namespace contained
{
namespace
{

template <typename Char, typename Traits, typename T>
struct string_view_convertible: std::integral_constant<
        bool,
        (
            std::is_convertible<const T&, basic_string_view<Char, Traits> >::value &&
            !std::is_convertible<const T&, const Char*>::value
        )
    >
{};

template <typename Char, typename Traits, typename T>
using enable_string_view_convertible = typename std::enable_if<
    string_view_convertible<Char, Traits, T>::value
>::type;

}   /* anonymous */

// OBJECTS
// -------

// STRING FACET

template <
    typename Char,
    typename TraitsType = std::char_traits<Char>,
    typename VoidPtr = void*
>
class basic_string_facet
{
protected:
    using view_type = basic_string_view<Char, TraitsType>;

    template <typename T>
    using enable_sv_convertible = enable_string_view_convertible<Char, TraitsType, T>;

public:
    using value_type = Char;
    using traits_type = TraitsType;
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

    static constexpr size_type npos = -1;

    // Constructors
    basic_string_facet()
    {
        zero();
    }
    // Iterators
    iterator
    begin()
    noexcept
    {
        return iterator(get_pointer());
    }

    const_iterator
    begin()
    const noexcept
    {
        return const_iterator(get_pointer());
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
        return begin() + size();
    }

    const_iterator
    end()
    const noexcept
    {
        return begin() + size();
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
    size_type
    size()
    const noexcept
    {
        return is_long() ? get_long_size() : get_short_size();
    }

    size_type
    length()
    const noexcept
    {
        return size();
    }

    size_type
    max_size()
    const noexcept
    {
        constexpr size_type m = std::numeric_limits<size_type>::max();
#if BYTE_ORDER == BIG_ENDIAN
        return (m <= ~long_mask ? m : m/2) - alignment;
#else
        return m - alignment;
#endif
    }

    size_type
    capacity()
    const noexcept
    {
        return (is_long() ? get_long_cap() : static_cast<size_type>(min_cap)) - 1;
    }

    CONTAINED_CPP17_NODISCARD
    bool
    empty()
    const noexcept
    {
        return size() == 0;
    }

    // Element access
    reference
    operator[](size_type pos)
    {
        assert(pos <= size() && "string index out of bounds");
        return *(get_pointer() + pos);
    }

    const_reference
    operator[](size_type pos)
    const
    {
        assert(pos <= size() && "string index out of bounds");
        return *(data() + pos);
    }

    reference
    at(size_type pos)
    {
        if (pos > size()) {
            throw std::out_of_range("string::at");
        }
        return operator[](pos);
    }

    const_reference
    at(size_type pos)
    const
    {
        if (pos > size()) {
            throw std::out_of_range("string::at");
        }
        return operator[](pos);
    }

    reference
    front()
    {
        assert(!empty() && "basic_string::front(): string is empty");
        return *get_pointer();
    }

    const_reference
    front()
    const
    {
        assert(!empty() && "basic_string::front(): string is empty");
        return *data();
    }

    reference
    back()
    {
        assert(!empty() && "basic_string::front(): string is empty");
        return *(get_pointer() + size() - 1);
    }

    const_reference
    back()
    const
    {
        assert(!empty() && "basic_string::back(): string is empty");
        return *(data() + size() - 1);
    }

    const_pointer
    c_str()
    const noexcept
    {
        return data();
    }

    pointer
    data()
    noexcept
    {
        return get_pointer();
    }

    const_pointer
    data()
    const noexcept
    {
        return get_pointer();
    }

    // Compare
    int
    compare(const basic_string_facet& x)
    const noexcept
    {
        return compare(view_type(x));
    }

    int
    compare(view_type s)
    const noexcept
    {
        return view_type(*this).compare(s);
    }

    int
    compare(size_type pos1, size_type n1, const basic_string_facet& s)
    const
    {
        return compare(pos1, n1, view_type(s));
    }

    int compare(size_type pos1, size_type n1, view_type s)
    const
    {
        return view_type(*this).compare(pos1, n1, s);
    }

    int
    compare(size_type pos1, size_type n1, const basic_string_facet& s, size_type pos2, size_type n2 = npos)
    const
    {
        return compare(pos1, n1, view_type(s), pos2, n2);
    }

    template <typename T, enable_sv_convertible<T>* = nullptr>
    int
    compare(size_type pos1, size_type n1, const T& t, size_type pos2, size_type n2 = npos)
    const
    {
        return compare(pos1, n1, view_type(t), pos2, n2);
    }

    int
    compare(const_pointer s)
    const noexcept
    {
        return compare(view_type(s));
    }

    int
    compare(size_type pos1, size_type n1, const_pointer s)
    const
    {
        return compare(pos1, n1, view_type(s));
    }

    int
    compare(size_type pos1, size_type n1, const_pointer s, size_type n2)
    const
    {
        return compare(pos1, n1, view_type(s, n2));
    }

    // Find
    size_type
    find(const basic_string_facet& str, size_type pos = 0)
    const noexcept
    {
        return find(view_type(str), pos);
    }

    size_type
    find(view_type sv, size_type pos = 0)
    const noexcept
    {
        return view_type(*this).find(sv, pos);
    }

    size_type
    find(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return find(view_type(s, n), pos);
    }

    size_type
    find(const_pointer s, size_type pos = 0)
    const noexcept
    {
        return find(view_type(s), pos);
    }

    size_type
    find(value_type c, size_type pos = 0)
    const noexcept
    {
        return find(view_type(&c, 1), pos);
    }

    // Rfind
    size_type
    rfind(const basic_string_facet& str, size_type pos = 0)
    const noexcept
    {
        return rfind(view_type(str), pos);
    }

    size_type
    rfind(view_type sv, size_type pos = 0)
    const noexcept
    {
        return view_type(*this).rfind(sv, pos);
    }

    size_type
    rfind(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return rfind(view_type(s, n), pos);
    }

    size_type
    rfind(const_pointer s, size_type pos = 0)
    const noexcept
    {
        return rfind(view_type(s), pos);
    }

    size_type
    rfind(value_type c, size_type pos = 0)
    const noexcept
    {
        return rfind(view_type(&c, 1), pos);
    }

    // Find First Of
    size_type
    find_first_of(const basic_string_facet& str, size_type pos = 0)
    const noexcept
    {
        return find_first_of(view_type(str), pos);
    }

    size_type
    find_first_of(view_type sv, size_type pos = 0)
    const noexcept
    {
        return view_type(*this).find_first_of(sv, pos);
    }

    size_type
    find_first_of(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return find_first_of(view_type(s, n), pos);
    }

    size_type
    find_first_of(const_pointer s, size_type pos = 0)
    const noexcept
    {
        return find_first_of(view_type(s), pos);
    }

    size_type
    find_first_of(value_type c, size_type pos = 0)
    const noexcept
    {
        return find_first_of(view_type(&c, 1), pos);
    }

    size_type
    find_last_of(const basic_string_facet& str, size_type pos = 0)
    const noexcept
    {
        return find_last_of(view_type(str), pos);
    }

    size_type
    find_last_of(view_type sv, size_type pos = 0)
    const noexcept
    {
        return view_type(*this).find_last_of(sv, pos);
    }

    size_type
    find_last_of(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return find_last_of(view_type(s, n), pos);
    }

    size_type
    find_last_of(const_pointer s, size_type pos = 0)
    const noexcept
    {
        return find_last_of(view_type(s), pos);
    }

    size_type
    find_last_of(value_type c, size_type pos = 0)
    const noexcept
    {
        return find_last_of(view_type(&c, 1), pos);
    }

    // Find First Not Of
    size_type
    find_first_not_of(const basic_string_facet& str, size_type pos = 0)
    const noexcept
    {
        return find_first_not_of(view_type(str), pos);
    }

    size_type
    find_first_not_of(view_type sv, size_type pos = 0)
    const noexcept
    {
        return view_type(*this).find_first_not_of(sv, pos);
    }

    size_type
    find_first_not_of(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return find_first_not_of(view_type(s, n), pos);
    }

    size_type
    find_first_not_of(const_pointer s, size_type pos = 0)
    const noexcept
    {
        return find_first_not_of(view_type(s), pos);
    }

    size_type
    find_first_not_of(value_type c, size_type pos = 0)
    const noexcept
    {
        return find_first_not_of(view_type(&c, 1), pos);
    }

    // Find Last Not Of
    size_type
    find_last_not_of(const basic_string_facet& str, size_type pos = 0)
    const noexcept
    {
        return find_last_not_of(view_type(str), pos);
    }

    size_type
    find_last_not_of(view_type sv, size_type pos = 0)
    const noexcept
    {
        return view_type(*this).find_last_not_of(sv, pos);
    }

    size_type
    find_last_not_of(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return find_last_not_of(view_type(s, n), pos);
    }

    size_type
    find_last_not_of(const_pointer s, size_type pos = 0)
    const noexcept
    {
        return find_last_not_of(view_type(s), pos);
    }

    size_type
    find_last_not_of(value_type c, size_type pos = 0)
    const noexcept
    {
        return find_last_not_of(view_type(&c, 1), pos);
    }

    // Starts with
    bool
    starts_with(view_type sv)
    const noexcept
    {
        return view_type(*this).starts_with(sv);
    }

    bool
    starts_with(value_type c)
    const noexcept
    {
        return view_type(*this).starts_with(c);
    }

    bool
    starts_with(const_pointer s)
    const
    {
        return view_type(*this).starts_with(s);
    }

    // Ends with
    bool
    ends_with(view_type sv)
    const noexcept
    {
        return view_type(*this).ends_with(sv);
    }

    bool
    ends_with(value_type c)
    const noexcept
    {
        return view_type(*this).ends_with(c);
    }

    bool
    ends_with(const_pointer s)
    const
    {
        return view_type(*this).ends_with(s);
    }

    // Conversion
    operator view_type()
    const
    {
        return view_type(data(), size());
    }

protected:
    template <typename, typename, typename> friend class basic_string;

    // Short-string optimization
    struct long_layout
    {
        size_type cap_;
        size_type size_;
        pointer data_;
    };

#if BYTE_ORDER == BIG_ENDIAN
    static constexpr size_type short_mask = 0x80;
    static constexpr size_type long_mask  = ~(size_type(~0) >> 1);
#else   // BYTE_ORDER != BIG_ENDIAN
    static constexpr size_type short_mask = 0x01;
    static constexpr size_type long_mask  = 0x1ul;
#endif  // BYTE_ORDER == BIG_ENDIAN

    enum {
        min_cap = (sizeof(long_layout) - 1)/ sizeof(value_type) > 2 ?
                   (sizeof(long_layout) - 1)/sizeof(value_type) : 2
    };

    struct short_layout
    {
        union
        {
            unsigned char size_;
            value_type lx;
        };
        value_type data_[min_cap];
    };

    union ulx
    {
        long_layout lx;
        short_layout lxx;
    };

    enum {
        n_words = sizeof(ulx) / sizeof(size_type)
    };

    struct raw
    {
        size_type words[n_words];
    };

    struct rep
    {
        union
        {
            long_layout l;
            short_layout s;
            raw r;
        };
    };

    rep r_;

    // Constructors
    basic_string_facet(const basic_string_facet&) = delete;

    basic_string_facet(basic_string_facet&& x)
    noexcept:
        r_(std::move(x.r_))
    {
        x.zero();
    }

    // Assignment
    basic_string_facet& operator=(const basic_string_facet&) = delete;

    basic_string_facet&
    operator=(basic_string_facet&& x)
    {
        swap(x);
        return *this;
    }

    // Modifiers
    void
    swap(basic_string_facet& x)
    {
        std::swap(r_, x.r_);
    }

    // Zero
    void
    zero()
    noexcept
    {
        size_type (&a)[n_words] = r_.r.words;
        for (unsigned i = 0; i < n_words; ++i) {
            a[i] = 0;
        }
    }

    // Is long
    bool
    is_long()
    const noexcept
    {
        return static_cast<bool>(r_.s.size_ & short_mask);
    }

    // Get long cap
    size_type
    get_long_cap()
    const noexcept
    {
        return r_.l.cap_ & size_type(~long_mask);
    }

    // Get long size
    size_type
    get_long_size()
    const noexcept
    {
        return r_.l.size_;
    }

    // Set long size
    void
    set_long_size(size_type s)
    const noexcept
    {
        r_.l.size_ = s;
    }

    // Get short size
    size_type
    get_short_size()
    const noexcept
    {
#if BYTE_ORDER == BIG_ENDIAN
        return r_.s.size_;
#else
        return r_.s.size_ >> 1;
#endif
    }

    // Set short size
    void
    set_short_size(size_type s)
    const noexcept
    {
#if BYTE_ORDER == BIG_ENDIAN
        r_.s.size_ = static_cast<unsigned char>(s);
#else
        r_.s.size_ = static_cast<unsigned char>(s << 1);
#endif
    }

    // Set size
    void
    set_size(size_type s)
    {
        if (is_long()) {
            set_long_size(s);
        } else {
            set_short_size(s);
        }
    }

    // Get long pointer
    pointer
    get_long_pointer()
    noexcept
    {
        return r_.l.data_;
    }

    const_pointer
    get_long_pointer()
    const noexcept
    {
        return r_.l.data_;
    }

    // Get short pointer
    pointer
    get_short_pointer()
    noexcept
    {
        return std::pointer_traits<pointer>::pointer_to(r_.s.data_[0]);
    }

    const_pointer
    get_short_pointer()
    const noexcept
    {
        return std::pointer_traits<const_pointer>::pointer_to(r_.s.data_[0]);
    }

    // Get pointer
    pointer
    get_pointer()
    noexcept
    {
        return is_long() ? get_long_pointer() : get_short_pointer();
    }

    const_pointer
    get_pointer()
    const noexcept
    {
        return is_long() ? get_long_pointer() : get_short_pointer();
    }

    template <size_type N>
    static
    size_type
    align_it(size_type s)
    noexcept
    {
        return (s + (N-1)) & ~(N-1);
    }

    enum {
        alignment = 16
    };

    // Recommend
    static
    size_type
    recommend(size_type s)
    noexcept
    {
        constexpr size_type n = sizeof(value_type) < alignment ? alignment / sizeof(value_type) : 1;
        size_type l = s < min_cap ? static_cast<size_type>(min_cap) : align_it<n>(s + 1);
        return l - 1;
    }
};

// Relative Operators
template <typename Char, typename Traits>
inline
bool
operator==(
    const basic_string_facet<Char, Traits>& lhs,
    const basic_string_facet<Char, Traits>& rhs
)
noexcept
{
    size_t lhs_size = lhs.size();
    return lhs_size == rhs.size() && Traits::compare(lhs.data(), rhs.data(), lhs_size) == 0;
}


template <typename Char, typename Traits>
inline
bool
operator==(
    const Char* lhs,
    const basic_string_facet<Char, Traits>& rhs)
{
    return basic_string_view<Char, Traits>(lhs) == rhs;
}


template <typename Char, typename Traits>
inline
bool
operator==(
    const basic_string_facet<Char, Traits>& lhs,
    const Char* rhs)
{
    return lhs == basic_string_view<Char, Traits>(rhs);
}



template <typename Char, typename Traits>
inline
bool
operator!=(
    const basic_string_facet<Char, Traits>& lhs,
    const basic_string_facet<Char, Traits>& rhs
)
noexcept
{

    return !(lhs == rhs);
}


template <typename Char, typename Traits>
inline
bool
operator!=(
    const Char* lhs,
    const basic_string_facet<Char, Traits>& rhs)
{
    return !(lhs == rhs);
}


template <typename Char, typename Traits>
inline
bool
operator!=(
    const basic_string_facet<Char, Traits>& lhs,
    const Char* rhs)
{
    return !(lhs == rhs);
}


template <typename Char, typename Traits>
inline
bool
operator<(
    const basic_string_facet<Char, Traits>& lhs,
    const basic_string_facet<Char, Traits>& rhs
)
noexcept
{
    return lhs.compare(rhs) < 0;
}


template <typename Char, typename Traits>
inline
bool
operator<(
    const Char* lhs,
    const basic_string_facet<Char, Traits>& rhs)
{
    return basic_string_view<Char, Traits>(lhs) < rhs;
}


template <typename Char, typename Traits>
inline
bool
operator<(
    const basic_string_facet<Char, Traits>& lhs,
    const Char* rhs)
{
    return lhs < basic_string_view<Char, Traits>(rhs);
}


template <typename Char, typename Traits>
inline
bool
operator<=(
    const basic_string_facet<Char, Traits>& lhs,
    const basic_string_facet<Char, Traits>& rhs
)
noexcept
{
    return !(rhs < lhs);
}


template <typename Char, typename Traits>
inline
bool
operator<=(
    const Char* lhs,
    const basic_string_facet<Char, Traits>& rhs
)
{
    return basic_string_view<Char, Traits>(lhs) <= rhs;
}


template <typename Char, typename Traits>
inline
bool
operator<=(
    const basic_string_facet<Char, Traits>& lhs,
    const Char* rhs
)
{
    return lhs <= basic_string_view<Char, Traits>(rhs);
}



template <typename Char, typename Traits>
inline
bool
operator>(
    const basic_string_facet<Char, Traits>& lhs,
    const basic_string_facet<Char, Traits>& rhs
)
noexcept
{
    return rhs < lhs;
}


template <typename Char, typename Traits>
inline
bool
operator>(
    const Char* lhs,
    const basic_string_facet<Char, Traits>& rhs)
{
    return basic_string_view<Char, Traits>(lhs) > rhs;
}


template <typename Char, typename Traits>
inline
bool
operator>(
    const basic_string_facet<Char, Traits>& lhs,
    const Char* rhs)
{
    return lhs > basic_string_view<Char, Traits>(rhs);
}


template <typename Char, typename Traits>
inline
bool
operator>=(
    const basic_string_facet<Char, Traits>& lhs,
    const basic_string_facet<Char, Traits>& rhs
)
noexcept
{
    return !(lhs < rhs);
}


template <typename Char, typename Traits>
inline
bool
operator>=(
    const Char* lhs,
    const basic_string_facet<Char, Traits>& rhs)
{
    return basic_string_view<Char, Traits>(lhs) >= rhs;
}


template <typename Char, typename Traits>
inline
bool
operator>=(
    const basic_string_facet<Char, Traits>& lhs,
    const Char* rhs)
{
    return lhs >= basic_string_view<Char, Traits>(rhs);
}

// TODO: friend functions

// STRING

template <
    typename Char,
    typename TraitsType = std::char_traits<Char>,
    typename Allocator = std::allocator<Char>
>
class basic_string
{
protected:
    using alloc_traits = std::allocator_traits<Allocator>;
    using void_pointer = typename alloc_traits::void_pointer;
    using view_type = basic_string_view<Char, TraitsType>;

    template <typename T>
    using enable_sv_convertible = enable_string_view_convertible<Char, TraitsType, T>;

public:
    using value_type = Char;
    using traits_type = TraitsType;
    using allocator_type = Allocator;
    using facet_type = basic_string_facet<Char, TraitsType, void_pointer>;
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

    static constexpr size_type npos = -1;

    // Constructors
    basic_string()
    noexcept
    {}

    explicit
    basic_string(const allocator_type& alloc)
    noexcept:
        data_(alloc)
    {}

    basic_string(
        const basic_string& x,
        size_type pos,
        const allocator_type& alloc = allocator_type()
    ):
        basic_string(alloc)
    {
        size_type x_sz = x.size();
        if (pos > x_sz) {
            throw std::out_of_range("basic_string");
        }
        init(x.data() + pos, x_sz - pos);
    }

    basic_string(
        const basic_string& x,
        size_type pos,
        size_type n,
        const allocator_type& alloc = allocator_type()
    ):
        basic_string(alloc)
    {
        size_type x_sz = x.size();
        if (pos > x_sz) {
            throw std::out_of_range("basic_string");
        }
        init(x.data() + pos, std::min(n, x_sz - pos));
    }

    template <typename T, enable_sv_convertible<T>* = nullptr>
    basic_string(
        const T& t,
        size_type pos,
        size_type n,
        const allocator_type& alloc = allocator_type()
    ):
        basic_string(alloc)
    {
        view_type sv = view_type(t).substr(pos, n);
        init(sv.data(), sv.size());
    }

    explicit
    basic_string(const view_type sv):
        basic_string()
    {
        init(sv.data(), sv.size());
    }

    basic_string(const view_type sv, const allocator_type& alloc):
        basic_string(alloc)
    {
        init(sv.data(), sv.size());
    }

    basic_string(const_pointer s):
        basic_string()
    {
        assert(s != nullptr && "basic_string(const char*, allocator) detected nullptr");
        init(s, traits_type::length(s));
    }

    basic_string(const_pointer s, const allocator_type& alloc):
        basic_string(alloc)
    {
        assert(s != nullptr && "basic_string(const char*, allocator) detected nullptr");
        init(s, traits_type::length(s));
    }

    basic_string(const_pointer s, size_type n):
        basic_string()
    {
        assert((n == 0 || s != nullptr) && "basic_string(const char*, n) detected nullptr");
        init(s, n);
    }

    basic_string(const_pointer s, size_type n, const allocator_type& alloc):
        basic_string(alloc)
    {
        assert((n == 0 || s != nullptr) && "basic_string(const char*, n) detected nullptr");
        init(s, n);
    }

    basic_string(size_type n, value_type c):
        basic_string()
    {
        init(n, c);
    }

    basic_string(size_type n, value_type c, const allocator_type& alloc):
        basic_string(alloc)
    {
        init(n, c);
    }

    template <typename InputIter>
    basic_string(InputIter first, InputIter last):
        basic_string()
    {
        init(first, last);
    }

    template <typename InputIter>
    basic_string(InputIter first, InputIter last, const allocator_type& alloc):
        basic_string(alloc)
    {
        init(first, last);
    }

    basic_string(std::initializer_list<value_type> il)
    {
        init(il.begin(), il.end());
    }

    basic_string(std::initializer_list<value_type> il, const allocator_type& alloc):
        basic_string(alloc)
    {
        init(il.begin(), il.end());
    }

    // Copy constructors
    basic_string(const basic_string& x):
        basic_string(alloc_traits::select_on_container_copy_construction(x.alloc()))
    {
        if (!x.facet().is_long()) {
            facet().r_.r = x.facet().r_.r;
        } else {
            init(to_raw_pointer(x.facet().get_long_pointer()), x.facet().get_long_size());
        }
    }

    basic_string(const basic_string& x, const allocator_type& alloc):
        basic_string(alloc)
    {
        if (!x.facet().is_long()) {
            facet().r_.r = x.facet().r_.r;
        } else {
            init(to_raw_pointer(x.facet().get_long_pointer()), x.facet().get_long_size());
        }
    }

    // Move constructors
    basic_string(basic_string&& x)
    noexcept:
        data_(std::move(x.facet(), std::move(x.alloc())))
    {
        x.zero();
    }

    basic_string(basic_string&& x, const allocator_type& alloc)
    noexcept:
        data_(std::move(x.facet(), alloc))
    {
        x.zero();
    }

    // Destructors
    ~basic_string()
    {
        deallocate();
    }

    // Assignment
    basic_string&
    operator=(const basic_string& x)
    {
        if (this != &x) {
            copy_assign_alloc(x);
            assign(x.data(), x.size());
        }
        return *this;
    }

    basic_string&
    operator=(basic_string&& x)
    noexcept
    {
        move_assign(x);
        return *this;
    }

    basic_string&
    operator=(view_type x)
    {
        return assign(x);
    }

    basic_string&
    operator=(const_pointer s)
    {
        return assign(s);
    }

    basic_string&
    operator=(value_type c)
    {
        pointer p;
        if (facet().is_long()) {
            p = facet().get_long_pointer();
            facet().set_long_size(1);
        } else {
            p = facet().get_short_pointer();
            facet().set_short_size(1);
        }
        traits_type::assign(*p, c);
        traits_type::assign(*++p, value_type());

        return *this;
    }

    basic_string&
    operator=(std::initializer_list<value_type> il)
    {
        return assign(il.begin(), il.end());
    }

    // Assign
    basic_string&
    assign(view_type x)
    {
        return assign(x.data(), x.size());
    }

    basic_string& assign(const basic_string& x)
    {
        return *this = x;
    }

    basic_string&
    assign(basic_string&& x)
    noexcept
    {
        *this = std::move(x);
        return *this;
    }

    basic_string&
    assign(const basic_string& str, size_type pos, size_type n = npos)
    {
        size_type sz = str.size();
        if (pos > sz) {
            throw std::out_of_range("basic_string");
        }
        return assign(str.data() + pos, std::min(n, sz - pos));
    }

    template <typename T, enable_sv_convertible<T>* = nullptr>
    basic_string&
    asign(const T& t, size_type pos, size_type n = npos)
    {
        view_type sv = t;
        size_type sz = sv.size();
        if (pos > sz) {
            throw std::out_of_range("basic_string");
        }
        return assign(sv.data() + pos, std::min(n, sz - pos));
    }

    basic_string&
    assign(const_pointer s, size_type n)
    {
        assert((n == 0 || s != nullptr) && "string::assign received nullptr");
        size_type cap = capacity();
        if (cap >= n) {
            value_type* p = to_raw_pointer(facet().get_pointer());
            traits_type::move(p, s, n);
            traits_type::assign(p[n], value_type());
            facet().set_size(n);
        } else {
            size_type sz = size();
            grow_by_and_replace(cap, n - cap, sz, 0, sz, n, s);
        }
        return *this;
    }

    basic_string&
    assign(const_pointer s)
    {
        assert(s != nullptr && "string::assign received nullptr");
        return assign(s, traits_type::length(s));
    }

    basic_string&
    assign(size_type n, value_type c)
    {
        size_type cap = capacity();
        if (cap < n) {
            size_type sz = size();
            grow_by(cap, n - cap, sz, 0, sz);
        }
        value_type* p = to_raw_pointer(facet().get_pointer());
        traits_type::assign(p, n, c);
        traits_type::assign(p[n], value_type());
        facet().set_size(n);

        return *this;
    }

    template <typename InputIter, enable_input_iterator_t<InputIter>* = nullptr>
    basic_string&
    assign(InputIter first, InputIter last)
    {
        const basic_string tmp(first, last, alloc());
        assign(tmp.data(), tmp.size());
        return *this;
    }

    template <typename ForwardIter, enable_input_iterable_t<ForwardIter>* = nullptr>
    basic_string&
    assign(ForwardIter first, ForwardIter last)
    {
        size_type n = static_cast<size_type>(std::distance(first, last));
        size_type cap = capacity();
        if (cap < n) {
            size_type sz = size();
            grow_by(cap, n - cap, sz, 0, sz);
        }
        pointer p = facet().get_pointer();
        for (; first != last; ++first, ++p) {
            traits_type::assign(*p, *first);
        }
        traits_type::assign(*p, value_type());
        facet().set_size(n);
        return *this;
    }

    basic_string&
    assign(std::initializer_list<value_type> il)
    {
        return assign(il.begin(), il.size());
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
    size_type
    size()
    const noexcept
    {
        return facet().size();
    }

    size_type
    length()
    const noexcept
    {
        return facet().length();
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

    CONTAINED_CPP17_NODISCARD
    bool
    empty()
    const noexcept
    {
        return facet().empty();
    }

    void
    reserve(size_type size = 0)
    {
        if (size > max_size()) {
            throw std::length_error("basic_string");
        }
        size_type cap = capacity();
        size_type sz = size();
        size = std::max(size, sz);
        size = recommend(size);

        if (size != cap) {
            pointer new_data, p;
            bool was_long, now_long;
            if (size == facet_type::min_cap - 1) {
                was_long = true;
                now_long = false;
                new_data = facet().get_short_pointer();
                p = facet().get_long_pointer();
            } else {
                if (size > cap) {
                    new_data = alloc_traits::allocate(alloc(), size+1);
                } else {
                    try {
                        new_data = alloc_traits::allocate(alloc(), size+1);
                    } catch (...) {
                        return;
                    }
                }
                now_long = true;
                was_long = facet().is_long();
                p = facet().get_pointer();
            }
            traits_type::copy(to_raw_pointer(new_data), to_raw_pointer(p), size()+1);
            if (was_long) {
                alloc_traits::deallocate(alloc(), p, cap+1);
            }
            if (now_long) {
                facet().set_long_cap(size+1);
                facet().set_long_size(sz);
                facet().set_long_pointer(new_data);
            } else {
                facet().set_short_size(sz);
            }
        }
    }

    void
    shrink_to_fit()
    noexcept
    {
        reserve();
    }

    // Element access
    reference
    operator[](size_type pos)
    {
        return facet()[pos];
    }

    const_reference
    operator[](size_type pos)
    const
    {
        return facet()[pos];
    }

    reference
    at(size_type pos)
    {
        return facet().at(pos);
    }

    const_reference
    at(size_type pos)
    const
    {
        return facet().at(pos);
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

    const_pointer
    c_str()
    const noexcept
    {
        return facet().c_str();
    }

    pointer
    data()
    noexcept
    {
        return facet().data();
    }

    const_pointer
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
        if (facet().is_long()) {
            traits_type::assign(*facet().get_long_pointer(), value_type());
            facet().set_long_size(0);
        } else {
            traits_type::assign(*facet().get_short_pointer(), value_type());
            facet().set_short_size(0);
        }
    }

    // TODO: add here
    // insert
    // erase

    void
    push_back(value_type c)
    {
        bool is_short = !facet().is_long();
        size_type cap;
        size_type sz;
        if (is_short) {
            cap = facet_type::min_cap - 1;
            sz = facet().get_short_size();
        } else {
            cap = facet().get_long_cap() - 1;
            sz = facet().get_long_size();
        }
        if (sz == cap) {
            grow_by(cap, 1, sz, sz, 0);
            is_short = !facet().is_long();
        }
        pointer p;
        if (is_short) {
            p = facet().get_short_pointer() + sz;
            facet().set_short_size(sz+1);
        } else {
            p = facet().get_long_pointer() + sz;
            facet().set_long_size(sz+1);
        }
        traits_type::assign(*p, c);
        traits_type::assign(*++p, value_type());
    }

    void
    pop_back()
    {
        size_type sz;
        if (facet().is_long()) {
            sz = facet().get_long_size() - 1;
            facet().set_long_size(sz);
            traits_type::assign(*(facet().get_long_pointer() + sz), value_type());
        } else {
            sz = facet().get_short_size() - 1;
            facet().set_short_size(sz);
            traits_type::assign(*(facet().get_short_pointer() + sz), value_type());
        }
    }

    // TODO: add here
    // append
    // operator+=
    // replace

    basic_string
    substr(size_type pos, size_type n = npos)
    {
        return basic_string(*this, pos, n, alloc());
    }

    size_type
    copy(pointer dst, size_type count, size_type pos = 0)
    const
    {
        return view_type(*this).copy(dst, count, pos);
    }

    void
    resize(size_type n)
    {
        resize(n, value_type());
    }

    void
    resize(size_type n, value_type c)
    {
        size_type sz = size();
        if (n > sz) {
            append(n - sz, c);
        } else {
            erase_to_end(n);
        }
    }

    void
    swap(basic_string& x)
    {
        assert(
            (
                alloc_traits::propagate_on_container_swap::value ||
                alloc_traits::is_always_equal::value ||
                alloc() == x.alloc()
            ) &&
            "swapping non-equal allocators"
        );
        std::swap(facet().r_, x.facet().r_);
        swap_allocator(alloc(), x.alloc());
    }

    // Compare
    int
    compare(const basic_string& x)
    const noexcept
    {
        return facet().compare(x.facet());
    }

    int
    compare(view_type s)
    const noexcept
    {
        return facet().compare(s);
    }

    int
    compare(size_type pos1, size_type n1, const basic_string& s)
    const
    {
        return facet().compare(pos1, n1, s.facet());
    }

    int compare(size_type pos1, size_type n1, view_type s)
    const
    {
        return facet().compare(pos1, n1, s);
    }

    int
    compare(size_type pos1, size_type n1, const basic_string& s, size_type pos2, size_type n2 = npos)
    const
    {
        return facet().compare(pos1, n1, s.facet(), pos2, n2);
    }

    template <typename T, enable_sv_convertible<T>* = nullptr>
    int
    compare(size_type pos1, size_type n1, const T& t, size_type pos2, size_type n2 = npos)
    const
    {
        return facet().compare(pos1, n1, t, pos2, n2);
    }

    int
    compare(const_pointer s)
    const noexcept
    {
        return facet().compare(s);
    }

    int
    compare(size_type pos1, size_type n1, const_pointer s)
    const
    {
        return facet().compare(pos1, n1, s);
    }

    int
    compare(size_type pos1, size_type n1, const_pointer s, size_type n2)
    const
    {
        return facet().compare(pos1, n1, s, n2);
    }

    // Find
    size_type
    find(const basic_string& str, size_type pos = 0)
    const noexcept
    {
        return facet().find(str.facet(), pos);
    }

    size_type
    find(view_type sv, size_type pos = 0)
    const noexcept
    {
        return facet().find(sv, pos);
    }

    size_type
    find(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return facet().find(s, pos, n);
    }

    size_type
    find(const_pointer s, size_type pos = 0)
    const noexcept
    {
        return facet().find(s, pos);
    }

    size_type
    find(value_type c, size_type pos = 0)
    const noexcept
    {
        return facet().find(c, pos);
    }

    // Rfind
    size_type
    rfind(const basic_string& str, size_type pos = 0)
    const noexcept
    {
        return facet().rfind(str.facet(), pos);
    }

    size_type
    rfind(view_type sv, size_type pos = 0)
    const noexcept
    {
        return facet().rfind(sv, pos);
    }

    size_type
    rfind(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return facet().rfind(s, pos, n);
    }

    size_type
    rfind(const_pointer s, size_type pos = 0)
    const noexcept
    {
        return facet().rfind(s, pos);
    }

    size_type
    rfind(value_type c, size_type pos = 0)
    const noexcept
    {
        return facet().rfind(c, pos);
    }

    // Find First Of
    size_type
    find_first_of(const basic_string& str, size_type pos = 0)
    const noexcept
    {
        return facet().find_first_of(str.facet(), pos);
    }

    size_type
    find_first_of(view_type sv, size_type pos = 0)
    const noexcept
    {
        return facet().find_first_of(sv, pos);
    }

    size_type
    find_first_of(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return facet().find_first_of(s, pos, n);
    }

    size_type
    find_first_of(const_pointer s, size_type pos = 0)
    const noexcept
    {
        return facet().find_first_of(s, pos);
    }

    size_type
    find_first_of(value_type c, size_type pos = 0)
    const noexcept
    {
        return facet().find_first_of(c, pos);
    }

    size_type
    find_last_of(const basic_string& str, size_type pos = 0)
    const noexcept
    {
        return facet().find_last_of(str.facet(), pos);
    }

    size_type
    find_last_of(view_type sv, size_type pos = 0)
    const noexcept
    {
        return facet().find_last_of(sv, pos);
    }

    size_type
    find_last_of(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return facet().find_last_of(s, pos, n);
    }

    size_type
    find_last_of(const_pointer s, size_type pos = 0)
    const noexcept
    {
        return facet().find_last_of(s, pos);
    }

    size_type
    find_last_of(value_type c, size_type pos = 0)
    const noexcept
    {
        return facet().find_last_of(c, pos);
    }

    // Find First Not Of
    size_type
    find_first_not_of(const basic_string& str, size_type pos = 0)
    const noexcept
    {
        return facet().find_first_not_of(str.facet(), pos);
    }

    size_type
    find_first_not_of(view_type sv, size_type pos = 0)
    const noexcept
    {
        return facet().find_first_not_of(sv, pos);
    }

    size_type
    find_first_not_of(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return facet().find_first_not_of(s, pos, n);
    }

    size_type
    find_first_not_of(const_pointer s, size_type pos = 0)
    const noexcept
    {
        return facet().find_first_not_of(s, pos);
    }

    size_type
    find_first_not_of(value_type c, size_type pos = 0)
    const noexcept
    {
        return facet().find_first_not_of(c, pos);
    }

    // Find Last Not Of
    size_type
    find_last_not_of(const basic_string& str, size_type pos = 0)
    const noexcept
    {
        return find_last_not_of(str.facet(), pos);
    }

    size_type
    find_last_not_of(view_type sv, size_type pos = 0)
    const noexcept
    {
        return facet().find_last_not_of(sv, pos);
    }

    size_type
    find_last_not_of(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return find_last_not_of(s, pos, n);
    }

    size_type
    find_last_not_of(const_pointer s, size_type pos = 0)
    const noexcept
    {
        return find_last_not_of(s, pos);
    }

    size_type
    find_last_not_of(value_type c, size_type pos = 0)
    const noexcept
    {
        return find_last_not_of(c, pos);
    }

    // Starts with
    bool
    starts_with(view_type sv)
    const noexcept
    {
        return facet().starts_with(sv);
    }

    bool
    starts_with(value_type c)
    const noexcept
    {
        return facet().starts_with(c);
    }

    bool
    starts_with(const_pointer s)
    const
    {
        return facet().starts_with(s);
    }

    // Ends with
    bool
    ends_with(view_type sv)
    const noexcept
    {
        return facet().ends_with(sv);
    }

    bool
    ends_with(value_type c)
    const noexcept
    {
        return facet().ends_with(c);
    }

    bool
    ends_with(const_pointer s)
    const
    {
        return facet().ends_with(s);
    }

    // Conversion
    operator view_type()
    const
    {
        return view_type(facet());
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

    // Deallocate
    void
    deallocate()
    {
        if (facet().is_long()) {
            alloc_traits::deallocate(alloc(), facet().get_long_pointer(), facet().get_long_cap());
        }
    }

    // Init
    template <typename InputIter, enable_input_iterator_t<InputIter>* = nullptr>
    void
    init(InputIter first, InputIter last)
    {
        facet().zero();
        try {
            for (; first != last; ++first) {
                push_back(*first);
            }
        } catch(...) {
            deallocate();
            throw;
        }
    }

    template <typename ForwardIter, enable_forward_iterable_t<ForwardIter>* = nullptr>
    void
    init(ForwardIter first, ForwardIter last)
    {
        size_type sz = static_cast<size_type>(std::distance(first, last));
        if (sz > max_size()) {
            throw std::length_error("basic_string");
        }
        pointer p;
        if (sz < facet_type::min_cap) {
            facet().set_short_size(sz);
            p = facet().get_short_pointer();
        } else {
            size_type cap = recommend(sz);
            p = alloc_traits::allocate(alloc(), cap+1);
            facet().set_long_pointer(p);
            facet().set_long_cap(cap+1);
            facet().set_long_size(sz);
        }

        for (; first != last; ++first, (void) ++p) {
            traits_type::assign(*p, *first);
        }
        traits_type::assign(*p, value_type());
    }

    void
    init(const_pointer s, size_type sz, size_type reserve)
    {
        if (reserve > max_size()) {
            throw std::length_error("basic_string");
        }

        pointer p;
        if (reserve < facet_type::min_cap) {
            facet().set_short_size(sz);
            p = facet().get_short_pointer();
        } else {
            size_type cap = recommend(reserve);
            p = alloc_traits::allocate(alloc(), cap+1);
            facet().set_long_pointer(p);
            facet().set_long_cap(cap+1);
            facet().set_long_size(sz);
        }
        traits_type::copy(to_raw_pointer(p), s, sz);
        traits_type::assign(p[sz], value_type());
    }

    void
    init(const_pointer s, size_type sz)
    {
        if (sz > max_size()) {
            throw std::length_error("basic_string");
        }

        pointer p;
        if (sz < facet_type::min_cap) {
            facet().set_short_size(sz);
            p = facet().get_short_pointer();
        } else {
            size_type cap = recommend(sz);
            p = alloc_traits::allocate(alloc(), cap+1);
            facet().set_long_pointer(p);
            facet().set_long_cap(cap+1);
            facet().set_long_size(sz);
        }
        traits_type::copy(to_raw_pointer(p), s, sz);
        traits_type::assign(p[sz], value_type());
    }

    void
    init(size_type n, value_type c)
    {
        if (n > max_size()) {
            throw std::length_error("basic_string");
        }

        pointer p;
        if (n < facet_type::min_cap) {
            facet().set_short_size(n);
            p = facet().get_short_pointer();
        } else {
            size_type cap = recommend(n);
            p = alloc_traits::allocate(alloc(), cap+1);
            facet().set_long_pointer(p);
            facet().set_long_cap(cap+1);
            facet().set_long_size(n);
        }
        traits_type::assign(to_raw_pointer(p), n, c);
        traits_type::assign(p[n], value_type());
    }

    // Copy Assign Alloc
    void
    copy_assign_alloc(const basic_string& x, std::true_type)
    {
        if (alloc() == x.alloc()) {
            alloc() = x.alloc();
        } else {
            if (x.facet().is_long()) {
                clear();
                shrink_to_fit();
                alloc() = x.alloc();
            } else {
                allocator_type a = x.alloc();
                pointer p = alloc_traits::allocate(a, x.facet().get_long_cap());
                clear();
                shrink_to_fit();
                alloc() = std::move(a);
                facet().set_long_pointer(p);
                facet().set_long_cap(x.facet().get_long_cap());
                facet().set_long_size(x.size());
            }
        }
    }

    void
    copy_assign_alloc(const basic_string&, std::false_type)
    {}

    void
    copy_assign_alloc(const basic_string& x)
    {
        constexpr bool propagate = alloc_traits::propagate_on_container_copy_assignment::value;
        copy_assign_alloc(x, std::integral_constant<bool, propagate>());
    }

    // Move Assign Alloc
    void
    move_assign_alloc(basic_string& x, std::true_type)
    noexcept
    {
        alloc() = move(x.alloc());
    }

    void
    move_assign_alloc(basic_string&, std::false_type)
    noexcept
    {}

    void
    move_assign_alloc(basic_string& x)
    noexcept
    {
        constexpr bool propagate = alloc_traits::propagate_on_container_move_assignment::value;
        move_assign_alloc(x, std::integral_constant<bool, propagate>());
    }

    // Move Assign
    void
    move_assign(basic_string& x, std::true_type)
    {
        clear();
        shrink_to_fit();
        facet().r_ = x.facet().r_;
        move_assign_alloc(x);
        x.zero();
    }

    void
    move_assign(basic_string& x, std::false_type)
    {
        if (alloc() == x.alloc()) {
            move_assign(x, std::true_type());
        } else {
            assign(x);
        }
    }

    void
    move_assign(basic_string& x)
    {
        constexpr bool propagate = alloc_traits::propagate_on_container_move_assignment::value;
        move_assign(x, std::integral_constant<bool, propagate>());
    }

    // Grow by
    void
    grow_by(
        size_type old_cap,
        size_type delta_cap,
        size_type old_sz,
        size_type n_copy,
        size_type n_del,
        size_type n_add = 0
    )
    {
        // get the ratio for our growth
        constexpr double ratio = CONTAINED_GROWTH_RATIO;

        size_type ms = max_size();
        if (delta_cap > ms - old_cap) {
            throw std::length_error("basic_string");
        }
        pointer old_p = facet().get_pointer();
        size_type cap;
        if (old_cap < ms / ratio - facet_type::alignment) {
            cap = recommend(std::max(old_cap + delta_cap, ratio * old_cap));
        } else {
            cap = ms - 1;
        }

        pointer p = alloc_traits::allocate(alloc(), cap+1);
        if (n_copy != 0) {
            traits_type::copy(to_raw_pointer(p), to_raw_pointer(old_p), n_copy);
        }
        size_type sec_cp_sz = old_sz - n_del - n_copy;
        if (sec_cp_sz != 0) {
            pointer dst = to_raw_pointer(p) + n_copy + n_add;
            pointer src = to_raw_pointer(old_p) + n_copy + n_del;
            traits_type::copy(dst, src, sec_cp_sz);
        }

        if (old_cap+1 != facet_type::min_cap) {
            alloc_traits::deallocate(alloc(), old_p, old_cap+1);
        }
        facet().set_long_pointer(p);
        facet().set_long_cap(cap+1);
    }

    void
    grow_by_and_replace(
        size_type old_cap,
        size_type delta_cap,
        size_type old_sz,
        size_type n_copy,
        size_type n_del,
        size_type n_add,
        const_pointer p_new_stuff
    )
    {
        // get the ratio for our growth
        constexpr double ratio = CONTAINED_GROWTH_RATIO;

        size_type ms = max_size();
        if (delta_cap > ms - old_cap - 1) {
            throw std::length_error("basic_string");
        }

        pointer old_p = facet().get_pointer();
        size_type cap;
        if (old_cap < ms / ratio - facet_type::alignment) {
            cap = recommend(std::max(old_cap + delta_cap, ratio * old_cap));
        } else {
            cap = ms - 1;
        }
        pointer p = alloc_traits::allocate(alloc(), cap+1);
        if (n_copy != 0) {
            traits_type::copy(to_raw_pointer(p), to_raw_pointer(old_p), n_copy);
        }
        if (n_add != 0) {
            traits_type::copy(to_raw_pointer(p) + n_copy, p_new_stuff, n_add);
        }
        size_type sec_cp_sz = old_sz - n_del - n_copy;
        if (sec_cp_sz != 0) {
            pointer dst = to_raw_pointer(p) + n_copy + n_add;
            pointer src = to_raw_pointer(old_p) + n_copy + n_del;
            traits_type::copy(dst, src, sec_cp_sz);
        }

        if (old_cap+1 != facet_type::min_cap) {
            alloc_traits::deallocate(alloc(), old_p, old_cap+1);
        }
        facet().set_long_pointer(p);
        facet().set_long_cap(cap+1);
        old_sz = n_copy + n_add + sec_cp_sz;
        facet().set_long_size(old_sz);
        traits_type::assign(p[old_sz], value_type());
    }

    // Recommend
    static
    size_type
    recommend(size_type s)
    noexcept
    {
        return facet_type::recommend(s);
    }

    // Erase to end
    void
    erase_to_end(size_type pos)
    {
        if (facet().is_long()) {
            traits_type::assign(*(facet().get_long_pointer() + pos), value_type());
            facet().set_long_size(pos);
        } else {
            traits_type::assign(*(facet().get_short_pointer() + pos), value_type());
            facet().set_short_size(pos);
        }
    }
};

// TODO: hash???
// TODO: relative operators

}   /* contained */
