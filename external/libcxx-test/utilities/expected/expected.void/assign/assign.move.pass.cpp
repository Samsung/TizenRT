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
// XFAIL: apple-clang-14

// constexpr expected& operator=(expected&& rhs) noexcept(see below);
//
// Effects:
// - If this->has_value() && rhs.has_value() is true, no effects.
// - Otherwise, if this->has_value() is true, equivalent to:
//   construct_at(addressof(unex), std::move(rhs.unex));
//   has_val = false;
// - Otherwise, if rhs.has_value() is true, destroys unex and sets has_val to true.
// - Otherwise, equivalent to unex = std::move(rhs.error()).
//
// Returns: *this.
//
// Remarks: The exception specification is equivalent to is_nothrow_move_constructible_v<E> && is_nothrow_move_assignable_v<E>.
//
// This operator is defined as deleted unless is_move_constructible_v<E> is true and is_move_assignable_v<E> is true.

#include <cassert>
#include <concepts>
#include <expected>
#include <type_traits>
#include <utility>

#include "../../types.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

struct NotMoveConstructible {
  NotMoveConstructible(NotMoveConstructible&&)            = delete;
  NotMoveConstructible& operator=(NotMoveConstructible&&) = default;
};

struct NotMoveAssignable {
  NotMoveAssignable(NotMoveAssignable&&)            = default;
  NotMoveAssignable& operator=(NotMoveAssignable&&) = delete;
};

// Test constraints
static_assert(std::is_move_assignable_v<std::expected<void, int>>);

// !is_move_assignable_v<E>
static_assert(!std::is_move_assignable_v<std::expected<void, NotMoveAssignable>>);

// !is_move_constructible_v<E>
static_assert(!std::is_move_assignable_v<std::expected<void, NotMoveConstructible>>);

// Test noexcept
struct MoveCtorMayThrow {
  MoveCtorMayThrow(MoveCtorMayThrow&&) noexcept(false) {}
  MoveCtorMayThrow& operator=(MoveCtorMayThrow&&) noexcept = default;
};

struct MoveAssignMayThrow {
  MoveAssignMayThrow(MoveAssignMayThrow&&) noexcept = default;
  MoveAssignMayThrow& operator=(MoveAssignMayThrow&&) noexcept(false) { return *this; }
};

// Test noexcept
static_assert(std::is_nothrow_move_assignable_v<std::expected<void, int>>);

// !is_nothrow_move_assignable_v<E>
static_assert(!std::is_nothrow_move_assignable_v<std::expected<void, MoveAssignMayThrow>>);

// !is_nothrow_move_constructible_v<E>
static_assert(!std::is_nothrow_move_assignable_v<std::expected<void, MoveCtorMayThrow>>);

constexpr bool test() {
  // If this->has_value() && rhs.has_value() is true, no effects.
  {
    std::expected<void, int> e1;
    std::expected<void, int> e2;
    decltype(auto) x = (e1 = std::move(e2));
    static_assert(std::same_as<decltype(x), std::expected<void, int>&>);
    TC_ASSERT_EXPR(&x == &e1);
    TC_ASSERT_EXPR(e1.has_value());
  }

  // Otherwise, if this->has_value() is true, equivalent to:
  // construct_at(addressof(unex), std::move(rhs.unex));
  // has_val = false;
  {
    Traced::state state{};
    std::expected<void, Traced> e1;
    std::expected<void, Traced> e2(std::unexpect, state, 5);
    decltype(auto) x = (e1 = std::move(e2));
    static_assert(std::same_as<decltype(x), std::expected<void, Traced>&>);
    TC_ASSERT_EXPR(&x == &e1);
    TC_ASSERT_EXPR(!e1.has_value());
    TC_ASSERT_EXPR(e1.error().data_ == 5);

    TC_ASSERT_EXPR(state.moveCtorCalled);
  }

  // Otherwise, if rhs.has_value() is true, destroys unex and sets has_val to true.
  {
    Traced::state state{};
    std::expected<void, Traced> e1(std::unexpect, state, 5);
    std::expected<void, Traced> e2;
    decltype(auto) x = (e1 = std::move(e2));
    static_assert(std::same_as<decltype(x), std::expected<void, Traced>&>);
    TC_ASSERT_EXPR(&x == &e1);
    TC_ASSERT_EXPR(e1.has_value());

    TC_ASSERT_EXPR(state.dtorCalled);
  }

  // Otherwise, equivalent to unex = rhs.error().
  {
    Traced::state state{};
    std::expected<void, Traced> e1(std::unexpect, state, 5);
    std::expected<void, Traced> e2(std::unexpect, state, 10);
    decltype(auto) x = (e1 = std::move(e2));
    static_assert(std::same_as<decltype(x), std::expected<void, Traced>&>);
    TC_ASSERT_EXPR(&x == &e1);
    TC_ASSERT_EXPR(!e1.has_value());
    TC_ASSERT_EXPR(e1.error().data_ == 10);

    TC_ASSERT_EXPR(state.moveAssignCalled);
  }

  return true;
}

void testException() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  std::expected<void, ThrowOnMoveConstruct> e1(std::in_place);
  std::expected<void, ThrowOnMoveConstruct> e2(std::unexpect);
#ifndef _LIBCPP_NO_EXCEPTIONS
  try {
    e1 = std::move(e2);
    TC_ASSERT_EXPR(false);
  } catch (Except) {
    TC_ASSERT_EXPR(e1.has_value());
  }
#endif // _LIBCPP_NO_EXCEPTIONS
#endif // TEST_HAS_NO_EXCEPTIONS
}

int tc_utilities_expected_expected_void_assign_assign_move(void) {
  test();
  static_assert(test());
  testException();
  return 0;
}
