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
// Older Clangs do not support the C++20 feature to constrain destructors
// XFAIL: clang-15, apple-clang-14

// friend constexpr void swap(expected& x, expected& y) noexcept(noexcept(swap(x,y)));

#include <cassert>
#include <expected>
#include <type_traits>
#include <utility>

#include "../../types.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

// Test constraint
static_assert(std::is_swappable_v<std::expected<void, int>>);

struct NotSwappable {
  NotSwappable& operator=(const NotSwappable&) = delete;
};
void swap(NotSwappable&, NotSwappable&) = delete;

// !is_swappable_v<E>
static_assert(!std::is_swappable_v<std::expected<void, NotSwappable>>);

struct NotMoveContructible {
  NotMoveContructible(NotMoveContructible&&) = delete;
  friend void swap(NotMoveContructible&, NotMoveContructible&) {}
};

// !is_move_constructible_v<E>
static_assert(!std::is_swappable_v<std::expected<void, NotMoveContructible>>);

// Test noexcept
struct MoveMayThrow {
  MoveMayThrow(MoveMayThrow&&) noexcept(false);
  friend void swap(MoveMayThrow&, MoveMayThrow&) noexcept {}
};

template <class E>
concept FreeSwapNoexcept =
    requires(std::expected<void, E> x, std::expected<void, E> y) {
      { swap(x, y) } noexcept;
    };

static_assert(FreeSwapNoexcept<int>);

// !is_nothrow_move_constructible_v<E>
static_assert(!FreeSwapNoexcept<MoveMayThrow>);

struct SwapMayThrow {
  friend void swap(SwapMayThrow&, SwapMayThrow&) noexcept(false) {}
};

// !is_nothrow_swappable_v<E>
static_assert(!FreeSwapNoexcept<SwapMayThrow>);

constexpr bool test() {
  // this->has_value() && rhs.has_value()
  {
    std::expected<void, int> x;
    std::expected<void, int> y;
    swap(x, y);

    TC_ASSERT_EXPR(x.has_value());
    TC_ASSERT_EXPR(y.has_value());
  }

  // !this->has_value() && !rhs.has_value()
  {
    std::expected<void, ADLSwap> x(std::unexpect, 5);
    std::expected<void, ADLSwap> y(std::unexpect, 10);
    swap(x, y);

    TC_ASSERT_EXPR(!x.has_value());
    TC_ASSERT_EXPR(x.error().i == 10);
    TC_ASSERT_EXPR(x.error().adlSwapCalled);
    TC_ASSERT_EXPR(!y.has_value());
    TC_ASSERT_EXPR(y.error().i == 5);
    TC_ASSERT_EXPR(y.error().adlSwapCalled);
  }

  // this->has_value() && !rhs.has_value()
  {
    Traced::state s{};
    std::expected<void, Traced> e1(std::in_place);
    std::expected<void, Traced> e2(std::unexpect, s, 10);

    swap(e1, e2);

    TC_ASSERT_EXPR(!e1.has_value());
    TC_ASSERT_EXPR(e1.error().data_ == 10);
    TC_ASSERT_EXPR(e2.has_value());

    TC_ASSERT_EXPR(s.moveCtorCalled);
    TC_ASSERT_EXPR(s.dtorCalled);
  }

  // !this->has_value() && rhs.has_value()
  {
    Traced::state s{};
    std::expected<void, Traced> e1(std::unexpect, s, 10);
    std::expected<void, Traced> e2(std::in_place);

    swap(e1, e2);

    TC_ASSERT_EXPR(e1.has_value());
    TC_ASSERT_EXPR(!e2.has_value());
    TC_ASSERT_EXPR(e2.error().data_ == 10);

    TC_ASSERT_EXPR(s.moveCtorCalled);
    TC_ASSERT_EXPR(s.dtorCalled);
  }

  // TailClobberer
  {
    std::expected<void, TailClobbererNonTrivialMove<1>> x(std::in_place);
    std::expected<void, TailClobbererNonTrivialMove<1>> y(std::unexpect);

    swap(x, y);

    // The next line would fail if adjusting the "has value" flag happened
    // _before_ constructing the member object inside the `swap`.
    TC_ASSERT_EXPR(!x.has_value());
    TC_ASSERT_EXPR(y.has_value());
  }

  return true;
}

void testException() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  // !e1.has_value() && e2.has_value()
  {
    bool e1Destroyed = false;
    std::expected<void, ThrowOnMove> e1(std::unexpect, e1Destroyed);
    std::expected<void, ThrowOnMove> e2(std::in_place);
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      swap(e1, e2);
      TC_ASSERT_EXPR(false);
    } catch (Except) {
      TC_ASSERT_EXPR(!e1.has_value());
      TC_ASSERT_EXPR(e2.has_value());
      TC_ASSERT_EXPR(!e1Destroyed);
    }
#endif // _LIBCPP_NO_EXCEPTIONS
  }

  // e1.has_value() && !e2.has_value()
  {
    bool e2Destroyed = false;
    std::expected<void, ThrowOnMove> e1(std::in_place);
    std::expected<void, ThrowOnMove> e2(std::unexpect, e2Destroyed);
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      swap(e1, e2);
      TC_ASSERT_EXPR(false);
    } catch (Except) {
      TC_ASSERT_EXPR(e1.has_value());
      TC_ASSERT_EXPR(!e2.has_value());
      TC_ASSERT_EXPR(!e2Destroyed);
    }
#endif // _LIBCPP_NO_EXCEPTIONS
  }

  // TailClobberer
  {
    std::expected<void, TailClobbererNonTrivialMove<0, false, true>> x(std::in_place);
    std::expected<void, TailClobbererNonTrivialMove<0, false, true>> y(std::unexpect);
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      swap(x, y);
      TC_ASSERT_EXPR(false);
    } catch (Except) {
      // This would fail if `TailClobbererNonTrivialMove<0, false, true>`
      // clobbered the flag before throwing the exception.
      TC_ASSERT_EXPR(x.has_value());
      TC_ASSERT_EXPR(!y.has_value());
    }
#endif // _LIBCPP_NO_EXCEPTIONS
  }
#endif // TEST_HAS_NO_EXCEPTIONS
}

int tc_utilities_expected_expected_void_swap_free_swap(void) {
  test();
  static_assert(test());
  testException();
  return 0;
}
