//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \synopsis
 *      template<class Char, class traits = std::char_traits<Char>>
 *      class basic_string_view
 *      {
 *      public:
 *          typedef traits traits_type;
 *          typedef Char value_type;
 *          typedef Char* pointer;
 *          typedef const Char* const_pointer;
 *          typedef Char& reference;
 *          typedef const Char& const_reference;
 *          typedef implementation-defined const_iterator;
 *          typedef const_iterator iterator;
 *          typedef reverse_iterator<const_iterator> const_reverse_iterator;
 *          typedef const_reverse_iterator reverse_iterator;
 *          typedef size_t size_type;
 *          typedef ptrdiff_t difference_type;
 *
 *          static constexpr size_type npos = size_type(-1);
 *
 *          constexpr basic_string_view() noexcept;
 *          constexpr basic_string_view(const basic_string_view&) noexcept = default;
 *          basic_string_view& operator=(const basic_string_view&) noexcept = default;
 *          constexpr basic_string_view(const Char* str);
 *          constexpr basic_string_view(const Char* str, size_type len);
 *
 *          constexpr const_iterator begin() const noexcept;
 *          constexpr const_iterator end() const noexcept;
 *          constexpr const_iterator cbegin() const noexcept;
 *          constexpr const_iterator cend() const noexcept;
 *          const_reverse_iterator rbegin() const noexcept;
 *          const_reverse_iterator rend() const noexcept;
 *          const_reverse_iterator crbegin() const noexcept;
 *          const_reverse_iterator crend() const noexcept;
 *
 *          constexpr size_type size() const noexcept;
 *          constexpr size_type length() const noexcept;
 *          constexpr size_type max_size() const noexcept;
 *          constexpr bool empty() const noexcept;
 *
 *          constexpr const_reference operator[](size_type pos) const;
 *          constexpr const_reference at(size_type pos) const;
 *          constexpr const_reference front() const;
 *          constexpr const_reference back() const;
 *          constexpr const_pointer data() const noexcept;
 *
 *          constexpr void remove_prefix(size_type n);
 *          constexpr void remove_suffix(size_type n);
 *          constexpr void swap(basic_string_view& s) noexcept;
 *          size_type copy(Char* s, size_type n, size_type pos = 0) const;
 *          constexpr basic_string_view substr(size_type pos = 0, size_type n = npos) const;
 *          constexpr int compare(basic_string_view s) const noexcept;
 *          constexpr int compare(size_type pos1, size_type n1, basic_string_view s) const;
             *nstexpr int compare(size_type pos1, size_type n1, basic_string_view s, size_type pos2, size_type n2) const;
 *          constexpr int compare(const Char* s) const;
 *          constexpr int compare(size_type pos1, size_type n1, const Char* s) const;
 *          constexpr int compare(size_type pos1, size_type n1, const Char* s, size_type n2) const;
 *          constexpr size_type find(basic_string_view s, size_type pos = 0) const noexcept;
 *          constexpr size_type find(Char c, size_type pos = 0) const noexcept;
 *          constexpr size_type find(const Char* s, size_type pos, size_type n) const;
 *          constexpr size_type find(const Char* s, size_type pos = 0) const;
 *          constexpr size_type rfind(basic_string_view s, size_type pos = npos) const noexcept;
 *          constexpr size_type rfind(Char c, size_type pos = npos) const noexcept;
 *          constexpr size_type rfind(const Char* s, size_type pos, size_type n) const;
 *          constexpr size_type rfind(const Char* s, size_type pos = npos) const;
 *          constexpr size_type find_first_of(basic_string_view s, size_type pos = 0) const noexcept;
 *          constexpr size_type find_first_of(Char c, size_type pos = 0) const noexcept;
 *          constexpr size_type find_first_of(const Char* s, size_type pos, size_type n) const;
 *          constexpr size_type find_first_of(const Char* s, size_type pos = 0) const;
 *          constexpr size_type find_last_of(basic_string_view s, size_type pos = npos) const noexcept;
 *          constexpr size_type find_last_of(Char c, size_type pos = npos) const noexcept;
 *          constexpr size_type find_last_of(const Char* s, size_type pos, size_type n) const;
 *          constexpr size_type find_last_of(const Char* s, size_type pos = npos) const;
 *          constexpr size_type find_first_not_of(basic_string_view s, size_type pos = 0) const noexcept;
 *          constexpr size_type find_first_not_of(Char c, size_type pos = 0) const noexcept;
 *          constexpr size_type find_first_not_of(const Char* s, size_type pos, size_type n) const;
 *          constexpr size_type find_first_not_of(const Char* s, size_type pos = 0) const;
 *          constexpr size_type find_last_not_of(basic_string_view s, size_type pos = npos) const noexcept;
 *          constexpr size_type find_last_not_of(Char c, size_type pos = npos) const noexcept;
 *          constexpr size_type find_last_not_of(const Char* s, size_type pos, size_type n) const;
 *          constexpr size_type find_last_not_of(const Char* s, size_type pos = npos) const;
 *          constexpr bool starts_with(basic_string_view s) const noexcept;
 *          constexpr bool starts_with(Char c) const noexcept;
 *          constexpr bool starts_with(const Char* s) const;
 *          constexpr bool ends_with(basic_string_view s) const noexcept;
 *          constexpr bool ends_with(Char c) const noexcept;
 *          constexpr bool ends_with(const Char* s) const;
 *      };
 */

