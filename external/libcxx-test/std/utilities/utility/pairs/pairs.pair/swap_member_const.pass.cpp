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
// <utility>

// template <class T1, class T2> struct pair
// void swap(const pair& p) const;

// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

#include <cassert>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

// Remarks: The expression inside noexcept is equivalent to
// is_nothrow_swappable_v<const first_type> && is_nothrow_swappable_v<const second_type> for the second overload.
template <class T>
concept ConstMemberSwapNoexcept =
    requires(const T& t1, const T& t2) {
      { t1.swap(t2) } noexcept;
    };

template <bool canThrow>
struct SwapMayThrow {};

template <bool canThrow>
void swap(const SwapMayThrow<canThrow>&, const SwapMayThrow<canThrow>&) noexcept(!canThrow);

static_assert(ConstMemberSwapNoexcept<std::pair<SwapMayThrow<false>, SwapMayThrow<false>>>);
static_assert(!ConstMemberSwapNoexcept<std::pair<SwapMayThrow<true>, SwapMayThrow<false>>>);
static_assert(!ConstMemberSwapNoexcept<std::pair<SwapMayThrow<false>, SwapMayThrow<true>>>);
static_assert(!ConstMemberSwapNoexcept<std::pair<SwapMayThrow<true>, SwapMayThrow<true>>>);

struct ConstSwappable {
  mutable int i;
  friend constexpr void swap(const ConstSwappable& lhs, const ConstSwappable& rhs) { std::swap(lhs.i, rhs.i); }
};

constexpr bool test() {
  // user defined const swap
  {
    using P = std::pair<const ConstSwappable, const ConstSwappable>;
    const P p1(ConstSwappable{0}, ConstSwappable{1});
    const P p2(ConstSwappable{2}, ConstSwappable{3});
    p1.swap(p2);
    TC_ASSERT_EXPR(p1.first.i == 2);
    TC_ASSERT_EXPR(p1.second.i == 3);
    TC_ASSERT_EXPR(p2.first.i == 0);
    TC_ASSERT_EXPR(p2.second.i == 1);
  }

  // pair of references
  {
    int i1 = 0, i2 = 1, i3 = 2, i4 = 3;
    const std::pair<int&, int&> p1{i1, i2};
    const std::pair<int&, int&> p2{i3, i4};
    p1.swap(p2);
    TC_ASSERT_EXPR(p1.first == 2);
    TC_ASSERT_EXPR(p1.second == 3);
    TC_ASSERT_EXPR(p2.first == 0);
    TC_ASSERT_EXPR(p2.second == 1);
  }
  return true;
}

int tc_utilities_utility_pairs_pairs_pair_swap_member_const(void) {
  test();

// gcc cannot have mutable member in constant expression
#if !defined(TEST_COMPILER_GCC)
  static_assert(test());
#endif

  return 0;
}