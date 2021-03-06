//  :copyright: (c) 2000 Steve Cleary, Beman Dawes, Howard Hinnant & John Maddock.
//  :copyright: (c) 2018 Alex Huszagh.
//  :license:  Boost, see licenses/boost.md for more details.
/**
 *  A pair implementation that compressed empty classes using empty-base
 *  optimization (EBO). Used extensively in STL containers to minimize
 *  the overhead of stateless allocators.
 *
 *  \synopsis
 *      template <typename T1, typename T2>
 *      class compressed_pair
 *      {
 *      public:
 *          using first_type = T1;
 *          using second_type = T2;
 *          using first_reference = first_type&;
 *          using first_const_reference = const first_type&;
 *          using second_reference = second_type&;
 *          using second_const_reference = const second_type&;
 *
 *          compressed_pair();
 *          compressed_pair(first_const_reference x, second_const_reference y);
 *          explicit compressed_pair(first_const_reference x);
 *          explicit compressed_pair(second_const_reference y);
 *          compressed_pair(first_type&& x, second_type&& y);
 *          explicit compressed_pair(first_type&& x);
 *          explicit compressed_pair(second_type&& y);
 *
 *         constexpr first_reference first() noexcept;
 *         constexpr first_const_reference first() const noexcept;
 *         constexpr second_reference second() noexcept;
 *         constexpr second_const_reference second() const noexcept;
 *
 *         void swap(compressed_pair& y);
 *      };
 *
 *      template <typename T>
 *      class compressed_pair<T, T>
 *      {
 *      public:
 *          using first_type = T;
 *          using second_type = T;
 *          using first_reference = first_type&;
 *          using first_const_reference = const first_type&;
 *          using first_move_reference = first_type&&;
 *          using first_const_move_reference = const first_type&&;
 *          using second_reference = second_type&;
 *          using second_const_reference = const second_type&;
 *          using second_move_reference = second_type&&;
 *          using second_const_move_reference = const second_type&&;
 *
 *          compressed_pair();
 *          compressed_pair(first_const_reference x, second_const_reference y);
 *          explicit compressed_pair(first_const_reference x);
 *          compressed_pair(first_type&& x, second_type&& y);
 *          explicit compressed_pair(first_type&& x);
 *
 *         constexpr first_reference first() noexcept;
 *         constexpr first_const_reference first() const noexcept;
 *         constexpr second_reference second() noexcept;
 *         constexpr second_const_reference second() const noexcept;
 *
 *         void swap(compressed_pair& y);
 *      };
 *
 *      template <typename T1, typename T2>
 *      struct tuple_element<0, compressed_pair<T1, T2>>
 *      {
 *          using type = T1;
 *      };
 *
 *      template <typename T1, typename T2>
 *      struct tuple_element<1, compressed_pair<T1, T2>>
 *      {
 *          using type = T2;
 *      };
 *
 *      template <typename T1, typename T2>
 *      struct tuple_size<compressed_pair<T1, T2>>: integral_constant<size_t, 2>
 *      {};
 *
 *      template <size_t I, typename T1, typename T2>
 *      inline constexpr typename tuple_element_t<I, compressed_pair<T1, T2>>&
 *      get(compressed_pair<T1, T2>& p) noexcept;
 *
 *      template <size_t I, typename T1, typename T2>
 *      inline constexpr const typename tuple_element_t<I, compressed_pair<T1, T2>>&
 *      get(const compressed_pair<T1, T2>& p) noexcept;
 *
 *      template <size_t I, typename T1, typename T2>
 *      inline constexpr typename tuple_element_t<I, compressed_pair<T1, T2>>&&
 *      get(compressed_pair<T1, T2>&& p) noexcept;
 *
 *      template <size_t I, typename T1, typename T2>
 *      inline constexpr const typename tuple_element_t<I, compressed_pair<T1, T2>>&&
 *      get(const compressed_pair<T1, T2>&& p) noexcept;
 *
 *      template <typename T1, typename T2>
 *      inline constexpr T1& get(compressed_pair<T1, T2>& p) noexcept;
 *
 *      template <typename T1, typename T2>
 *      inline constexpr const T1& get(const compressed_pair<T1, T2>& p) noexcept;
 *
 *      template <typename T1, typename T2>
 *      inline constexpr T1&& get(compressed_pair<T1, T2>&& p) noexcept;
 *
 *      template <typename T1, typename T2>
 *      inline constexpr const T1&& get(const compressed_pair<T1, T2>&& p) noexcept;
 *
 *      template <typename T1, typename T2>
 *      inline constexpr T1& get(compressed_pair<T2, T1>& p) noexcept;
 *
 *      template <typename T1, typename T2>
 *      inline constexpr const T1& get(const compressed_pair<T2, T1>& p) noexcept;
 *
 *      template <typename T1, typename T2>
 *      inline constexpr T1&& get(compressed_pair<T2, T1>&& p) noexcept;
 *
 *      template <typename T1, typename T2>
 *      inline constexpr const T1&& get(const compressed_pair<T2, T1>&& p) noexcept;
 */

