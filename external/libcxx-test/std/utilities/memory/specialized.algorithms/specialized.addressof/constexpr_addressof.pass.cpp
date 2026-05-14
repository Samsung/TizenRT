//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14

// <memory>

// template <ObjectType T> constexpr T* addressof(T& r);

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct Pointer {
  constexpr Pointer(void* v) : value(v) {}
  void* value;
};

struct A
{
    constexpr A() : n(42) {}
    void operator&() const { }
    int n;
};

constexpr int i = 0;
constexpr double d = 0.0;
constexpr A a{};

int tc_utilities_memory_specialized_algorithms_specialized_addressof_constexpr_addressof(void) {
    static_assert(std::addressof(i) == &i, "");
    static_assert(std::addressof(d) == &d, "");
    constexpr const A* ap = std::addressof(a);
    static_assert(&ap->n == &a.n, "");

  return 0;
}
