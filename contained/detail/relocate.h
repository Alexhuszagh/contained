//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  Deleter type for a `unique_ptr` that wraps a custom allocator.
 *  Abstraction to allocate ephemeral objects without resource leaks.
 *
 *  \synopsis
 // TODO:
 */

#pragma once

#include <contained/detail/uninitialized.h>
#include <contained/detail/util.h>
#include <cstring>
#include <memory>
#include <type_traits>
#include <utility>

namespace contained
{
// SFINAE
// ------

// Relocatable (Proposed extension)
// `is_relocatable` should be specialized for any types if possible,
// since it allows raw bitwise copies.
// Inspired by Working Group paper P0023R0.
template <typename T>
struct is_relocatable: std::integral_constant<
        bool,
        // Empty classes rely on global, or no state,
        // and therefore can always be relocated.
        std::is_empty<T>::value ||
        // Trivially copyable classes are guaranteed to be copyable
        // via `memcpy`.
        std::is_trivially_copyable<T>::value ||
        // Trivially move-constructible classes should also satisfy
        // trivially copyable types.
        std::is_trivially_move_constructible<T>::value
    >
{};

// HELPERS
// -------

template <typename P1, typename Size, typename P2>
inline
void
relocate_n_impl(P1 src_first, Size n, P2 dst_first, std::true_type)
{
    using src_traits = std::pointer_traits<P1>;
    using dst_traits = std::pointer_traits<P2>;
    using src_type = typename src_traits::element_type;
    using dst_type = typename dst_traits::element_type;

    char* src = reinterpret_cast<char*>(to_raw_pointer(src_first));
    char* dst = reinterpret_cast<char*>(to_raw_pointer(dst_first));
    Size bytes = n * sizeof(src_type);
    std::memmove(dst, src, bytes);
}


template <typename P1, typename Size, typename P2>
inline
void
relocate_n_impl(P1 src_first, Size n, P2 dst_first, std::false_type)
{
    uninitialized_move_n(src_first, n, dst_first);
}

// FUNCTIONS
// ---------

template <typename P1, typename Size, typename P2>
inline
void
relocate_n(P1 src_first, Size n, P2 dst_first)
{
    // Only relocate if the underlying value_type is relocatable,
    // and the pointer is a raw pointer. Other pointer types,
    // like `offset_ptr`, which used for shared memory, may be
    // catastrophic.
    using pointer = typename std::remove_cv<P1>::type;
    using value_type = typename std::pointer_traits<pointer>::element_type;
    using relocatable = std::integral_constant<
        bool,
        is_relocatable<value_type>::value && std::is_same<pointer, value_type*>::value
    >;

    relocate_n_impl(src_first, n, dst_first, relocatable());
}

template <typename P1, typename P2>
inline
void
relocate(P1 src_first, P1 src_last, P2 dst_first)
{

    relocate_n(src_first, std::distance(src_first, src_last), dst_first);
}

}   /* contained */
