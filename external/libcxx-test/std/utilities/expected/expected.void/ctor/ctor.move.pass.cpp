//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr expected(expected&& rhs) noexcept(is_nothrow_move_constructible_v<E>);
//
// Constraints: is_move_constructible_v<E> is true.
//
// Effects: If rhs.has_value() is false, direct-non-list-initializes unex with std::move(rhs.error()).
//
// Postconditions: rhs.has_value() is unchanged; rhs.has_value() == this->has_value() is true.
//
// Throws: Any exception thrown by the initialization of unex.
//
// Remarks: This constructor is trivial if is_trivially_move_constructible_v<E> is true.

#include <cassert>
#include <expected>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "../../types.h"
#include "libcxx_tc_common.h"

struct NonMovable {
  NonMovable(NonMovable&&) = delete;
};

struct MovableNonTrivial {
  int i;
  constexpr MovableNonTrivial(int ii) : i(ii) {}
  constexpr MovableNonTrivial(MovableNonTrivial&& o) : i(o.i) { o.i = 0; }
  friend constexpr bool operator==(const MovableNonTrivial&, const MovableNonTrivial&) = default;
};

struct MoveMayThrow {
  MoveMayThrow(MoveMayThrow&&) {}
};

// Test Constraints:
// - is_move_constructible_v<E> is true.
static_assert(std::is_move_constructible_v<std::expected<void, int>>);
static_assert(std::is_move_constructible_v<std::expected<void, MovableNonTrivial>>);
static_assert(!std::is_move_constructible_v<std::expected<void, NonMovable>>);

// Test: This constructor is trivial if is_trivially_move_constructible_v<E> is true.
static_assert(std::is_trivially_move_constructible_v<std::expected<void, int>>);
static_assert(!std::is_trivially_move_constructible_v<std::expected<void, MovableNonTrivial>>);

// Test: noexcept(is_nothrow_move_constructible_v<E>)
static_assert(std::is_nothrow_move_constructible_v<std::expected<int, int>>);
static_assert(!std::is_nothrow_move_constructible_v<std::expected<MoveMayThrow, int>>);
static_assert(!std::is_nothrow_move_constructible_v<std::expected<int, MoveMayThrow>>);
static_assert(!std::is_nothrow_move_constructible_v<std::expected<MoveMayThrow, MoveMayThrow>>);

constexpr bool test() {
  // move the error non-trivial
  {
    std::expected<void, MovableNonTrivial> e1(std::unexpect, 5);
    auto e2 = std::move(e1);
    TC_ASSERT_EXPR(!e2.has_value());
    TC_ASSERT_EXPR(e2.error().i == 5);
    TC_ASSERT_EXPR(!e1.has_value());
    TC_ASSERT_EXPR(e1.error().i == 0);
  }

  // move the error trivial
  {
    std::expected<void, int> e1(std::unexpect, 5);
    auto e2 = std::move(e1);
    TC_ASSERT_EXPR(!e2.has_value());
    TC_ASSERT_EXPR(e2.error() == 5);
    TC_ASSERT_EXPR(!e1.has_value());
  }

  // move TailClobbererNonTrivialMove as error
  {
    std::expected<void, TailClobbererNonTrivialMove<1>> e1(std::unexpect);
    auto e2 = std::move(e1);
    TC_ASSERT_EXPR(!e2.has_value());
    TC_ASSERT_EXPR(!e1.has_value());
  }

  return true;
}

void testException() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  struct Throwing {
    Throwing() = default;
    Throwing(Throwing&&) { throw Except{}; }
  };

  // throw on moving error
  {
    std::expected<void, Throwing> e1(std::unexpect);
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      [[maybe_unused]] auto e2 = std::move(e1);
      TC_ASSERT_EXPR(false);
    } catch (Except) {
    }
#endif // _LIBCPP_NO_EXCEPTIONS
  }

#endif // TEST_HAS_NO_EXCEPTIONS
}

int tc_utilities_expected_expected_void_ctor_ctor_move(void) {
  test();
  static_assert(test());
  testException();
  return 0;
}
