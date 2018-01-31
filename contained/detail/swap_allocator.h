//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  Swap STL allocators guided by `propagate_on_container_swap`.
 *
 *  \synopsis
 *      template <typename Allocator>
 *      void swap_allocator(Allocator& a1, Allocator& a2) noexcept;
 */

#pragma once

namespace contained
{
namespace
{
// FUNCTIONS
// ---------

template <typename Allocator>
inline
void
swap_allocator_impl(
    Allocator& a1,
    Allocator& a2,
    std::true_type
)
noexcept
{
    std::swap(a1, a2);
}

template <typename Allocator>
inline
void
swap_allocator_impl(
    Allocator&,
    Allocator&,
    std::false_type
)
noexcept
{}

}   /* anonymous */

template <typename Allocator>
inline
void
swap_allocator(
    Allocator& a1,
    Allocator& a2
)
noexcept
{
    using propagate = typename std::allocator_traits<Allocator>::propagate_on_container_swap;
    swap_allocator_impl(a1, a2, std::integral_constant<bool, propagate::value>());
}

}   /* contained */
