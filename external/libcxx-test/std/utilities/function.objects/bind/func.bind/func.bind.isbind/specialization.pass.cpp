//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11

// <functional>

// template<class T> struct is_bind_expression;
//   A program may specialize this template for a program-defined type T
//   to have a base characteristic of true_type to indicate that T should
//   be treated as a subexpression in a bind call.
//   https://llvm.org/PR51753

#include <cassert>
#include <functional>
#include <type_traits>
#include <utility>
#include "libcxx_tc_common.h"

struct MyBind {
    int operator()(int x, int y) const { return 10*x + y; }
};
template<> struct std::is_bind_expression<MyBind> : std::true_type {};

int tc_utilities_function_objects_bind_func_bind_func_bind_isbind_specialization(void) {
  {
    auto f = [](auto x) { return 10*x + 9; };
    MyBind bindexpr;
    auto bound = std::bind(f, bindexpr);
    TC_ASSERT_EXPR(bound(7, 8) == 789);
  }
  {
    auto f = [](auto x) { return 10*x + 9; };
    const MyBind bindexpr;
    auto bound = std::bind(f, bindexpr);
    TC_ASSERT_EXPR(bound(7, 8) == 789);
  }
  {
    auto f = [](auto x) { return 10*x + 9; };
    MyBind bindexpr;
    auto bound = std::bind(f, std::move(bindexpr));
    TC_ASSERT_EXPR(bound(7, 8) == 789);
  }
  {
    auto f = [](auto x) { return 10*x + 9; };
    const MyBind bindexpr;
    auto bound = std::bind(f, std::move(bindexpr));
    TC_ASSERT_EXPR(bound(7, 8) == 789);
  }

  return 0;
}
