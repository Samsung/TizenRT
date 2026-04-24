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

// template <class... Types>
// template <class U1, class U2>
// constexpr explicit(see below) tuple<Types...>::tuple(pair<U1, U2>& u);

// Constraints:
// - sizeof...(Types) is 2 and
// - is_constructible_v<T0, decltype(get<0>(FWD(u)))> is true and
// - is_constructible_v<T1, decltype(get<1>(FWD(u)))> is true.

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

#include <cassert>
#include <tuple>
#include <utility>

#include "copy_move_types.h"
#include "libcxx_tc_common.h"

// test constraints
// sizeof...(Types) == 2
static_assert(std::is_constructible_v<std::tuple<MutableCopy, int>, std::pair<MutableCopy, int>&>);

static_assert(!std::is_constructible_v<std::tuple<MutableCopy>, std::pair<MutableCopy, int>&>);

static_assert(!std::is_constructible_v<std::tuple<MutableCopy, int, int>, std::pair<MutableCopy, int>&>);

// test constraints
// is_constructible_v<T0, decltype(get<0>(FWD(u)))> is true and
// is_constructible_v<T1, decltype(get<1>(FWD(u)))> is true.
static_assert(std::is_constructible_v<std::tuple<int, int>, std::pair<int, int>&>);

static_assert(!std::is_constructible_v<std::tuple<NoConstructorFromInt, int>, std::pair<int, int>&>);

static_assert(!std::is_constructible_v<std::tuple<int, NoConstructorFromInt>, std::pair<int, int>&>);

static_assert(!std::is_constructible_v< std::tuple<NoConstructorFromInt, NoConstructorFromInt>, std::pair<int, int>&>);

// test: The expression inside explicit is equivalent to:
// !is_convertible_v<decltype(get<0>(FWD(u))), T0> ||
// !is_convertible_v<decltype(get<1>(FWD(u))), T1>
static_assert(std::is_convertible_v<std::pair<MutableCopy, MutableCopy>&,
                                    std::tuple<ConvertibleFrom<MutableCopy>, ConvertibleFrom<MutableCopy>>>);

static_assert(!std::is_convertible_v<std::pair<MutableCopy, MutableCopy>&,
                                     std::tuple<ExplicitConstructibleFrom<MutableCopy>, ConvertibleFrom<MutableCopy>>>);

static_assert(!std::is_convertible_v<std::pair<MutableCopy, MutableCopy>&,
                                     std::tuple<ConvertibleFrom<MutableCopy>, ExplicitConstructibleFrom<MutableCopy>>>);

constexpr bool test() {
  // test implicit conversions.
  {
    std::pair<MutableCopy, int> p{1, 2};
    std::tuple<ConvertibleFrom<MutableCopy>, ConvertibleFrom<int>> t = p;
    TC_ASSERT_EXPR(std::get<0>(t).v.val == 1);
    TC_ASSERT_EXPR(std::get<1>(t).v == 2);
  }

  // test explicit conversions.
  {
    std::pair<MutableCopy, int> p{1, 2};
    std::tuple<ExplicitConstructibleFrom<MutableCopy>, ExplicitConstructibleFrom<int>> t{p};
    TC_ASSERT_EXPR(std::get<0>(t).v.val == 1);
    TC_ASSERT_EXPR(std::get<1>(t).v == 2);
  }

  // non const overload should be called
  {
    std::pair<TracedCopyMove, TracedCopyMove> p;
    std::tuple<ConvertibleFrom<TracedCopyMove>, TracedCopyMove> t = p;
    TC_ASSERT_EXPR(nonConstCopyCtrCalled(std::get<0>(t).v));
    TC_ASSERT_EXPR(nonConstCopyCtrCalled(std::get<1>(t)));
  }

  return true;
}

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_non_const_pair(void) {
  test();
  static_assert(test());

  return 0;
}
