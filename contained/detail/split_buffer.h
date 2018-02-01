//  :copyright: (c) 2009-2017 LLVM Team.
//  :copyright: (c) 2017-2018 Alex Huszagh.
//  :license: MIT, see licenses/mit.md for more details.
/**
 *  \addtogroup PySTD
 *  \brief Split buffer type to facilitate container implementations.
 *
 *  Highly optimized buffer with leading and trailing uninitialized
 *  values, making it useful for deque and vector operations.
 *
 *  \synopsis
 // TODO: implement
 */

#pragma once

#include <contained/detail/compressed_pair.h>
#include <contained/detail/growth.h>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <iterator>
#include <memory>
#include <ratio>
#include <type_traits>

namespace contained
{
// OBJECTS
// -------

// SPLICE BUFFER FACET

template <typename T, typename VoidPtr = void*>
class split_buffer
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
    split_buffer()
    noexcept:
        first_(nullptr),
        begin_(nullptr),
        end_(nullptr),
        end_cap_(nullptr)
    {}

    template <typename Allocator>
    split_buffer(
        size_type cap,
        size_type start,
        Allocator& alloc
    ):
        first_(cap != 0 ? alloc.allocate(cap) : nullptr),
        begin_(first_ + start),
        end_(first_ + start),
        end_cap_(first_ + cap)
    {}

    split_buffer(const split_buffer&) = delete;

    split_buffer(split_buffer&& x):
        first_(std::move(x.first_)),
        begin_(std::move(x.begin_)),
        end_(std::move(x.end_)),
        end_cap_(std::move(x.end_cap_))
    {
        x.first_ = x.begin_ = x.end_ = x.end_cap_ = nullptr;
    }

    ~split_buffer()
    {}

    // Assignment
    split_buffer&
    operator=(split_buffer&& x)
    {
        first_ = x.first_;
        begin_ = x.begin_;
        end_ = x.end_;
        end_cap_ = x.end_cap_;
        x.first_ = x.begin_ = x.end_ = x.end_cap_ = nullptr;
        return *this;
    }

    split_buffer& operator=(const split_buffer&) = delete;

    // Iterators
    iterator
    begin()
    noexcept
    {
        return begin_;
    }

    const_iterator
    begin()
    const noexcept
    {
        return begin_;
    }

    iterator
    end()
    noexcept
    {
        return end_;
    }

    const_iterator
    end()
    const noexcept
    {
        return end_;
    }

    // Capacity
    size_type
    size()
    const
    {
        return static_cast<size_type>(end_ - begin_);
    }

    bool
    empty()
    const
    {
        return end_ == begin_;
    }

    size_type
    capacity()
    const
    {
        return static_cast<size_type>(end_cap_ - first_);
    }

    size_type
    front_spare()
    const
    {
        return static_cast<size_type>(begin_ - first_);
    }

    size_type
    has_front_spare()
    const
    {
        return first_ != begin_;
    }

    size_type
    back_spare()
    const
    {
        return static_cast<size_type>(end_cap_ - end_);
    }

    size_type
    has_back_spare()
    const
    {
        return end_ != end_cap_;
    }

    template <typename Allocator>
    void
    reserve(Allocator& alloc, size_type n)
    {
        if (n < capacity()) {
            internal_resize(alloc, n, 0);
        }
    }

    template <typename Allocator>
    void
    shrink_to_fit(Allocator& alloc)
    noexcept
    {
        if (capacity() > size()) {
            try {
                internal_resize(alloc, size(), 0);
            } catch (...) {
            }
        }
    }

