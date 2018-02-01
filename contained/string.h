//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \synopsis
 // TODO: implement.
 */

#pragma once

#include <contained/string_view.h>
#include <contained/detail/util.h>
#include <string>

namespace contained
{
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
    // TODO: implement

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
        return std::numeric_limits<size_type>::max();
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

    // COMPARE

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
    compare(size_type pos1, size_type n1, const basic_string_facet& s, size_type pos2, size_type n2=npos)
    const
    {
        return compare(pos1, n1, view_type(s), pos2, n2);
    }


//    template <class T>
//    int compare(size_type pos1, size_type n1, const T& t, size_type pos2, size_type n2=npos) const;
//    int compare(const value_type* s) const noexcept;
//    int compare(size_type pos1, size_type n1, const value_type* s) const;
//    int compare(size_type pos1, size_type n1, const value_type* s, size_type n2) const;

// TODO: here...
//    size_type find(const basic_string& str, size_type pos = 0) const noexcept;
//    size_type find(basic_string_view<charT, traits> sv, size_type pos = 0) const noexcept;
//    size_type find(const value_type* s, size_type pos, size_type n) const noexcept;
//    size_type find(const value_type* s, size_type pos = 0) const noexcept;
//    size_type find(value_type c, size_type pos = 0) const noexcept;
//    size_type rfind(const basic_string& str, size_type pos = npos) const noexcept;
//    size_type rfind(basic_string_view<charT, traits> sv, size_type pos = npos) const noexcept;
//    size_type rfind(const value_type* s, size_type pos, size_type n) const noexcept;
//    size_type rfind(const value_type* s, size_type pos = npos) const noexcept;
//    size_type rfind(value_type c, size_type pos = npos) const noexcept;
//    size_type find_first_of(const basic_string& str, size_type pos = 0) const noexcept;
//    size_type find_first_of(basic_string_view<charT, traits> sv, size_type pos = 0) const noexcept;
//    size_type find_first_of(const value_type* s, size_type pos, size_type n) const noexcept;
//    size_type find_first_of(const value_type* s, size_type pos = 0) const noexcept;
//    size_type find_first_of(value_type c, size_type pos = 0) const noexcept;
//    size_type find_last_of(const basic_string& str, size_type pos = npos) const noexcept;
//    size_type find_last_of(basic_string_view<charT, traits> sv, size_type pos = npos) const noexcept;
//    size_type find_last_of(const value_type* s, size_type pos, size_type n) const noexcept;
//    size_type find_last_of(const value_type* s, size_type pos = npos) const noexcept;
//    size_type find_last_of(value_type c, size_type pos = npos) const noexcept;
//    size_type find_first_not_of(const basic_string& str, size_type pos = 0) const noexcept;
//    size_type find_first_not_of(basic_string_view<charT, traits> sv, size_type pos = 0) const noexcept;
//    size_type find_first_not_of(const value_type* s, size_type pos, size_type n) const noexcept;
//    size_type find_first_not_of(const value_type* s, size_type pos = 0) const noexcept;
//    size_type find_first_not_of(value_type c, size_type pos = 0) const noexcept;
//    size_type find_last_not_of(const basic_string& str, size_type pos = npos) const noexcept;
//    size_type find_last_not_of(basic_string_view<charT, traits> sv, size_type pos = npos) const noexcept;
//    size_type find_last_not_of(const value_type* s, size_type pos, size_type n) const noexcept;
//    size_type find_last_not_of(const value_type* s, size_type pos = npos) const noexcept;
//    size_type find_last_not_of(value_type c, size_type pos = npos) const noexcept;

    // Starts with
    // TODO: here...
//    bool starts_with(basic_string_view sv) const noexcept;
//    bool starts_with(charT c) const noexcept;
//    bool starts_with(const charT* s) const;

    // Ends with
    // TODO: here...
//    bool ends_with(basic_string_view sv) const noexcept;
//    bool ends_with(charT c) const noexcept;
//    bool ends_with(const charT* s) const;

protected:
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

    // Is long
    bool
    is_long()
    const noexcept
    {
        return static_cast<bool>(r_.first().s.size_ & short_mask);
    }

    // Get long cap
    size_type
    get_long_cap()
    const noexcept
    {
        return r_.first().l.cap_ & size_type(~long_mask);
    }

    // Get long size
    size_type
    get_long_size()
    const noexcept
    {
        return r_.first().l.size_;
    }

    // Set long size
    void
    set_long_size(size_type s)
    const noexcept
    {
        r_.first().l.size_ = s;
    }

    // Get short size
    size_type
    get_short_size()
    const noexcept
    {
#if BYTE_ORDER == BIG_ENDIAN
        return r_.first().s.size_;
#else
        return r_.first().s.size_ >> 1;
#endif
    }

    // Set short size
    void
    set_short_size(size_type s)
    const noexcept
    {
#if BYTE_ORDER == BIG_ENDIAN
        r_.first().s.size_ = static_cast<unsigned char>(s);
#else
        r_.first().s.size_ = static_cast<unsigned char>(s << 1);
#endif
    }

    // Get long pointer
    pointer
    get_long_pointer()
    noexcept
    {
        return r_.first().l.data_;
    }

    const_pointer
    get_long_pointer()
    const noexcept
    {
        return r_.first().l.data_;
    }

    // Get short pointer
    pointer
    get_short_pointer()
    noexcept
    {
        return std::pointer_traits<pointer>::pointer_to(r_.first().s.data_[0]);
    }

    const_pointer
    get_short_pointer()
    const noexcept
    {
        return std::pointer_traits<const_pointer>::pointer_to(r_.first().s.data_[0]);
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
};

// STRING



}   /* contained */