#pragma once

#include <contained/detail/util.h>
#include <algorithm>
#include <cassert>
#include <functional>
#include <iosfwd>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <string>

namespace contained
{
// FORWARD
// -------

template <
    typename Char,
    typename Traits = std::char_traits<Char>
>
class basic_string_view;

// HELPERS
// -------

template <typename Iter, typename Char, typename Traits>
static
Iter
find_not_of_impl(Iter first, Iter last, basic_string_view<Char, Traits> s)
noexcept;

// ALIAS
// -----

using string_view = basic_string_view<char>;
using wstring_view = basic_string_view<wchar_t>;
using u16string_view = basic_string_view<char16_t>;
using u32string_view = basic_string_view<char32_t>;

// OBJECTS

// BASIC STRING VIEW

/**
 *  \brief STL string wrapper.
 *
 *  Binds a pointer and the string length, accepting either a C++
 *  string, a null-terminated string, a character array and length,
 *  or an begin/end pointer pair.
 *
 *  \warning The lifetime of the source data must outlive the wrapper.
 *  The wrapper has **no** ownership, and is merely an STL-like wrapper
 *  for performance reasons.
 */
template <typename Char, typename Traits>
class basic_string_view
{
public:
    using value_type = Char;
    using traits_type = Traits;
    using reference = value_type&;
    using const_reference = const value_type&;
    using pointer = value_type*;
    using const_pointer = const value_type*;
    using size_type = size_t;
    using difference_type = std::ptrdiff_t;
    using iterator = const_pointer;
    using const_iterator = const_pointer;
    using reverse_iterator = std::reverse_iterator<pointer>;
    using const_reverse_iterator = std::reverse_iterator<const_pointer>;

    static const size_type npos = SIZE_MAX;

    // Constructors
    basic_string_view() noexcept = default;
    basic_string_view(const basic_string_view& str) noexcept = default;

    basic_string_view(const_pointer str, size_type count)
    noexcept:
        data_(str),
        length_(count)
    {}

    basic_string_view(const_pointer str)
    noexcept:
        data_(str),
        length_(traits_type::length(str))
    {}

    // Assignment
    basic_string_view& operator=(const basic_string_view& str) noexcept = default;

    // Iterators
    const_iterator
    begin()
    const noexcept
    {
        return data_;
    }

    const_iterator
    end()
    const noexcept
    {
        return data_ + length_;
    }

    const_reverse_iterator
    rbegin()
    const noexcept
    {
        return const_reverse_iterator(end());
    }

    const_reverse_iterator
    rend()
    const noexcept
    {
        return const_reverse_iterator(begin());
    }

    const_iterator
    cbegin()
    const noexcept
    {
        return begin();
    }

    const_iterator
    cend()
    const noexcept
    {
        return end();
    }

    const_reverse_iterator
    crbegin()
    const noexcept
    {
        return rbegin();
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
        return length();
    }

    size_type
    length()
    const noexcept
    {
        return length_;
    }

    size_type
    max_size()
    const noexcept
    {
        return std::numeric_limits<size_type>::max();
    }

    CONTAINED_CPP17_NODISCARD
    bool
    empty()
    const noexcept
    {
        return size() == 0;
    }

    // Element Access
    const_reference
    operator[](size_type pos)
    const
    {
        assert(pos <= size() && "string index out of bounds");
        return *(data_ + pos);
    }

