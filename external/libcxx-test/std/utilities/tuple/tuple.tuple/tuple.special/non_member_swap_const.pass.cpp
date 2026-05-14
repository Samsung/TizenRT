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
// <tuple>

// template <class... Types> class tuple;

// template <class... Types>
//   void swap(const tuple<Types...>& x, const tuple<Types...>& y);

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

#include <tuple>
#include <cassert>
#include "libcxx_tc_common.h"

struct S {
  int* calls;
  friend constexpr void swap(S& a, S& b) {
    *a.calls += 1;
    *b.calls += 1;
  }
};
struct CS {
  int* calls;
  friend constexpr void swap(const CS& a, const CS& b) {
    *a.calls += 1;
    *b.calls += 1;
  }
};

static_assert(std::is_swappable_v<std::tuple<>>);
static_assert(std::is_swappable_v<std::tuple<S>>);
static_assert(std::is_swappable_v<std::tuple<CS>>);
static_assert(std::is_swappable_v<std::tuple<S&>>);
static_assert(std::is_swappable_v<std::tuple<CS, S>>);
static_assert(std::is_swappable_v<std::tuple<CS, S&>>);
static_assert(std::is_swappable_v<const std::tuple<>>);
static_assert(!std::is_swappable_v<const std::tuple<S>>);
static_assert(std::is_swappable_v<const std::tuple<CS>>);
static_assert(std::is_swappable_v<const std::tuple<S&>>);
static_assert(!std::is_swappable_v<const std::tuple<CS, S>>);
static_assert(std::is_swappable_v<const std::tuple<CS, S&>>);

constexpr bool test() {
  int cs_calls = 0;
  int s_calls = 0;
  S s1{&s_calls};
  S s2{&s_calls};
  const std::tuple<CS, S&> t1 = {CS{&cs_calls}, s1};
  const std::tuple<CS, S&> t2 = {CS{&cs_calls}, s2};
  swap(t1, t2);
  TC_ASSERT_EXPR(cs_calls == 2);
  TC_ASSERT_EXPR(s_calls == 2);

  return true;
}

int tc_utilities_tuple_tuple_tuple_tuple_special_non_member_swap_const(void) {
  test();
  static_assert(test());

  return 0;
}
