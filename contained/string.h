//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \synopsis
 // TODO: implement.
 */

#pragma once

#include <contained/string_view.h>
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
class string_facet
{
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

    // Constructors

    // Iterators

    // TODO:

protected:
    // Short-string optimization
    struct long_layout
    {
        size_type cap_;
        size_type size_;
        pointer   data_;
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
};

// STRING



}   /* contained */