    const_reference
    at(size_type pos)
    const
    {
        if (pos > size()) {
            throw std::out_of_range("string_view::at");
        }
        return operator[](pos);
    }

    const_reference
    front()
    const
    {
        assert(!empty() && "basic_string_view::front(): string is empty");
        return *data_;
    }

    const_reference
    back()
    const
    {
        assert(!empty() && "basic_string_view::back(): string is empty");
        return *(data_ + length_ - 1);
    }

    const_pointer
    data()
    const noexcept
    {
        return data_;
    }

    // Modifiers
    void
    remove_prefix(size_type n)
    noexcept
    {
        assert(n <= size() && "basic_string_view::remove_prefix greater than size.");
        data_ += n;
        length_ -= n;
    }

    void
    remove_suffix(size_type n)
    noexcept
    {
        assert(n <= size() && "basic_string_view::remove_suffix greater than size.");
        length_ -= n;
    }

    void
    swap(basic_string_view& x)
    noexcept
    {
        std::swap(data_, x.data_);
        std::swap(length_, x.length_);
    }

    // Operations
    size_type
    copy(pointer dst, size_type count, size_type pos = 0)
    const
    {
        if (pos > size()) {
            throw std::out_of_range("basic_string_view::copy");
        }

        size_type length = std::min(count, size() - pos);
        traits_type::copy(dst, data() + pos, length * sizeof(value_type));

        return length;
    }

    basic_string_view
    substr(size_type pos = 0, size_type n = npos)
    const
    {
        if (pos > size()) {
            throw std::out_of_range("basic_string_view::substr");
        }

        return basic_string_view(data() + pos, min(n, size() - pos));
    }

    // Compare
    int
    compare(basic_string_view s)
    const noexcept
    {
        size_type lhs_size = size();
        size_type rhs_size = s.size();
        int result = traits_type::compare(data(), s.data(), std::min(lhs_size, rhs_size));
        if (result != 0) {
            return result;
        } else if (lhs_size < rhs_size) {
            return -1;
        } else if (lhs_size > rhs_size) {
            return 1;
        }
        return 0;
    }

    int
    compare(size_type pos1, size_type n1, basic_string_view s)
    const
    {
        return substr(pos1, n1).compare(s);
    }

    int
    compare(size_type pos1, size_type n1, basic_string_view s, size_type pos2, size_type n2)
    const
    {
        return substr(pos1, n1).compare(s.substr(pos2, n2));
    }

    int
    compare(const_pointer s)
    const noexcept
    {
        return compare(basic_string_view(s));
    }

    int
    compare(size_type pos1, size_type n1, const_pointer s)
    const
    {
        return substr(pos1, n1).compare(basic_string_view(s));
    }

    int
    compare(size_type pos1, size_type n1, const_pointer s, size_type n2)
    const
    {
        return substr(pos1, n1).compare(basic_string_view(s, n2));
    }

    // Starts with
    bool
    starts_with(basic_string_view x)
    const noexcept
    {
        return size() >= x.size() && compare(0, x.size(), x) == 0;
    }

    bool
    starts_with(value_type x)
    const noexcept
    {
        return starts_with(basic_string_view(&x, 1));
    }

    bool
    starts_with(const_pointer x)
    const noexcept
    {
        return starts_with(basic_string_view(x));
    }

    // Ends with
    bool
    ends_with(basic_string_view x)
    const noexcept
    {
        return size() >= x.size() && compare(size() - x.size(), npos, x) == 0;
    }

    bool
    ends_with(value_type x)
    const noexcept
    {
        return ends_with(basic_string_view(&x, 1));
    }

    bool
    ends_with(const_pointer x)
    const noexcept
    {
        return ends_with(basic_string_view(x));
    }

    // Find
    size_type
    find(basic_string_view s, size_type pos = 0)
    const noexcept
    {
        //  :copyright: (c) 2012-2015 Marshall Clow.
        //  :copyright: (c) 2015 Beman Dawes.
        //  :copyright: (c) 2017 Alex Huszagh.
        //  :license: Boost, see licenses/boost.md for more details.

        if (pos > size()) {
            return npos;
        }
        if (s.empty()) {
          return pos;
        }
        const_iterator iter = std::search(cbegin() + pos, cend(), s.cbegin(), s.cend(), traits_type::eq);
        return iter == cend() ? npos : std::distance(cbegin(), iter);
    }

