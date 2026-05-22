# record — 类型安全的 C++20 异构记录容器

[English](README.md) | [中文](README_zh.md)

一个基于 C++20 的轻量级异构记录（heterogeneous record）实现，提供**类型安全**的运行时索引访问，支持重复类型、拷贝/移动语义以及完整的 const 正确性。

## 特性

- **类型安全的运行时访问** — 通过 `get<T>(index)` 在运行时按索引访问元素，同时进行越界检查和类型匹配检查
- **支持重复类型** — 允许模板参数列表中出现相同类型（如 `record<int, int, int>`），通过索引精确区分
- **零开销抽象** — 底层使用 `std::tuple` 存储数据，通过指针数组实现 O(1) 的元素定位
- **C++20 Concepts 约束** — 利用概念（concepts）在编译期对模板参数进行约束，提供清晰的编译错误信息
- **完整的值语义** — 支持拷贝构造、拷贝赋值、移动构造、移动赋值，且均为异常安全实现

## 环境要求

- **编译器**：支持 C++20 的编译器（仅测试了GCC 13）
- **构建工具**：CMake 3.10 或更高版本
- **测试框架**：Google Test（GTest）

## 文件说明

| 文件 | 说明 |
|------|------|
| `record.hpp` | 核心模板类实现 |
| `test.cpp` | 基于 Google Test 的完整单元测试 |
| `CMakeLists.txt` | CMake 构建配置 |

## 快速开始

### 构建与测试

```bash
mkdir build && cd build
cmake ..
cmake --build .
ctest --output-on-failure
```

### 使用示例

```cpp
#include "record.hpp"
#include <string>
#include <iostream>

int main() {
    // 构造一个包含多种类型的记录
    record<int, double, std::string> r{42, 3.14, "hello"};

    // 类型安全的运行时访问
    std::cout << r.get<int>(0) << "\n";        // 42
    std::cout << r.get<double>(1) << "\n";     // 3.14
    std::cout << r.get<std::string>(2) << "\n"; // hello

    // 支持重复类型
    record<int, double, int> rd{100, 2.5, 200};
    std::cout << rd.get<int>(0) << "\n";       // 100
    std::cout << rd.get<int>(2) << "\n";       // 200

    // 可变访问
    r.get<int>(0) = 99;

    // 获取记录大小
    std::cout << r.size() << "\n"; // 3

    // 类型别名
    record<int, double>::type<0> val = 10; // int
}
```

## API 参考

### 构造

| 构造函数 | 说明 |
|----------|------|
| `record()` | 默认构造，所有成员值初始化 |
| `record(Args&&... args)` | 显式值构造，要求参数可转换到对应类型 |
| `record(const record& other)` | 拷贝构造 |
| `record(record&& other)` | 移动构造 |

### 元素访问

| 方法 | 说明 |
|------|------|
| `const T& get<T>(size_t i) const` | const 访问第 `i` 个元素，类型必须为 `T` |
| `T& get<T>(size_t i)` | 可变访问第 `i` 个元素，类型必须为 `T` |

### 其他操作

| 方法 | 说明 |
|------|------|
| `size_t size() const` | 返回字段数量 |
| `type<N>` | 模板别名，表示第 `N` 个字段的类型 |

### 异常说明

| 异常类型 | 触发条件 |
|----------|----------|
| `std::out_of_range` | 索引 `i` 超出 `[0, size())` 范围 |
| `std::runtime_error` | 模板类型 `T` 与第 `i` 个位置的实际类型不匹配 |

## 测试覆盖

测试文件 `test.cpp` 包含以下测试类别：

- **默认构造** — 空记录构造、拷贝与移动
- **显式值构造** — 基本类型、多类型、可转换类型构造
- **重复类型** — 相同类型连续/交错出现时的正确性
- **异常安全** — 越界访问、类型不匹配、异常优先级
- **拷贝语义** — 拷贝构造、拷贝赋值、自赋值安全
- **移动语义** — 移动构造、移动赋值、自移动安全
- **类型系统** — const 正确性、复杂类型（`std::vector`、`std::unique_ptr`）

## 实现要点

`record` 的核心设计在于同时维护两份数据结构：

1. `std::tuple<Types...>` — 实际存储数据，保证内存紧凑和标准布局
2. `std::array<void*, sizeof...(Types)>` — 存储指向各 tuple 元素的指针，支持运行时的 O(1) 索引定位

配合编译期生成的类型标记数组 `is_type_at<T>`，可在运行时完成类型匹配校验，从而在保持高效的同时实现类型安全。

## 许可证

MIT License  
Copyright (c) 2026 Ximiaw