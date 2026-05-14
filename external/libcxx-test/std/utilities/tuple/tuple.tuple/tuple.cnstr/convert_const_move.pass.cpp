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
// template <class... UTypes>
//   constexpr explicit(see below) tuple<Types>::tuple(const
//   tuple<UTypes...>&&);
//
// Constraints:
//  sizeof...(Types) equals sizeof...(UTypes) &&
//  (is_constructible_v<Types, decltype(get<I>(FWD(u)))> && ...) is true &&
//  (
//    sizeof...(Types) is not 1 ||
//    (
//      !is_convertible_v<decltype(u), T> &&
//      !is_constructible_v<T, decltype(u)> &&
//      !is_same_v<T, U>
//    )
//  )

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

#include <cassert>
#include <tuple>

#include "copy_move_types.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

// test: The expression inside explicit is equivalent to:
// !(is_convertible_v<decltype(get<I>(FWD(u))), Types> && ...)
static_assert(std::is_convertible_v<const std::tuple<ConstMove>&&, std::tuple<ConvertibleFrom<ConstMove>>>);

static_assert(std::is_convertible_v<const std::tuple<ConstMove, ConstMove>&&,
                                    std::tuple<ConvertibleFrom<ConstMove>, ConvertibleFrom<ConstMove>>>);

static_assert(
    !std::is_convertible_v<const std::tuple<MutableCopy>&&, std::tuple<ExplicitConstructibleFrom<ConstMove>>>);

static_assert(!std::is_convertible_v<const std::tuple<ConstMove, ConstMove>&&,
                                     std::tuple<ConvertibleFrom<ConstMove>, ExplicitConstructibleFrom<ConstMove>>>);

constexpr bool test() {
  // test implicit conversions.
  // sizeof...(Types) == 1
  {
    const std::tuple<ConstMove> t1{1};
    std::tuple<ConvertibleFrom<ConstMove>> t2 = std::move(t1);
    TC_ASSERT_EXPR(std::get<0>(t2).v.val == 1);
  }

  // test implicit conversions.
  // sizeof...(Types) > 1
  {
    const std::tuple<ConstMove, int> t1{1, 2};
    std::tuple<ConvertibleFrom<ConstMove>, int> t2 = std::move(t1);
    TC_ASSERT_EXPR(std::get<0>(t2).v.val == 1);
    TC_ASSERT_EXPR(std::get<1>(t2) == 2);
  }

  // test explicit conversions.
  // sizeof...(Types) == 1
  {
    const std::tuple<ConstMove> t1{1};
    std::tuple<ExplicitConstructibleFrom<ConstMove>> t2{std::move(t1)};
    TC_ASSERT_EXPR(std::get<0>(t2).v.val == 1);
  }

  // test explicit conversions.
  // sizeof...(Types) > 1
  {
    const std::tuple<ConstMove, int> t1{1, 2};
    std::tuple<ExplicitConstructibleFrom<ConstMove>, int> t2{std::move(t1)};
    TC_ASSERT_EXPR(std::get<0>(t2).v.val == 1);
    TC_ASSERT_EXPR(std::get<1>(t2) == 2);
  }

  // test constraints

  // sizeof...(Types) != sizeof...(UTypes)
  static_assert(!std::is_constructible_v<std::tuple<int, int>, const std::tuple<int>&&>);
  static_assert(!std::is_constructible_v<std::tuple<int, int, int>, const std::tuple<int, int>&&>);

  // !(is_constructible_v<Types, decltype(get<I>(FWD(u)))> && ...)
  static_assert(!std::is_constructible_v<std::tuple<int, NoConstructorFromInt>, const std::tuple<int, int>&&>);

  // sizeof...(Types) == 1 && other branch of "||" satisfied
  {
    const std::tuple<TracedCopyMove> t1{};
    std::tuple<ConvertibleFrom<TracedCopyMove>> t2{std::move(t1)};
    TC_ASSERT_EXPR(constMoveCtrCalled(std::get<0>(t2).v));
  }

  // sizeof...(Types) == 1 && is_same_v<T, U>
  {
    const std::tuple<TracedCopyMove> t1{};
    std::tuple<TracedCopyMove> t2{t1};
    TC_ASSERT_EXPR(!constMoveCtrCalled(std::get<0>(t2)));
  }

  // sizeof...(Types) != 1
  {
    const std::tuple<TracedCopyMove, TracedCopyMove> t1{};
    std::tuple<TracedCopyMove, TracedCopyMove> t2{std::move(t1)};
    TC_ASSERT_EXPR(constMoveCtrCalled(std::get<0>(t2)));
  }

  // These two test points cause gcc to ICE
#if !defined(TEST_COMPILER_GCC)
  // sizeof...(Types) == 1 && is_convertible_v<decltype(u), T>
  {
    const std::tuple<CvtFromConstTupleRefRef> t1{};
    std::tuple<ConvertibleFrom<CvtFromConstTupleRefRef>> t2{std::move(t1)};
    TC_ASSERT_EXPR(!constMoveCtrCalled(std::get<0>(t2).v));
  }

  // sizeof...(Types) == 1 && is_constructible_v<decltype(u), T>
  {
    const std::tuple<ExplicitCtrFromConstTupleRefRef> t1{};
    std::tuple<ConvertibleFrom<ExplicitCtrFromConstTupleRefRef>> t2{std::move(t1)};
    TC_ASSERT_EXPR(!constMoveCtrCalled(std::get<0>(t2).v));
  }
#endif

  return true;
}

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_convert_const_move(void) {
  test();
  static_assert(test());
  return 0;
}
