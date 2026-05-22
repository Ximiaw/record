# record — Type-Safe C++20 Heterogeneous Record Container

[English](README.md) | [中文](README_zh.md)

A lightweight C++20-based heterogeneous record implementation that provides **type-safe** runtime index access, supports duplicate types, copy/move semantics, and full const correctness.

## Features

- **Type-Safe Runtime Access** — Access elements by index at runtime via `get<T>(index)`, with both bounds checking and type matching verification
- **Duplicate Type Support** — Allows identical types in the template parameter list (e.g., `record<int, int, int>`), distinguished precisely by index
- **Zero-Cost Abstraction** — Uses `std::tuple` for underlying storage and achieves O(1) element lookup via a pointer array
- **C++20 Concepts Constraints** — Leverages concepts to constrain template parameters at compile time, yielding clear compilation error messages
- **Full Value Semantics** — Supports copy construction, copy assignment, move construction, and move assignment, all with exception-safe implementations

## Requirements

- **Compiler**: C++20-compliant compiler (tested only with GCC 13)
- **Build Tool**: CMake 3.10 or higher
- **Test Framework**: Google Test (GTest)

## File Overview

| File | Description |
|------|-------------|
| `record.hpp` | Core template class implementation |
| `test.cpp` | Comprehensive unit tests based on Google Test |
| `CMakeLists.txt` | CMake build configuration |

## Quick Start

### Build & Test

```bash
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

### Usage Example

```cpp
#include "record.hpp"
#include <string>
#include <iostream>

int main() {
    // Construct a record with multiple types
    record<int, double, std::string> r{42, 3.14, "hello"};

    // Type-safe runtime access
    std::cout << r.get<int>(0) << "\n";         // 42
    std::cout << r.get<double>(1) << "\n";      // 3.14
    std::cout << r.get<std::string>(2) << "\n"; // hello

    // Duplicate types supported
    record<int, double, int> rd{100, 2.5, 200};
    std::cout << rd.get<int>(0) << "\n";        // 100
    std::cout << rd.get<int>(2) << "\n";        // 200

    // Mutable access
    r.get<int>(0) = 99;

    // Get record size
    std::cout << r.size() << "\n"; // 3

    // Type alias
    record<int, double>::type<0> val = 10; // int
}
```

## API Reference

### Constructors

| Constructor | Description |
|-------------|-------------|
| `record()` | Default construction; all members value-initialized |
| `record(Args&&... args)` | Explicit value construction; arguments must be convertible to the corresponding types |
| `record(const record& other)` | Copy constructor |
| `record(record&& other)` | Move constructor |

### Element Access

| Method | Description |
|--------|-------------|
| `const T& get<T>(size_t i) const` | Const access to the `i`-th element; type must be `T` |
| `T& get<T>(size_t i)` | Mutable access to the `i`-th element; type must be `T` |

### Other Operations

| Method | Description |
|--------|-------------|
| `size_t size() const` | Returns the number of fields |
| `type<N>` | Template alias representing the type of the `N`-th field |

### Exception Specification

| Exception Type | Trigger Condition |
|----------------|-------------------|
| `std::out_of_range` | Index `i` is outside the range `[0, size())` |
| `std::runtime_error` | Template type `T` does not match the actual type at position `i` |

## Test Coverage

The test file `test.cpp` includes the following test categories:

- **Default Construction** — Empty record construction, copy and move
- **Explicit Value Construction** — Construction with basic types, multiple types, and convertible types
- **Duplicate Types** — Correctness when identical types appear consecutively or interleaved
- **Exception Safety** — Out-of-bounds access, type mismatch, exception precedence
- **Copy Semantics** — Copy construction, copy assignment, self-assignment safety
- **Move Semantics** — Move construction, move assignment, self-move safety
- **Type System** — Const correctness, complex types (`std::vector`, `std::unique_ptr`)

## Implementation Highlights

The core design of `record` lies in maintaining two data structures simultaneously:

1. `std::tuple<Types...>` — Actual data storage ensuring compact memory layout and standard layout
2. `std::array<void*, sizeof...(Types)>` — Stores pointers to each tuple element, enabling O(1) runtime index lookup

Combined with the compile-time generated type marker array `is_type_at<T>`, runtime type matching verification is achieved, delivering type safety while maintaining high performance.

## License

MIT License  
Copyright (c) 2026 Ximiaw
