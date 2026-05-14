//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20
// Older Clangs do not support the C++20 feature to constrain destructors
// XFAIL: apple-clang-14

// constexpr void swap(expected& rhs) noexcept(see below);
//
// Constraints:
// is_swappable_v<E> is true and is_move_constructible_v<E> is true.
//
// Throws: Any exception thrown by the expressions in the Effects.
//
// Remarks: The exception specification is equivalent to:
// is_nothrow_move_constructible_v<E> && is_nothrow_swappable_v<E>.

#include <cassert>
#include <expected>
#include <type_traits>
#include <utility>

#include "../../types.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

// Test Constraints:
template <class E>
concept HasMemberSwap = requires(std::expected<void, E> x, std::expected<void, E> y) { x.swap(y); };

static_assert(HasMemberSwap<int>);

struct NotSwappable {};
void swap(NotSwappable&, NotSwappable&) = delete;

// !is_swappable_v<E>
static_assert(!HasMemberSwap<NotSwappable>);

struct NotMoveContructible {
  NotMoveContructible(NotMoveContructible&&) = delete;
  friend void swap(NotMoveContructible&, NotMoveContructible&) {}
};

// !is_move_constructible_v<E>
static_assert(!HasMemberSwap<NotMoveContructible>);

// Test noexcept
struct MoveMayThrow {
  MoveMayThrow(MoveMayThrow&&) noexcept(false);
  friend void swap(MoveMayThrow&, MoveMayThrow&) noexcept {}
};

template <class E>
concept MemberSwapNoexcept = //
    requires(std::expected<void, E> x, std::expected<void, E> y) {
      { x.swap(y) } noexcept;
    };

static_assert(MemberSwapNoexcept<int>);

// !is_nothrow_move_constructible_v<E>
static_assert(!MemberSwapNoexcept<MoveMayThrow>);

struct SwapMayThrow {
  friend void swap(SwapMayThrow&, SwapMayThrow&) noexcept(false) {}
};

// !is_nothrow_swappable_v<E>
static_assert(!MemberSwapNoexcept<SwapMayThrow>);

constexpr bool test() {
  // this->has_value() && rhs.has_value()
  {
    std::expected<void, int> x;
    std::expected<void, int> y;
    x.swap(y);

    TC_ASSERT_EXPR(x.has_value());
    TC_ASSERT_EXPR(y.has_value());
  }

  // !this->has_value() && !rhs.has_value()
  {
    std::expected<void, ADLSwap> x(std::unexpect, 5);
    std::expected<void, ADLSwap> y(std::unexpect, 10);
    x.swap(y);

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

    e1.swap(e2);

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

    e1.swap(e2);

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

    x.swap(y);

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
      e1.swap(e2);
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
      e1.swap(e2);
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
      x.swap(y);
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

int tc_utilities_expected_expected_void_swap_member_swap(void) {
  test();
  static_assert(test());
  testException();
  return 0;
}
