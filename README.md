# Contained

Stop. How many times have you heard the words "header-only library" in any other language? In how many other languages is "hours" the acceptable unit of time for compiling or interpreting large projects? Contained "contains" your standard containers.

## Why?

C++ fundamentally ties data and memory allocation together through the standard library, requiring you to know both the data and allocator type to access data via STL containers. Although this added complexity is fine for trivial examples, it scales very poorly to large codebases and interfaces.

You are maintaining a large, potentially proprietary code base. Users of your library must call a function, `extract_stock_data`, which takes a linked list with stock symbols and price ranges.

For example, your interface may look like:

```cpp
#ifndef EXTRACT_STOCK_DATA_HPP
#define EXTRACT_STOCK_DATA_HPP

#include <chrono>
#include <forward_list>
#include <vector>

using time_point = std::chrono::system_clock::time_point;

struct stock_input
{
    char symbol[6] = {0};   // stock symbols are max 5 characters.
    time_point start;
    time_point end;
};


struct stock_output
{
    char symbol[6] = {0};
    std::vector<double> prices;
};


std::forward_list<stock_output>
extract_stock_data(const std::list<stock_input>& input);

#endif
```

However, due to the poor performance of linked lists with standard allocators, a user wishes to use a custom pooled memory allocator with your library to improve performance. Unfortunately, the custom allocator does not match the function's type signature. 

You could include the function body in the header:

```cpp

template <typename T>
std::forward_list<stock_output>
extract_stock_data(const T& input)
{
    static_assert(
        std::is_same<typename T::value_type, stock_input>::value, 
        "Must pass valid stock data to our library."
    );

    // ...
}
```

This solution creates two issues: one, the exposed function is large, and required , and the code require includes headers from large libraries for networking, coroutines, and reading and writing JSON. Next, the code is proprietary, and you would prefer not to distribute the raw source code to your users.

You now have two possible solutions:

1. Ese a [polymorphic](http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2013/n3525.pdf) (virtual) allocator.
2. Use type-erasure to remove the allocator type from a subset of container operations independent of allocation (element access, iteration).

Polymorphic allocators (in C++17, under `<memory_resource>`) enable the use of diverse allocation strategies abstracted away through a virtual interface. 

However, the added overhead of virtual function calls may be intolerable for certain applications, most notably in game development. This is where non-virtual type-erasure comes in, by allowing data access independent of the allocator at compile-time.

Combining both simplifies the generation of C++ interfaces, improves compile times, with little to no run-time cost.

## Design

Each STL container is duplicated in the `contained` namespace, with one additional member function (`facet()`) and one additional member type ('facet_type'). Each facet supports all member functions independent of memory allocation or type construction, and guarantees all methods to access individual elements (`front()`, `at()`, ...), iterate over the container (`begin()`, `end()`, ...), and determine the container capacity (`size()`, `empty()`, ...) are part of the facet.

For example, our interface now looks like:

```cpp

std::forward_list<stock_output>
extract_stock_data(const list_facet<stock_input>& input);
```

## Getting Started

// TODO: document

## Documentation

// TODO: document

## Finer Details

For contiguous buffers, like `vector`,`deque`, and `string`, contained uses relocation whenever possible for performance optimizations (using `memcpy`, rather than move constructors).

## Installing

// TODO: document

## Contributors

Alex Huszagh

## License

Contained builds extensively off the LLVM/Clang infrastructure, and is distributed under the terms of the [MIT](LICENSE.md) license.

## Contact

Email: ahuszagh@gmail.com  
Twitter: @AlexHuszagh  