#pragma once

#include <contained/detail/util.h>
#include <tuple>
#include <type_traits>
#include <utility>

#if defined(_MSC_VER)
#   pragma warning(push)
#   pragma warning(disable: 4512)
#endif

namespace contained
{

// FORWARD
// -------

template <typename T1, typename T2>
class compressed_pair;

namespace compressed_detail
{
// DECLARATIONS
// ------------

// Helpers to detect the proper class specialization
template <typename T1, typename T2, bool IsSame, bool FirstEmpty, bool SecondEmpty>
struct compressed_pair_switch_helper;

template <typename T1, typename T2>
struct compressed_pair_switch_helper<T1, T2, false, false, false>
{
    static const int value = 0;
};

template <typename T1, typename T2>
struct compressed_pair_switch_helper<T1, T2, false, true, true>
{
    static const int value = 3;
};

template <typename T1, typename T2>
struct compressed_pair_switch_helper<T1, T2, false, true, false>
{
    static const int value = 1;
};

template <typename T1, typename T2>
struct compressed_pair_switch_helper<T1, T2, false, false, true>
{
    static const int value = 2;
};

template <typename T1, typename T2>
struct compressed_pair_switch_helper<T1, T2, true, true, true>
{
    static const int value = 4;
};

template <typename T1, typename T2>
struct compressed_pair_switch_helper<T1, T2, true, false, false>
{
    static const int value = 5;
};

// Uses the helper to get the value for the proper specialization
template <typename T1, typename T2>
using compressed_switch = compressed_pair_switch_helper<
    T1,
    T2,
    std::is_same<
        typename std::remove_cv<T1>::type,
        typename std::remove_cv<T2>::type
    >::value,
    std::is_empty<T1>::value,
    std::is_empty<T2>::value
>;

template <size_t I>
struct get_pair;

template <typename T1, typename T2, int Version>
class compressed_pair_impl;

// 0    derive from neither
template <typename T1, typename T2>
class compressed_pair_impl<T1, T2, 0>
{
public:
    using first_type = T1;
    using second_type = T2;

    // Constructors
    compressed_pair_impl()
    {}

    compressed_pair_impl(
        const first_type& x,
        const second_type& y
    ):
        first_(x),
        second_(y)
    {}

    compressed_pair_impl(
        const first_type& x
    ):
        first_(x)
    {}

    compressed_pair_impl(
        const second_type& y
    ):
        second_(y)
    {}

    compressed_pair_impl(
        first_type&& x,
        second_type&& y
    ):
        first_(std::forward<first_type>(x)),
        second_(std::forward<second_type>(y))
    {}

    compressed_pair_impl(
        first_type&& x
    ):
        first_(std::forward<first_type>(x))
    {}

    compressed_pair_impl(
        second_type&& y
    ):
        second_(std::forward<second_type>(y))
    {}

    // Element Access
    CONTAINED_CPP14_CONSTEXPR
    first_type&
    first()
    noexcept
    {
        return first_;
    }

    CONTAINED_CPP14_CONSTEXPR
    const first_type&
    first()
    const noexcept
    {
        return first_;
    }

    CONTAINED_CPP14_CONSTEXPR
    second_type&
    second()
    noexcept
    {
        return second_;
    }

    CONTAINED_CPP14_CONSTEXPR
    const second_type&
    second()
    const noexcept
    {
        return second_;
    }