    // Internal resize, does not default initialize any values.
    template <typename Allocator>
    void
    internal_resize(Allocator& alloc, size_type new_size, size_type new_offset)
    {
        using alloc_traits = std::allocator_traits<Allocator>;

        // get parameters
        assert(new_size > new_offset && "Buffer overflow.");
        size_type old_offset = front_spare();
        size_type sz = size();
        size_type count = std::min(new_size-new_offset, sz);

        // reallocate and calculate new buffer positions
        pointer new_first = alloc_traits::reallocate(alloc, first_, capacity(), new_size, count, old_offset, new_offset);
        pointer new_begin = new_first + new_offset;
        pointer new_end = new_begin + sz;
        pointer new_cap = new_first + new_size;

        // set new buffer
        first_ = new_first;
        begin_ = new_begin;
        end_ = new_end;
        end_cap_ = new_cap;
    }

    // Guess default size and offset
    template <typename Allocator, bool AlignBack = false>
    void
    internal_resize(Allocator& alloc)
    {
        // get the ratio for our growth
        constexpr double ratio = CONTAINED_GROWTH_RATIO;
        constexpr unsigned inum = AlignBack ? 3 : 0;

        // get reallocation size, max(ratio * capacity(), 1);
        // internally resize and align data from 25%-75% of new buffer
        size_type c = std::max<size_type>(ceil(ratio * capacity()), 1);
        internal_resize(alloc, c, (c + inum) / 4);
    }

    // Element access
    reference
    front()
    {
        return *begin_;
    }

    const_reference
    front()
    const
    {
        return *begin_;
    }

    reference
    back()
    {
        return *(end_ - 1);
    }

    const_reference
    back()
    const
    {
        return *(end_ - 1);
    }

    // Modifiers
//    void
//    internal_move_back()
//    {
//        // move halfway (incrementing up) to the back
//        difference_type d = end_cap_ - end_;
//        d = (d + 1) / 2;
//
//        // relocate in the overlapping range, and reset views
//        // TODO: relocate replaces
//        // __begin_ = _VSTD::move_backward(__begin_, __end_, __end_ + __d);
//        relocate(begin_, end_, begin_+d);
//        begin_ += d;
//        end_ += d;
//    }
//
//    void
//    internal_move_front()
//    {
//        // Moving forward...
//        difference_type d = begin_ - first_;
//        d = (d + 1) / 2;
//
//        // relocate in the overlapping range, and reset views
//        // TODO: relocate replaces
//        // __begin_ = _VSTD::move_backward(__begin_, __end_, __end_ + __d);
//        relocate(begin_, end_, begin_-d);
//        begin_ -= d;
//        end_ -= d;
//    }

    template <typename Allocator>
    void
    shift_back(Allocator& alloc)
    {
        if (has_back_spare()) {
            internal_move_back(alloc);
        } else {
            internal_resize<true>(alloc);
        }
    }

    template <typename Allocator>
    void
    check_shift_back(Allocator& alloc)
    {
        if (!has_front_spare()) {
            shift_back(alloc);
        }
    }

    template <typename Allocator>
    void
    shift_front(Allocator& alloc)
    {
        if (has_front_spare()) {
            internal_move_front(alloc);
        } else {
            internal_resize<false>(alloc);
        }
    }

    template <typename Allocator>
    void
    check_shift_front(Allocator& alloc)
    {
        if (!has_back_spare()) {
            shift_front(alloc);
        }
    }

    void swap(split_buffer& x)
    noexcept
    {
        std::swap(first_, x.first_);
        std::swap(begin_, x.begin_);
        std::swap(end_, x.end_);
        std::swap(end_cap_, x.end_cap_);
    }

    template <typename Allocator>
    void
    clear(Allocator& alloc)
    noexcept
    {
        destruct_at_end(alloc, begin_);
    }

    template <typename Allocator>
    void
    pop_front(Allocator& alloc)
    {
        destruct_at_begin(alloc, begin_+1);
    }

    template <typename Allocator>
    void
    pop_back(Allocator& alloc)
    {
        destruct_at_end(alloc, end_-1);
    }


    template <typename Allocator>
    void
    push_front(Allocator& alloc, const_reference v)
    {
        using alloc_traits = std::allocator_traits<Allocator>;

        check_shift_back(alloc);
        alloc_traits::construct(alloc, to_raw_pointer(begin_-1), v);
        --begin_;
    }