    size_type
    find(value_type c, size_type pos = 0)
    const noexcept
    {
        return find(basic_string_view(&c, 1), pos);
    }

    size_type
    find(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return find(basic_string_view(s, n), pos);
    }

    size_type
    find(const_pointer s, size_type pos = 0)
    const noexcept
    {
        return find(basic_string_view(s), pos);
    }

    // Rfind
    size_type
    rfind(basic_string_view s, size_type pos = npos)
    const noexcept
    {
        //  :copyright: (c) 2012-2015 Marshall Clow.
        //  :copyright: (c) 2015 Beman Dawes.
        //  :copyright: (c) 2017 Alex Huszagh.
        //  :license: Boost, see licenses/boost.md for more details.

        if (size() < s.size()) {
            return npos;
        }
        if (pos > size() - s.size()) {
            pos = size() - s.size();
        }
        if (s.size() == 0) {
            return pos;
        }

        for (const_pointer it = data() + pos; ; --it) {
            if (traits_type::compare(it, s.data(), s.size()) == 0) {
                return it - data();
            } else if (it == data()) {
                return npos;
            }
        }
    }

    size_type
    rfind(value_type c, size_type pos = npos)
    const noexcept
    {
        return rfind(basic_string_view(&c, 1), pos);
    }

    size_type
    rfind(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return rfind(basic_string_view(s, n), pos);
    }

    size_type
    rfind(const_pointer s, size_type pos = npos)
    const noexcept
    {
        return rfind(basic_string_view(s), pos);
    }

    // Find first of
    size_type
    find_first_of(basic_string_view s, size_type pos = 0)
    const noexcept
    {
        //  :copyright: (c) 2012-2015 Marshall Clow.
        //  :copyright: (c) 2015 Beman Dawes.
        //  :copyright: (c) 2017 Alex Huszagh.
        //  :license: Boost, see licenses/boost.md for more details.

        if (pos >= size() || s.size() == 0) {
            return npos;
        }

        auto it = std::find_first_of(cbegin() + pos, cend(), s.cbegin(), s.cend(), traits_type::eq);
        return it == cend() ? npos : std::distance(cbegin(), it);
    }

    size_type
    find_first_of(value_type c, size_type pos = 0)
    const noexcept
    {
        return find_first_of(basic_string_view(&c, 1), pos);
    }

    size_type
    find_first_of(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return find_first_of(basic_string_view(s, n), pos);
    }

    size_type
    find_first_of(const_pointer s, size_type pos = 0)
    const noexcept
    {
        return find_first_of(basic_string_view(s), pos);
    }

    // Find last of
    size_type
    find_last_of(basic_string_view s, size_type pos = npos)
    const noexcept
    {
        //  :copyright: (c) 2012-2015 Marshall Clow.
        //  :copyright: (c) 2015 Beman Dawes.
        //  :copyright: (c) 2017 Alex Huszagh.
        //  :license: Boost, see licenses/boost.md for more details.

        if (s.size() == 0) {
            return npos;
        }
        if (pos >= size()) {
            pos = 0;
        } else {
            pos = size() - (pos+1);
        }

        auto iter = std::find_first_of(crbegin() + pos, crend(), s.cbegin(), s.cend(), traits_type::eq);
        if (iter == crend()) {
            return npos;
        }
        return size() - 1 - std::distance(crbegin(), iter);
    }

    size_type
    find_last_of(value_type c, size_type pos = npos)
    const noexcept
    {
        return find_last_of(basic_string_view(&c, 1), pos);
    }

    size_type
    find_last_of(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return find_last_of(basic_string_view(s, n), pos);
    }

    size_type
    find_last_of(const_pointer s, size_type pos = npos)
    const noexcept
    {
        return find_last_of(basic_string_view(s), pos);
    }

    // Find first not of
    size_type
    find_first_not_of(basic_string_view s, size_type pos = 0)
    const noexcept
    {
        //  :copyright: (c) 2012-2015 Marshall Clow.
        //  :copyright: (c) 2015 Beman Dawes.
        //  :copyright: (c) 2017 Alex Huszagh.
        //  :license: Boost, see licenses/boost.md for more details.

        if (pos >= size()) {
            return npos;
        }
        if (s.size() == 0) {
            return pos;
        }
        const_iterator iter = find_not_of_impl(cbegin() + pos, cend(), s);
        return iter == cend() ? npos : std::distance(cbegin(), iter);
    }

