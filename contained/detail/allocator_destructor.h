//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  Deleter type for a `unique_ptr` that wraps a custom allocator.
 *  Abstraction to allocate ephemeral objects without resource leaks.
 *
 *  \synopsis
 *      template <typename Allocator, std::size_t Size>
 *      struct allocator_destructor
 *      {
 *          using allocator_type = Allocator;
 *          using pointer = implementation-defined;
 *          using size_type = implementation-defined;
 *
 *          allocator_destructor(allocator_type& alloc, size_type size) noexcept;
 *          void operator()(pointer p) noexcept;
 *      };
 */

#pragma once

#include <cstddef>
#include <type_traits>
#include <contained/detail/util.h>

namespace contained
{
// SFINAE
// ------

CONTAINED_HAS_MEMBER_TYPE(is_always_equal, has_is_always_equal);

template <typename T>
using is_always_equal = std::integral_constant<
    bool,
    std::is_empty<T>::value ||
    (has_is_always_equal<T>::value && T::is_always_equal::value)
>;

// OBJECTS
// -------

template <
    typename Allocator,
    std::size_t Size = 1,
    bool IsAlwaysEqual = is_always_equal<Allocator>::value
>
class allocator_destructor;

// Always equal
// An always equal allocator means we can construct an instance
// whenever we wish: it stores no state, so we don't have to
// store a reference.
template <typename Allocator, std::size_t Size>
class allocator_destructor<Allocator, Size, true>: Allocator
{
public:
    using allocator_type = Allocator;
    using alloc_traits = std::allocator_traits<allocator_type>;
    using pointer = typename alloc_traits::pointer;
    using size_type = typename alloc_traits::size_type;

    allocator_destructor(allocator_type&)
    noexcept
    {}

    void
    operator()(pointer p)
    noexcept
    {
        allocator_type alloc;
        alloc_traits::deallocate(alloc, p, Size);
    }

    allocator_type&
    get_allocator()
    {
        return *this;
    }
};

// Not always equal
// Allocator stores state, and we don't know how much.
// Bind a reference to minimize overhead.
template <typename Allocator, std::size_t Size>
class allocator_destructor<Allocator, Size, false>
{
public:
    using allocator_type = Allocator;
    using alloc_traits = std::allocator_traits<allocator_type>;
    using pointer = typename alloc_traits::pointer;
    using size_type = typename alloc_traits::size_type;

public:
    allocator_destructor(allocator_type& alloc)
    noexcept:
        alloc_(alloc)
    {}

    void
    operator()(pointer p)
    noexcept
    {
        alloc_traits::deallocate(alloc_, p, Size);
    }

    allocator_type&
    get_allocator()
    {
        return alloc_;
    }

private:
    Allocator& alloc_;
};

}   /* contained */