    // Modifiers
    void
    swap(
        compressed_pair<T1, T2>& y
    )
    {
        fast_swap(first_, y.first());
        fast_swap(second_, y.second());
    }

private:
    first_type first_;
    second_type second_;
};


// 1    derive from T1
template <typename T1, typename T2>
class compressed_pair_impl<T1, T2, 1>:
    protected std::remove_cv<T1>::type
{
public:
    using first_type = T1;
    using second_type = T2;

    // Constructors
    compressed_pair_impl()
    {}

    compressed_pair_impl(
        const first_type& x,
        const second_type& y
    ):
        first_type(x),
        second_(y)
    {}

    compressed_pair_impl(
        const first_type& x
    ):
        first_type(x)
    {}

    compressed_pair_impl(
        const second_type& y
    ):
        second_(y)
    {}

    compressed_pair_impl(
        first_type&& x,
        second_type&& y
    ):
        first_type(std::forward<first_type>(x)),
        second_(std::forward<second_type>(y))
    {}

    compressed_pair_impl(
        first_type&& x
    ):
        first_type(std::forward<first_type>(x))
    {}

    compressed_pair_impl(
        second_type&& y
    ):
        second_(std::forward<second_type>(y))
    {}

    // Element Access
    CONTAINED_CPP14_CONSTEXPR
    first_type&
    first()
    noexcept
    {
        return *this;
    }


    CONTAINED_CPP14_CONSTEXPR
    const first_type&
    first()
    const noexcept
    {
        return *this;
    }

    CONTAINED_CPP14_CONSTEXPR
    second_type&
    second()
    noexcept
    {
        return second_;
    }

    CONTAINED_CPP14_CONSTEXPR
    const second_type&
    second()
    const noexcept
    {
        return second_;
    }

    // Modifiers
    void
    swap(
        compressed_pair<T1, T2>& y
    )
    {
        // no need to swap empty base class
        fast_swap(second_, y.second());
    }

private:
    second_type second_;
};


// 2    derive from T2
template <typename T1, typename T2>
class compressed_pair_impl<T1, T2, 2>:
    protected std::remove_cv<T2>::type
{
public:
    using first_type = T1;
    using second_type = T2;

    // Constructors
    compressed_pair_impl()
    {}

    compressed_pair_impl(
        const first_type& x,
        const second_type& y
    ):
        second_type(y),
        first_(x)
    {}

    compressed_pair_impl(
        const first_type& x
    ):
        first_(x)
    {}

    compressed_pair_impl(
        const second_type& y
    ):
        second_type(y)
    {}

    compressed_pair_impl(
        first_type&& x,
        second_type&& y
    ):
        second_type(std::forward<second_type>(y)),
        first_(std::forward<first_type>(x))
    {}

    compressed_pair_impl(
        first_type&& x
    ):
        first_(std::forward<first_type>(x))
    {}

    compressed_pair_impl(
        second_type&& y
    ):
        second_type(std::forward<second_type>(y))
    {}

    // Element Access
    CONTAINED_CPP14_CONSTEXPR
    first_type&
    first()
    noexcept
    {
        return first_;
    }

    CONTAINED_CPP14_CONSTEXPR
    const first_type&
    first()
    const noexcept
    {
        return first_;
    }

    CONTAINED_CPP14_CONSTEXPR
    second_type&
    second()
    noexcept
    {
        return *this;
    }

    CONTAINED_CPP14_CONSTEXPR
    const second_type&
    second()
    const noexcept
    {
        return *this;
    }

    // Modifiers
    void
    swap(
        compressed_pair<T1, T2>& y
    )
    {
        // no need to swap empty base class
        fast_swap(first_, y.first());
    }

private:
    first_type first_;
};


// 3    derive from T1 and T2
template <typename T1, typename T2>
class compressed_pair_impl<T1, T2, 3>:
    protected std::remove_cv<T1>::type,
    protected std::remove_cv<T2>::type
{
public:
    using first_type = T1;
    using second_type = T2;

    // Constructors
    compressed_pair_impl()
    {}

    compressed_pair_impl(
        const first_type& x,
        const second_type& y
    ):
        first_type(x),
        second_type(y)
    {}

    compressed_pair_impl(
        const first_type& x
    ):
        first_type(x)
    {}

    compressed_pair_impl(
        const second_type& y
    ):
        second_type(y)
    {}

    compressed_pair_impl(
        first_type&& x,
        second_type&& y
    ):
        first_type(std::forward<first_type>(x)),
        second_type(std::forward<second_type>(y))
    {}

    compressed_pair_impl(
        first_type&& x
    ):
        first_type(std::forward<first_type>(x))
    {}

    compressed_pair_impl(
        second_type&& y
    ):
        second_type(std::forward<second_type>(y))
    {}

    // Element Access
    CONTAINED_CPP14_CONSTEXPR
    first_type&
    first()
    noexcept
    {
        return *this;
    }

    CONTAINED_CPP14_CONSTEXPR
    const first_type&
    first()
    const noexcept
    {
        return *this;
    }

    CONTAINED_CPP14_CONSTEXPR
    second_type&
    second()
    noexcept
    {
        return *this;
    }

    CONTAINED_CPP14_CONSTEXPR
    const second_type&
    second()
    const noexcept
    {
        return *this;
    }

    // Modifiers
    void
    swap(
        compressed_pair<T1, T2>& y
    )
    noexcept
    {
        // no need to swap empty base class
    }
};


// 4    T1 == T2, T1 and T2 both empty
//  Originally, the pair did not store an instance of T2 at all
//  but that led to problems beause it meant &x.first() == &x.second()
//  which is not true for any other kind of pair, so now we store an
//  instance of T2 just in case the user is relying on first() and
//  second() returning different objects (albeit both empty).
template <typename T1, typename T2>
class compressed_pair_impl<T1, T2, 4>:
    protected std::remove_cv<T1>::type
{
public:
    using first_type = T1;
    using second_type = T2;

    // Constructors
    compressed_pair_impl()
    {}

    compressed_pair_impl(
        const first_type& x,
        const second_type& y
    ):
        first_type(x),
        second_(y)
    {}

    compressed_pair_impl(
        const first_type& x
    ):
        first_type(x),
        second_(x)
    {}

    compressed_pair_impl(
        first_type&& x,
        second_type&& y
    ):
        first_type(std::forward<first_type>(x)),
        second_(std::forward<second_type>(y))
    {}

    compressed_pair_impl(
        first_type&& x
    ):
        first_type(x),
        second_(std::forward<first_type>(x))
    {}

    // Element Access
    CONTAINED_CPP14_CONSTEXPR
    first_type&
    first()
    noexcept
    {
        return *this;
    }

    CONTAINED_CPP14_CONSTEXPR
    const first_type&
    first()
    const noexcept
    {
        return *this;
    }

    CONTAINED_CPP14_CONSTEXPR
    second_type&
    second()
    noexcept
    {
        return second_;
    }

    CONTAINED_CPP14_CONSTEXPR
    const second_type&
    second()
    const noexcept
    {
        return second_;
    }

    // Modifiers
    void
    swap(
        compressed_pair<T1, T2>& y
    )
    noexcept
    {
        // no need to swap empty base class
    }

private:
    second_type second_;
};

// 5    T1 == T2, both not empty
template <typename T1, typename T2>
class compressed_pair_impl<T1, T2, 5>
{
public:
    using first_type = T1;
    using second_type = T2;

