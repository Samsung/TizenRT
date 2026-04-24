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

// constexpr void swap(expected& rhs) noexcept(see below);
//
// Constraints:
// is_swappable_v<T> is true and
// is_swappable_v<E> is true and
// is_move_constructible_v<T> && is_move_constructible_v<E> is true, and
// is_nothrow_move_constructible_v<T> || is_nothrow_move_constructible_v<E> is true.
//
// Throws: Any exception thrown by the expressions in the Effects.
// Remarks: The exception specification is equivalent to:
// is_nothrow_move_constructible_v<T> && is_nothrow_swappable_v<T> &&
// is_nothrow_move_constructible_v<E> && is_nothrow_swappable_v<E>

#include <cassert>
#include <expected>
#include <type_traits>
#include <utility>

#include "../../types.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

// Test Constraints:
template <class T, class E>
concept HasMemberSwap = requires(std::expected<T, E> x, std::expected<T, E> y) { x.swap(y); };

static_assert(HasMemberSwap<int, int>);

struct NotSwappable {};
void swap(NotSwappable&, NotSwappable&) = delete;

// !is_swappable_v<T>
static_assert(!HasMemberSwap<NotSwappable, int>);

// !is_swappable_v<E>
static_assert(!HasMemberSwap<int, NotSwappable>);

struct NotMoveContructible {
  NotMoveContructible(NotMoveContructible&&) = delete;
  friend void swap(NotMoveContructible&, NotMoveContructible&) {}
};

// !is_move_constructible_v<T>
static_assert(!HasMemberSwap<NotMoveContructible, int>);

// !is_move_constructible_v<E>
static_assert(!HasMemberSwap<int, NotMoveContructible>);

struct MoveMayThrow {
  MoveMayThrow(MoveMayThrow&&) noexcept(false);
  friend void swap(MoveMayThrow&, MoveMayThrow&) noexcept {}
};

// !is_nothrow_move_constructible_v<T> && is_nothrow_move_constructible_v<E>
static_assert(HasMemberSwap<MoveMayThrow, int>);

// is_nothrow_move_constructible_v<T> && !is_nothrow_move_constructible_v<E>
static_assert(HasMemberSwap<int, MoveMayThrow>);

// !is_nothrow_move_constructible_v<T> && !is_nothrow_move_constructible_v<E>
static_assert(!HasMemberSwap<MoveMayThrow, MoveMayThrow>);

// Test noexcept
template <class T, class E>
concept MemberSwapNoexcept = //
    requires(std::expected<T, E> x, std::expected<T, E> y) {
      { x.swap(y) } noexcept;
    };

static_assert(MemberSwapNoexcept<int, int>);

// !is_nothrow_move_constructible_v<T>
static_assert(!MemberSwapNoexcept<MoveMayThrow, int>);

// !is_nothrow_move_constructible_v<E>
static_assert(!MemberSwapNoexcept<int, MoveMayThrow>);

struct SwapMayThrow {
  friend void swap(SwapMayThrow&, SwapMayThrow&) noexcept(false) {}
};

// !is_nothrow_swappable_v<T>
static_assert(!MemberSwapNoexcept<SwapMayThrow, int>);

// !is_nothrow_swappable_v<E>
static_assert(!MemberSwapNoexcept<int, SwapMayThrow>);

