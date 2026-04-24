/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
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