    // Constructors
    compressed_pair_impl()
    {}

    compressed_pair_impl(const first_type& x, const second_type& y):
        first_(x),
        second_(y)
    {}

    compressed_pair_impl(const first_type& x):
        first_(x),
        second_(x)
    {}

    compressed_pair_impl(first_type&& x, second_type&& y):
        first_(std::forward<first_type>(x)),
        second_(std::forward<second_type>(y))
    {}

    compressed_pair_impl(first_type&& x):
        first_(x),
        second_(std::forward<first_type>(x))
    {}

    // Element Access
    CONTAINED_CPP14_CONSTEXPR
    first_type&
    first()
    noexcept
    {
        return first_;
    }

    CONTAINED_CPP14_CONSTEXPR
    const first_type&
    first()
    const noexcept
    {
        return first_;
    }

    CONTAINED_CPP14_CONSTEXPR
    second_type&
    second()
    noexcept
    {
        return second_;
    }

    CONTAINED_CPP14_CONSTEXPR
    const second_type&
    second()
    const noexcept
    {
        return second_;
    }

    // Modifiers
    void swap(compressed_pair<T1, T2>& y)
    {
        fast_swap(first_, y.first());
        fast_swap(second_, y.second());
    }

private:
    first_type first_;
    second_type second_;
};

// Helper to select the proper compressed pair
template <typename T1, typename T2>
using compressed_pair_selector = compressed_pair_impl<
    T1,
    T2,
    compressed_switch<T1, T2>::value
>;

}   /* compressed_detail */


template <typename T1, typename T2>
class compressed_pair: private compressed_detail::compressed_pair_selector<T1, T2>
{
public:
    using self_t = compressed_pair<T1, T2>;
    using base_t = compressed_detail::compressed_pair_selector<T1, T2>;
    using first_type = T1;
    using second_type = T2;
    using first_reference = first_type&;
    using first_const_reference = const first_type&;
    using second_reference = second_type&;
    using second_const_reference = const second_type&;

    // Constructors
    compressed_pair():
        base_t()
    {}