constexpr bool test() {
  // this->has_value() && rhs.has_value()
  {
    std::expected<ADLSwap, int> x(std::in_place, 5);
    std::expected<ADLSwap, int> y(std::in_place, 10);
    x.swap(y);

    TC_ASSERT_EXPR(x.has_value());
    TC_ASSERT_EXPR(x->i == 10);
    TC_ASSERT_EXPR(x->adlSwapCalled);
    TC_ASSERT_EXPR(y.has_value());
    TC_ASSERT_EXPR(y->i == 5);
    TC_ASSERT_EXPR(y->adlSwapCalled);
  }

  // !this->has_value() && !rhs.has_value()
  {
    std::expected<int, ADLSwap> x(std::unexpect, 5);
    std::expected<int, ADLSwap> y(std::unexpect, 10);
    x.swap(y);

    TC_ASSERT_EXPR(!x.has_value());
    TC_ASSERT_EXPR(x.error().i == 10);
    TC_ASSERT_EXPR(x.error().adlSwapCalled);
    TC_ASSERT_EXPR(!y.has_value());
    TC_ASSERT_EXPR(y.error().i == 5);
    TC_ASSERT_EXPR(y.error().adlSwapCalled);
  }

  // this->has_value() && !rhs.has_value()
  // && is_nothrow_move_constructible_v<E>
  {
    std::expected<TrackedMove<true>, TrackedMove<true>> e1(std::in_place, 5);
    std::expected<TrackedMove<true>, TrackedMove<true>> e2(std::unexpect, 10);

    e1.swap(e2);

    TC_ASSERT_EXPR(!e1.has_value());
    TC_ASSERT_EXPR(e1.error().i == 10);
    TC_ASSERT_EXPR(e2.has_value());
    TC_ASSERT_EXPR(e2->i == 5);

    TC_ASSERT_EXPR(e1.error().numberOfMoves == 2);
    TC_ASSERT_EXPR(!e1.error().swapCalled);
    TC_ASSERT_EXPR(e2->numberOfMoves == 1);
    TC_ASSERT_EXPR(!e2->swapCalled);
  }

  // this->has_value() && !rhs.has_value()
  // && !is_nothrow_move_constructible_v<E>
  {
    std::expected<TrackedMove<true>, TrackedMove<false>> e1(std::in_place, 5);
    std::expected<TrackedMove<true>, TrackedMove<false>> e2(std::unexpect, 10);

    e1.swap(e2);

    TC_ASSERT_EXPR(!e1.has_value());
    TC_ASSERT_EXPR(e1.error().i == 10);
    TC_ASSERT_EXPR(e2.has_value());
    TC_ASSERT_EXPR(e2->i == 5);

    TC_ASSERT_EXPR(e1.error().numberOfMoves == 1);
    TC_ASSERT_EXPR(!e1.error().swapCalled);
    TC_ASSERT_EXPR(e2->numberOfMoves == 2);
    TC_ASSERT_EXPR(!e2->swapCalled);
  }

  // !this->has_value() && rhs.has_value()
  // && is_nothrow_move_constructible_v<E>
  {
    std::expected<TrackedMove<true>, TrackedMove<true>> e1(std::unexpect, 10);
    std::expected<TrackedMove<true>, TrackedMove<true>> e2(std::in_place, 5);

    e1.swap(e2);

    TC_ASSERT_EXPR(e1.has_value());
    TC_ASSERT_EXPR(e1->i == 5);
    TC_ASSERT_EXPR(!e2.has_value());
    TC_ASSERT_EXPR(e2.error().i == 10);

    TC_ASSERT_EXPR(e1->numberOfMoves == 1);
    TC_ASSERT_EXPR(!e1->swapCalled);
    TC_ASSERT_EXPR(e2.error().numberOfMoves == 2);
    TC_ASSERT_EXPR(!e2.error().swapCalled);
  }

  // !this->has_value() && rhs.has_value()
  // && !is_nothrow_move_constructible_v<E>
  {
    std::expected<TrackedMove<true>, TrackedMove<false>> e1(std::unexpect, 10);
    std::expected<TrackedMove<true>, TrackedMove<false>> e2(std::in_place, 5);

    e1.swap(e2);

    TC_ASSERT_EXPR(e1.has_value());
    TC_ASSERT_EXPR(e1->i == 5);
    TC_ASSERT_EXPR(!e2.has_value());
    TC_ASSERT_EXPR(e2.error().i == 10);

    TC_ASSERT_EXPR(e1->numberOfMoves == 2);
    TC_ASSERT_EXPR(!e1->swapCalled);
    TC_ASSERT_EXPR(e2.error().numberOfMoves == 1);
    TC_ASSERT_EXPR(!e2.error().swapCalled);
  }

  // TailClobberer
  {
    // is_nothrow_move_constructible_v<E>
    {
      std::expected<TailClobbererNonTrivialMove<0, true>, TailClobbererNonTrivialMove<1, true>> x(std::in_place);
      std::expected<TailClobbererNonTrivialMove<0, true>, TailClobbererNonTrivialMove<1, true>> y(std::unexpect);

      x.swap(y);

      // Both of these would fail if adjusting the "has value" flags happened
      // _before_ constructing the member objects inside the `swap`.
      TC_ASSERT_EXPR(!x.has_value());
      TC_ASSERT_EXPR(y.has_value());
    }

    // !is_nothrow_move_constructible_v<E>
    {
      std::expected<TailClobbererNonTrivialMove<0, true>, TailClobbererNonTrivialMove<1, false>> x(std::in_place);
      std::expected<TailClobbererNonTrivialMove<0, true>, TailClobbererNonTrivialMove<1, false>> y(std::unexpect);

      x.swap(y);

      // Both of these would fail if adjusting the "has value" flags happened
      // _before_ constructing the member objects inside the `swap`.
      TC_ASSERT_EXPR(!x.has_value());
      TC_ASSERT_EXPR(y.has_value());
    }
  }

  return true;
}

