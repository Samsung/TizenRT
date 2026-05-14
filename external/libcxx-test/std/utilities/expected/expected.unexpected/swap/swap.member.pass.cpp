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
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr void swap(unexpected& other) noexcept(is_nothrow_swappable_v<E>);
//
// Mandates: is_swappable_v<E> is true.
//
// Effects: Equivalent to: using std::swap; swap(unex, other.unex);

#include <cassert>
#include <concepts>
#include <expected>
#include <utility>
#include "libcxx_tc_common.h"

// test noexcept
struct NoexceptSwap {
  friend void swap(NoexceptSwap&, NoexceptSwap&) noexcept;
};

struct MayThrowSwap {
  friend void swap(MayThrowSwap&, MayThrowSwap&);
};

template <class T>
concept MemberSwapNoexcept =
    requires(T& t1, T& t2) {
      { t1.swap(t2) } noexcept;
    };

static_assert(MemberSwapNoexcept<std::unexpected<NoexceptSwap>>);
static_assert(!MemberSwapNoexcept<std::unexpected<MayThrowSwap>>);

struct ADLSwap {
  constexpr ADLSwap(int ii) : i(ii) {}
  ADLSwap& operator=(const ADLSwap&) = delete;
  int i;
  constexpr friend void swap(ADLSwap& x, ADLSwap& y) { std::swap(x.i, y.i); }
};

constexpr bool test() {
  // using std::swap;
  {
    std::unexpected<int> unex1(5);
    std::unexpected<int> unex2(6);
    unex1.swap(unex2);
    TC_ASSERT_EXPR(unex1.error() == 6);
    TC_ASSERT_EXPR(unex2.error() == 5);
  }

  // adl swap
  {
    std::unexpected<ADLSwap> unex1(5);
    std::unexpected<ADLSwap> unex2(6);
    unex1.swap(unex2);
    TC_ASSERT_EXPR(unex1.error().i == 6);
    TC_ASSERT_EXPR(unex2.error().i == 5);
  }
  return true;
}

int tc_utilities_expected_expected_unexpected_swap_swap_member(void) {
  test();
  static_assert(test());
  return 0;
}
