//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2018 Alex Huszagh.
//  :license: MIT, see LICENSE.md for more details.

#pragma once

#include <type_traits>

namespace contained
{
// MACROS
// ------

// Constexpr does not imply const after C++14.
#if __cplusplus >= 201402L
#   define CONTAINED_CPP14_CONSTEXPR constexpr
#else
#   define CONTAINED_CPP14_CONSTEXPR
#endif

/**
 *  \brief Check if class has member type.
 */
#define CONTAINED_HAS_MEMBER_TYPE(member, name)                 \
    template <typename T>                                       \
    class name                                                  \
    {                                                           \
    protected:                                                  \
                                                                \
        template <typename C>                                   \
        static char                                             \
        &test(typename C::member*);                             \
                                                                \
        template <typename C>                                   \
        static long                                             \
        &test(...);                                             \
                                                                \
    public:                                                     \
        enum {                                                  \
            value = sizeof(test<T>(0)) == sizeof(char)          \
        };                                                      \
    }

// SFINAE
// ------
template <typename T>
using is_input_iterable = std::is_base_of<
    std::input_iterator_tag,
    typename std::iterator_traits<T>::iterator_category
>;

template <typename T, typename R = void>
using enable_input_iterable_t = typename std::enable_if<
    is_input_iterable<T>::value, R
>::type;

}   /* contained */
