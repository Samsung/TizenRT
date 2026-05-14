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

// void swap(const tuple& rhs);

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

#include <cassert>
#include <tuple>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

#ifndef TEST_HAS_NO_EXCEPTIONS
class SwapThrower {
  void swap(SwapThrower&) = delete;
  void swap(const SwapThrower&) const = delete;
};

void swap(const SwapThrower&, const SwapThrower&) { throw 0.f; }

static_assert(std::is_swappable_v<const SwapThrower>);
static_assert(std::is_swappable_with_v<const SwapThrower&, const SwapThrower&>);

void test_noexcept() {
  const std::tuple<SwapThrower> t1;
  const std::tuple<SwapThrower> t2;

#ifndef _LIBCPP_NO_EXCEPTIONS
  try {
    t1.swap(t2);
    std::swap(t1, t2);
    TC_ASSERT_EXPR(false);
  } catch (float) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS

#ifndef _LIBCPP_NO_EXCEPTIONS
  try {
    std::swap(std::as_const(t1), std::as_const(t2));
    TC_ASSERT_EXPR(false);
  } catch (float) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
}
#endif // TEST_HAS_NO_EXCEPTIONS

struct ConstSwappable {
  mutable int i;
};

constexpr void swap(const ConstSwappable& lhs, const ConstSwappable& rhs) { std::swap(lhs.i, rhs.i); }

constexpr bool test() {
  {
    typedef std::tuple<const ConstSwappable> T;
    const T t0(ConstSwappable{0});
    T t1(ConstSwappable{1});
    t0.swap(t1);
    TC_ASSERT_EXPR(std::get<0>(t0).i == 1);
    TC_ASSERT_EXPR(std::get<0>(t1).i == 0);
  }
  {
    typedef std::tuple<ConstSwappable, ConstSwappable> T;
    const T t0({0}, {1});
    const T t1({2}, {3});
    t0.swap(t1);
    TC_ASSERT_EXPR(std::get<0>(t0).i == 2);
    TC_ASSERT_EXPR(std::get<1>(t0).i == 3);
    TC_ASSERT_EXPR(std::get<0>(t1).i == 0);
    TC_ASSERT_EXPR(std::get<1>(t1).i == 1);
  }
  {
    typedef std::tuple<ConstSwappable, const ConstSwappable, const ConstSwappable> T;
    const T t0({0}, {1}, {2});
    const T t1({3}, {4}, {5});
    t0.swap(t1);
    TC_ASSERT_EXPR(std::get<0>(t0).i == 3);
    TC_ASSERT_EXPR(std::get<1>(t0).i == 4);
    TC_ASSERT_EXPR(std::get<2>(t0).i == 5);
    TC_ASSERT_EXPR(std::get<0>(t1).i == 0);
    TC_ASSERT_EXPR(std::get<1>(t1).i == 1);
    TC_ASSERT_EXPR(std::get<2>(t1).i == 2);
  }
  return true;
}

int tc_utilities_tuple_tuple_tuple_tuple_swap_member_swap_const(void) {
#ifndef TEST_HAS_NO_EXCEPTIONS
  test_noexcept();
#endif
  test();

// gcc cannot have mutable member in constant expression
#if !defined(TEST_COMPILER_GCC)
  static_assert(test());
#endif

  return 0;
}