    compressed_pair(first_const_reference x, second_const_reference y):
        base_t(x, y)
    {}

    explicit
    compressed_pair(first_const_reference x):
        base_t(x)
    {}

    explicit
    compressed_pair(second_const_reference y):
        base_t(y)
    {}

    compressed_pair(first_type&& x, second_type&& y):
        base_t(std::forward<first_type>(x), std::forward<second_type>(y))
    {}

    explicit
    compressed_pair(first_type&& x):
        base_t(std::forward<first_type>(x))
    {}

    explicit
    compressed_pair(second_type&& y):
        base_t(std::forward<second_type>(y))
    {}

    // Element Access
    CONTAINED_CPP14_CONSTEXPR
    first_reference
    first()
    noexcept
    {
        return base_t::first();
    }

    CONTAINED_CPP14_CONSTEXPR
    first_const_reference
    first()
    const noexcept
    {
        return base_t::first();
    }

    CONTAINED_CPP14_CONSTEXPR
    second_reference
    second()
    noexcept
    {
        return base_t::second();
    }

    CONTAINED_CPP14_CONSTEXPR
    second_const_reference
    second()
    const noexcept
    {
        return base_t::second();
    }

    // Modifiers
    void
    swap(compressed_pair& y)
    {
        base_t::swap(y);
    }
};


template <typename T>
class compressed_pair<T, T>: private compressed_detail::compressed_pair_selector<T, T>
{
public:
    using self_t = compressed_pair<T, T>;
    using base_t = compressed_detail::compressed_pair_selector<T, T>;
    using first_type = T;
    using second_type = T;
    using first_reference = first_type&;
    using first_const_reference = const first_type&;
    using first_move_reference = first_type&&;
    using first_const_move_reference = const first_type&&;
    using second_reference = second_type&;
    using second_const_reference = const second_type&;
    using second_move_reference = second_type&&;
    using second_const_move_reference = const second_type&&;

    // Constructors
    compressed_pair():
        base_t()
    {}

    compressed_pair(first_const_reference x, second_const_reference y):
        base_t(x, y)
    {}

    explicit
    compressed_pair(first_const_reference x):
        base_t(x)
    {}

    compressed_pair(first_type&& x, second_type&& y):
        base_t(std::forward<first_type>(x), std::forward<second_type>(y))
    {}

    explicit
    compressed_pair(first_type&& x):
        base_t(std::forward<first_type>(x))
    {}

    // Element Access
    CONTAINED_CPP14_CONSTEXPR
    first_reference
    first()
    noexcept
    {
        return base_t::first();
    }

    CONTAINED_CPP14_CONSTEXPR
    first_const_reference
    first()
    const noexcept
    {
        return base_t::first();
    }

    CONTAINED_CPP14_CONSTEXPR
    second_reference
    second()
    noexcept
    {
        return base_t::second();
    }

    CONTAINED_CPP14_CONSTEXPR
    second_const_reference
    second()
    const noexcept
    {
        return base_t::second();
    }

    // Modifiers
    void
    swap(
        compressed_pair& y
    )
    {
        base_t::swap(y);
    }
};

// NON-MEMBER

template <typename T1, typename T2>
inline void swap(compressed_pair<T1, T2>& x, compressed_pair<T1, T2>& y)
{
   x.swap(y);
}


namespace compressed_detail
{
// SPECIALIZATION
// --------------

template <>
struct get_pair<0>
{
    template <typename T1, typename T2>
    inline CONTAINED_CPP14_CONSTEXPR
    T1&
    operator()(compressed_pair<T1, T2>& p)
    noexcept
    {
        return p.first();
    }

    template <typename T1, typename T2>
    inline CONTAINED_CPP14_CONSTEXPR
    const T1&
    operator()(const compressed_pair<T1, T2>& p)
    noexcept
    {
        return p.first();
    }

    template <typename T1, typename T2>
    inline CONTAINED_CPP14_CONSTEXPR
    T1&&
    operator()(compressed_pair<T1, T2>&& p)
    noexcept
    {
        return std::forward<T1>(p.first());
    }

    template <typename T1, typename T2>
    inline CONTAINED_CPP14_CONSTEXPR
    const T1&&
    operator()(const compressed_pair<T1, T2>&& p)
    noexcept
    {
        return std::forward<const T1>(p.first());
    }
};


template <>
struct get_pair<1>
{
    template <typename T1, typename T2>
    inline CONTAINED_CPP14_CONSTEXPR
    T2&
    operator()(compressed_pair<T1, T2>& p)
    noexcept
    {
        return p.second();
    }