    size_type
    find_first_not_of(value_type c, size_type pos = 0)
    const noexcept
    {
        return find_first_not_of(basic_string_view(&c, 1), pos);
    }

    size_type
    find_first_not_of(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return find_first_not_of(basic_string_view(s, n), pos);
    }

    size_type
    find_first_not_of(const_pointer s, size_type pos = 0)
    const noexcept
    {
        return find_first_not_of(basic_string_view(s), pos);
    }

    // Find last not of
    size_type
    find_last_not_of(basic_string_view s, size_type pos = npos)
    const noexcept
    {
        //  :copyright: (c) 2012-2015 Marshall Clow.
        //  :copyright: (c) 2015 Beman Dawes.
        //  :copyright: (c) 2017 Alex Huszagh.
        //  :license: Boost, see licenses/boost.md for more details.

        if (pos >= size()) {
            pos = size() - 1;
        }
        if (s.size() == 0) {
            return pos;
        }
        pos = size() - (pos+1);
        const_reverse_iterator iter = find_not_of_impl(crbegin() + pos, crend(), s);

        if (iter == crend()) {
            return npos;
        }
        return size() - 1 - std::distance(crbegin(), iter);
    }

    size_type
    find_last_not_of(value_type c, size_type pos = npos)
    const noexcept
    {
        return find_last_not_of(basic_string_view(&c, 1), pos);
    }

    size_type
    find_last_not_of(const_pointer s, size_type pos, size_type n)
    const noexcept
    {
        return find_last_not_of(basic_string_view(s, n), pos);
    }

    size_type
    find_last_not_of(const_pointer s, size_type pos = npos)
    const noexcept
    {
        return find_last_not_of(basic_string_view(s), pos);
    }

private:
    const_pointer data_ = nullptr;
    size_t length_= 0;
};

// HELPERS

template <typename Iter, typename Char, typename Traits>
static
Iter
find_not_of_impl(Iter first, Iter last, basic_string_view<Char, Traits> s)
noexcept
{
    //  :copyright: (c) 2012-2015 Marshall Clow.
    //  :copyright: (c) 2015 Beman Dawes.
    //  :copyright: (c) 2017 Alex Huszagh.
    //  :license: Boost, see licenses/boost.md for more details.

    using traits_type = Traits;

    for (; first != last ; ++first) {
        if (traits_type::find(s.data(), s.size(), *first) == 0) {
            return first;
        }
    }
    return last;
}

template <typename Char, typename Traits>
const typename
basic_string_view<Char, Traits>::size_type
basic_string_view<Char, Traits>::npos;

// OPERATORS

template <typename Char, typename Traits>
bool
operator==(
    basic_string_view<Char, Traits> lhs,
    basic_string_view<Char, Traits> rhs
)
noexcept
{
    size_t lhs_size = lhs.size();
    return lhs_size == rhs.size() && Traits::compare(lhs.data(), rhs.data(), lhs_size) == 0;
}

template <typename Char, typename Traits>
bool
operator!=(
    basic_string_view<Char, Traits> lhs,
    basic_string_view<Char, Traits> rhs
)
noexcept
{
    return !(lhs == rhs);
}


template <typename Char, typename Traits>
bool
operator<(
    basic_string_view<Char, Traits> lhs,
    basic_string_view<Char, Traits> rhs
)
noexcept
{
    return lhs.compare(rhs) < 0;
}


template <typename Char, typename Traits>
bool
operator<=(
    basic_string_view<Char, Traits> lhs,
    basic_string_view<Char, Traits> rhs
)
noexcept
{
    return !(rhs < lhs);
}


template <typename Char, typename Traits>
bool
operator>(
    basic_string_view<Char, Traits> lhs,
    basic_string_view<Char, Traits> rhs
)
noexcept
{
    return rhs < lhs;
}


template <typename Char, typename Traits>
bool
operator>=(
    basic_string_view<Char, Traits> lhs,
    basic_string_view<Char, Traits> rhs
)
noexcept
{
    return !(lhs < rhs);
}


template <typename Char, typename Traits>
std::basic_ostream<Char, Traits>&
operator<<(
    std::basic_ostream<Char, Traits>& os,
    basic_string_view<Char, Traits> v
)
{
    return os.write(v.data(), v.length());
}

// TODO: specialize hash....
// This isn't easy....

}   /* contained */