    template <typename Allocator>
    void
    push_front(Allocator& alloc, value_type&& v)
    {
        using alloc_traits = std::allocator_traits<Allocator>;

        check_shift_back();
        alloc_traits::construct(alloc, to_raw_pointer(begin_-1), std::move(v));
        --begin_;
    }

    template <typename Allocator, typename ... Ts>
    void
    emplace_front(Allocator& alloc, Ts&&... ts)
    {
        using alloc_traits = std::allocator_traits<Allocator>;

        check_shift_back();
        alloc_traits::construct(alloc(), to_raw_pointer(begin_-1), std::forward<Ts>(ts)...);
        --begin_;
    }

    template <typename Allocator>
    void
    push_back(Allocator& alloc, const_reference v)
    {
        using alloc_traits = std::allocator_traits<Allocator>;

        check_shift_front();
        alloc_traits::construct(alloc, to_raw_pointer(end_), v);
        ++end_;
    }

    template <typename Allocator>
    void
    push_back(Allocator& alloc, value_type&& v)
    {
        using alloc_traits = std::allocator_traits<Allocator>;

        check_shift_front();
        alloc_traits::construct(alloc, to_raw_pointer(end_), std::move(v));
        ++end_;
    }

    template <typename Allocator, typename ... Ts>
    void
    emplace_back(Allocator& alloc, Ts&&... ts)
    {
        using alloc_traits = std::allocator_traits<Allocator>;

        check_shift_front();
        alloc_traits::construct(alloc, to_raw_pointer(end_), std::forward<Ts>(ts)...);
        ++end_;
    }

    // Construct at
    template <typename Allocator>
    void
    construct_at_end(Allocator& alloc, size_type n)
    {
        using alloc_traits = std::allocator_traits<Allocator>;

        do {
            alloc_traits::construct(alloc, to_raw_pointer(end_));
            ++end_;
            --n;
        } while (n > 0);
    }

    template <typename Allocator>
    void
    construct_at_end(Allocator& alloc, size_type n, const_reference v)
    {
        using alloc_traits = std::allocator_traits<Allocator>;

        do {
            alloc_traits::construct(alloc, to_raw_pointer(end_), v);
            ++end_;
            --n;
        } while (n > 0);
    }

    // Destruct at
    template <typename Allocator>
    void
    destruct_at_begin(Allocator& alloc, pointer new_begin)
    {
        destruct_at_begin(alloc, new_begin, std::is_trivial<value_type>());
    }

    template <typename Allocator>
    void
    destruct_at_begin(Allocator& alloc, pointer new_begin, std::false_type)
    {
        using alloc_traits = std::allocator_traits<Allocator>;
        while (begin_ != new_begin) {
            alloc_traits::destroy(alloc, to_raw_pointer(begin_++));
        }
    }

    template <typename Allocator>
    void
    destruct_at_begin(Allocator& alloc, pointer new_begin, std::true_type)
    {
        begin_ = new_begin;
    }

    template <typename Allocator>
    void
    destruct_at_end(Allocator& alloc, pointer new_last)
    noexcept
    {
        destruct_at_end(alloc, new_last, std::is_trivial<value_type>());
    }

    template <typename Allocator>
    void
    destruct_at_end(Allocator& alloc, pointer new_last, std::false_type
    )
    noexcept
    {
        using alloc_traits = std::allocator_traits<Allocator>;
        while (end_ != new_last) {
            alloc_traits::destroy(alloc, to_raw_pointer(end_--));
        }
    }

    template <typename Allocator>
    void
    destruct_at_end(Allocator& alloc, pointer new_last, std::true_type
    )
    noexcept
    {
        end_ = new_last;
    }

private:
    pointer first_;
    pointer begin_;
    pointer end_;
    pointer end_cap_;
};

}   /* contained */