    template <typename T1, typename T2>
    inline CONTAINED_CPP14_CONSTEXPR
    const T2&
    operator()(const compressed_pair<T1, T2>& p)
    noexcept
    {
        return p.second();
    }

    template <typename T1, typename T2>
    inline CONTAINED_CPP14_CONSTEXPR
    T2&&
    operator()(compressed_pair<T1, T2>&& p)
    noexcept
    {
        return std::forward<T2>(p.second());
    }

    template <typename T1, typename T2>
    inline CONTAINED_CPP14_CONSTEXPR
    const T2&&
    operator()(const compressed_pair<T1, T2>&& p)
    noexcept
    {
        return std::forward<const T2>(p.second());
    }
};

}   /* compressed_detail */


template <size_t I, typename T1, typename T2>
inline CONTAINED_CPP14_CONSTEXPR
typename std::tuple_element<I, compressed_pair<T1, T2>>::type&
get(compressed_pair<T1, T2>& p)
noexcept
{
    return compressed_detail::get_pair<I>()(p);
}


template <size_t I, typename T1, typename T2>
inline CONTAINED_CPP14_CONSTEXPR
const typename std::tuple_element<I, compressed_pair<T1, T2>>::type&
get(const compressed_pair<T1, T2>& p)
noexcept
{
    return compressed_detail::get_pair<I>()(p);
}


template <size_t I, typename T1, typename T2>
inline CONTAINED_CPP14_CONSTEXPR
typename std::tuple_element<I, compressed_pair<T1, T2>>::type&&
get(compressed_pair<T1, T2>&& p)
noexcept
{
    return compressed_detail::get_pair<I>()(std::move(p));
}


template <size_t I, typename T1, typename T2>
inline CONTAINED_CPP14_CONSTEXPR
const typename std::tuple_element<I, compressed_pair<T1, T2>>::type&&
get(const compressed_pair<T1, T2>&& p)
noexcept
{
    return compressed_detail::get_pair<I>()(std::move(p));
}


template <typename T1, typename T2>
inline CONTAINED_CPP14_CONSTEXPR
T1&
get(compressed_pair<T1, T2>& p)
noexcept
{
    return compressed_detail::get_pair<0>()(p);
}

template <typename T1, typename T2>
inline CONTAINED_CPP14_CONSTEXPR
const T1&
get(const compressed_pair<T1, T2>& p)
noexcept
{
    return compressed_detail::get_pair<0>()(p);
}


template <typename T1, typename T2>
inline CONTAINED_CPP14_CONSTEXPR
T1&&
get(compressed_pair<T1, T2>&& p)
noexcept
{
    return compressed_detail::get_pair<0>()(std::move(p));
}


template <typename T1, typename T2>
inline CONTAINED_CPP14_CONSTEXPR
const T1&&
get(const compressed_pair<T1, T2>&& p)
noexcept
{
    return compressed_detail::get_pair<0>()(std::move(p));
}


template <typename T1, typename T2>
inline CONTAINED_CPP14_CONSTEXPR
T1&
get(compressed_pair<T2, T1>& p)
noexcept
{
    return compressed_detail::get_pair<1>()(p);
}


template <typename T1, typename T2>
inline CONTAINED_CPP14_CONSTEXPR
const T1&
get(const compressed_pair<T2, T1>& p)
noexcept
{
    return compressed_detail::get_pair<1>()(p);
}


template <typename T1, typename T2>
inline CONTAINED_CPP14_CONSTEXPR
T1&&
get(compressed_pair<T2, T1>&& p)
noexcept
{
    return compressed_detail::get_pair<1>()(std::move(p));
}


template <typename T1, typename T2>
inline CONTAINED_CPP14_CONSTEXPR
const T1&&
get(const compressed_pair<T2, T1>&& p)
noexcept
{
    return compressed_detail::get_pair<1>()(std::move(p));
}

}   /* contained */

namespace std
{
// SPECIALIZATION
// --------------

template <typename T1, typename T2>
struct tuple_element<0, contained::compressed_pair<T1, T2>>
{
    using type = T1;
};


template <typename T1, typename T2>
struct tuple_element<1, contained::compressed_pair<T1, T2>>
{
    using type = T2;
};

template <typename T1, typename T2>
struct tuple_size<contained::compressed_pair<T1, T2>>: integral_constant<size_t, 2>
{};

}   /* std */

#if defined(_MSC_VER)
#   pragma warning(pop)
#endif