void testException() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  // !e1.has_value() && e2.has_value()
  {
    std::expected<ThrowOnMoveConstruct, int> e1(std::unexpect, 5);
    std::expected<ThrowOnMoveConstruct, int> e2(std::in_place);
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      e1.swap(e2);
      TC_ASSERT_EXPR(false);
    } catch (Except) {
      TC_ASSERT_EXPR(!e1.has_value());
      TC_ASSERT_EXPR(e1.error() == 5);
    }
#endif // _LIBCPP_NO_EXCEPTIONS
  }

  // e1.has_value() && !e2.has_value()
  {
    std::expected<int, ThrowOnMoveConstruct> e1(5);
    std::expected<int, ThrowOnMoveConstruct> e2(std::unexpect);
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      e1.swap(e2);
      TC_ASSERT_EXPR(false);
    } catch (Except) {
      TC_ASSERT_EXPR(e1.has_value());
      TC_ASSERT_EXPR(*e1 == 5);
    }
#endif // _LIBCPP_NO_EXCEPTIONS
  }

  // TailClobberer
  {
    // is_nothrow_move_constructible_v<E>
    {
      std::expected<TailClobbererNonTrivialMove<0, false, true>, TailClobbererNonTrivialMove<1>> x(std::in_place);
      std::expected<TailClobbererNonTrivialMove<0, false, true>, TailClobbererNonTrivialMove<1>> y(std::unexpect);
#ifndef _LIBCPP_NO_EXCEPTIONS
      try {
        x.swap(y);
        TC_ASSERT_EXPR(false);
      } catch (Except) {
        TC_ASSERT_EXPR(x.has_value());
        // This would fail if `TailClobbererNonTrivialMove<1>` clobbered the
        // flag when rolling back the swap.
        TC_ASSERT_EXPR(!y.has_value());
      }
#endif // _LIBCPP_NO_EXCEPTIONS
    }

    // !is_nothrow_move_constructible_v<E>
    {
      std::expected<TailClobbererNonTrivialMove<0>, TailClobbererNonTrivialMove<1, false, true>> x(std::in_place);
      std::expected<TailClobbererNonTrivialMove<0>, TailClobbererNonTrivialMove<1, false, true>> y(std::unexpect);
#ifndef _LIBCPP_NO_EXCEPTIONS
      try {
        x.swap(y);
        TC_ASSERT_EXPR(false);
      } catch (Except) {
        // This would fail if `TailClobbererNonTrivialMove<0>` clobbered the
        // flag when rolling back the swap.
        TC_ASSERT_EXPR(x.has_value());
        TC_ASSERT_EXPR(!y.has_value());
      }
#endif // _LIBCPP_NO_EXCEPTIONS
    }
  }
#endif // TEST_HAS_NO_EXCEPTIONS
}

int tc_utilities_expected_expected_expected_swap_member_swap(void) {
  test();
  static_assert(test());
  testException();
  return 0;
}
