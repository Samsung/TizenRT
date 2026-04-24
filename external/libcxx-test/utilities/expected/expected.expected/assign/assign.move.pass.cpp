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
// Constraints:
// - is_move_constructible_v<T> is true and
// - is_move_assignable_v<T> is true and
// - is_move_constructible_v<E> is true and
// - is_move_assignable_v<E> is true and
// - is_nothrow_move_constructible_v<T> || is_nothrow_move_constructible_v<E> is true.
//
// Effects:
// - If this->has_value() && rhs.has_value() is true, equivalent to val = std::move(*rhs).
// - Otherwise, if this->has_value() is true, equivalent to:
//   reinit-expected(unex, val, std::move(rhs.error()))
// - Otherwise, if rhs.has_value() is true, equivalent to:
//   reinit-expected(val, unex, std::move(*rhs))
// - Otherwise, equivalent to unex = std::move(rhs.error()).
// - Then, if no exception was thrown, equivalent to: has_val = rhs.has_value(); return *this;
//
// Returns: *this.
//
// Remarks: The exception specification is equivalent to:
// is_nothrow_move_assignable_v<T> && is_nothrow_move_constructible_v<T> &&
// is_nothrow_move_assignable_v<E> && is_nothrow_move_constructible_v<E>

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

struct MoveCtorMayThrow {
  MoveCtorMayThrow(MoveCtorMayThrow&&) noexcept(false) {}
  MoveCtorMayThrow& operator=(MoveCtorMayThrow&&) noexcept = default;
};

// Test constraints
static_assert(std::is_move_assignable_v<std::expected<int, int>>);

// !is_move_assignable_v<T>
static_assert(!std::is_move_assignable_v<std::expected<NotMoveAssignable, int>>);

// !is_move_constructible_v<T>
static_assert(!std::is_move_assignable_v<std::expected<NotMoveConstructible, int>>);

// !is_move_assignable_v<E>
static_assert(!std::is_move_assignable_v<std::expected<int, NotMoveAssignable>>);

// !is_move_constructible_v<E>
static_assert(!std::is_move_assignable_v<std::expected<int, NotMoveConstructible>>);

// !is_nothrow_move_constructible_v<T> && is_nothrow_move_constructible_v<E>
static_assert(std::is_move_assignable_v<std::expected<MoveCtorMayThrow, int>>);

// is_nothrow_move_constructible_v<T> && !is_nothrow_move_constructible_v<E>
static_assert(std::is_move_assignable_v<std::expected<int, MoveCtorMayThrow>>);

// !is_nothrow_move_constructible_v<T> && !is_nothrow_move_constructible_v<E>
static_assert(!std::is_move_assignable_v<std::expected<MoveCtorMayThrow, MoveCtorMayThrow>>);

struct MoveAssignMayThrow {
  MoveAssignMayThrow(MoveAssignMayThrow&&) noexcept = default;
  MoveAssignMayThrow& operator=(MoveAssignMayThrow&&) noexcept(false) { return *this; }
};

// Test noexcept
static_assert(std::is_nothrow_move_assignable_v<std::expected<int, int>>);

// !is_nothrow_move_assignable_v<T>
static_assert(!std::is_nothrow_move_assignable_v<std::expected<MoveAssignMayThrow, int>>);

// !is_nothrow_move_constructible_v<T>
static_assert(!std::is_nothrow_move_assignable_v<std::expected<MoveCtorMayThrow, int>>);

// !is_nothrow_move_assignable_v<E>
static_assert(!std::is_nothrow_move_assignable_v<std::expected<int, MoveAssignMayThrow>>);

// !is_nothrow_move_constructible_v<E>
static_assert(!std::is_nothrow_move_assignable_v<std::expected<int, MoveCtorMayThrow>>);

constexpr bool test() {
  // If this->has_value() && rhs.has_value() is true, equivalent to val = std::move(*rhs).
  {
    Traced::state oldState{};
    Traced::state newState{};
    std::expected<Traced, int> e1(std::in_place, oldState, 5);
    std::expected<Traced, int> e2(std::in_place, newState, 10);
    decltype(auto) x = (e1 = std::move(e2));
    static_assert(std::same_as<decltype(x), std::expected<Traced, int>&>);
    TC_ASSERT_EXPR(&x == &e1);

    TC_ASSERT_EXPR(e1.has_value());
    TC_ASSERT_EXPR(e1.value().data_ == 10);
    TC_ASSERT_EXPR(oldState.moveAssignCalled);
  }

  // - Otherwise, if this->has_value() is true, equivalent to:
  // reinit-expected(unex, val, rhs.error())
  //  E move is not noexcept
  //  In this case, it should call the branch
  //
  //  U tmp(std::move(oldval));
  //  destroy_at(addressof(oldval));
  //  try {
  //    construct_at(addressof(newval), std::forward<Args>(args)...);
  //  } catch (...) {
  //    construct_at(addressof(oldval), std::move(tmp));
  //    throw;
  //  }
  //
  {
    TracedNoexcept::state oldState{};
    Traced::state newState{};
    std::expected<TracedNoexcept, Traced> e1(std::in_place, oldState, 5);
    std::expected<TracedNoexcept, Traced> e2(std::unexpect, newState, 10);

    decltype(auto) x = (e1 = std::move(e2));
    static_assert(std::same_as<decltype(x), std::expected<TracedNoexcept, Traced>&>);
    TC_ASSERT_EXPR(&x == &e1);

    TC_ASSERT_EXPR(!e1.has_value());
    TC_ASSERT_EXPR(e1.error().data_ == 10);

    TC_ASSERT_EXPR(!oldState.moveAssignCalled);
    TC_ASSERT_EXPR(oldState.moveCtorCalled);
    TC_ASSERT_EXPR(oldState.dtorCalled);
    TC_ASSERT_EXPR(!oldState.copyCtorCalled);
    TC_ASSERT_EXPR(!newState.copyCtorCalled);
    TC_ASSERT_EXPR(newState.moveCtorCalled);
    TC_ASSERT_EXPR(!newState.dtorCalled);
  }

  // - Otherwise, if this->has_value() is true, equivalent to:
  // reinit-expected(unex, val, rhs.error())
  //  E move is noexcept
  //  In this case, it should call the branch
  //
  //  destroy_at(addressof(oldval));
  //  construct_at(addressof(newval), std::forward<Args>(args)...);
  //
  {
    Traced::state oldState{};
    TracedNoexcept::state newState{};
    std::expected<Traced, TracedNoexcept> e1(std::in_place, oldState, 5);
    std::expected<Traced, TracedNoexcept> e2(std::unexpect, newState, 10);

    decltype(auto) x = (e1 = std::move(e2));
    static_assert(std::same_as<decltype(x), std::expected<Traced, TracedNoexcept>&>);
    TC_ASSERT_EXPR(&x == &e1);

    TC_ASSERT_EXPR(!e1.has_value());
    TC_ASSERT_EXPR(e1.error().data_ == 10);

    TC_ASSERT_EXPR(!oldState.moveCtorCalled);
    TC_ASSERT_EXPR(oldState.dtorCalled);
    TC_ASSERT_EXPR(!oldState.copyCtorCalled);
    TC_ASSERT_EXPR(!newState.copyCtorCalled);
    TC_ASSERT_EXPR(newState.moveCtorCalled);
    TC_ASSERT_EXPR(!newState.dtorCalled);
  }

  // - Otherwise, if rhs.has_value() is true, equivalent to:
  // reinit-expected(val, unex, *rhs)
  //  T move is not noexcept
  //  In this case, it should call the branch
  //
  //  U tmp(std::move(oldval));
  //  destroy_at(addressof(oldval));
  //  try {
  //    construct_at(addressof(newval), std::forward<Args>(args)...);
  //  } catch (...) {
  //    construct_at(addressof(oldval), std::move(tmp));
  //    throw;
  //  }
  //
  {
    TracedNoexcept::state oldState{};
    Traced::state newState{};
    std::expected<Traced, TracedNoexcept> e1(std::unexpect, oldState, 5);
    std::expected<Traced, TracedNoexcept> e2(std::in_place, newState, 10);

    decltype(auto) x = (e1 = std::move(e2));
    static_assert(std::same_as<decltype(x), std::expected<Traced, TracedNoexcept>&>);
    TC_ASSERT_EXPR(&x == &e1);

    TC_ASSERT_EXPR(e1.has_value());
    TC_ASSERT_EXPR(e1.value().data_ == 10);

    TC_ASSERT_EXPR(oldState.moveCtorCalled);
    TC_ASSERT_EXPR(oldState.dtorCalled);
    TC_ASSERT_EXPR(!oldState.copyCtorCalled);
    TC_ASSERT_EXPR(!newState.copyCtorCalled);
    TC_ASSERT_EXPR(newState.moveCtorCalled);
    TC_ASSERT_EXPR(!newState.dtorCalled);
  }

  // - Otherwise, if rhs.has_value() is true, equivalent to:
  // reinit-expected(val, unex, *rhs)
  //  T move is noexcept
  //  In this case, it should call the branch
  //
  //  destroy_at(addressof(oldval));
  //  construct_at(addressof(newval), std::forward<Args>(args)...);
  //
  {
    Traced::state oldState{};
    TracedNoexcept::state newState{};
    std::expected<TracedNoexcept, Traced> e1(std::unexpect, oldState, 5);
    std::expected<TracedNoexcept, Traced> e2(std::in_place, newState, 10);

    decltype(auto) x = (e1 = std::move(e2));
    static_assert(std::same_as<decltype(x), std::expected<TracedNoexcept, Traced>&>);
    TC_ASSERT_EXPR(&x == &e1);

    TC_ASSERT_EXPR(e1.has_value());
    TC_ASSERT_EXPR(e1.value().data_ == 10);

    TC_ASSERT_EXPR(!oldState.moveCtorCalled);
    TC_ASSERT_EXPR(oldState.dtorCalled);
    TC_ASSERT_EXPR(!oldState.copyCtorCalled);
    TC_ASSERT_EXPR(!newState.copyCtorCalled);
    TC_ASSERT_EXPR(newState.moveCtorCalled);
    TC_ASSERT_EXPR(!newState.dtorCalled);
  }

  // Otherwise, equivalent to unex = rhs.error().
  {
    Traced::state oldState{};
    Traced::state newState{};
    std::expected<int, Traced> e1(std::unexpect, oldState, 5);
    std::expected<int, Traced> e2(std::unexpect, newState, 10);
    decltype(auto) x = (e1 = std::move(e2));
    static_assert(std::same_as<decltype(x), std::expected<int, Traced>&>);
    TC_ASSERT_EXPR(&x == &e1);

    TC_ASSERT_EXPR(!e1.has_value());
    TC_ASSERT_EXPR(e1.error().data_ == 10);
    TC_ASSERT_EXPR(oldState.moveAssignCalled);
  }
  return true;
}

void testException() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  // assign value throw on move
  {
    std::expected<ThrowOnMoveConstruct, int> e1(std::unexpect, 5);
    std::expected<ThrowOnMoveConstruct, int> e2(std::in_place);
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      e1 = std::move(e2);
      TC_ASSERT_EXPR(false);
    } catch (Except) {
      TC_ASSERT_EXPR(!e1.has_value());
      TC_ASSERT_EXPR(e1.error() == 5);
    }
#endif // _LIBCPP_NO_EXCEPTIONS
  }

  // assign error throw on move
  {
    std::expected<int, ThrowOnMoveConstruct> e1(5);
    std::expected<int, ThrowOnMoveConstruct> e2(std::unexpect);
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      e1 = std::move(e2);
      TC_ASSERT_EXPR(false);
    } catch (Except) {
      TC_ASSERT_EXPR(e1.has_value());
      TC_ASSERT_EXPR(*e1 == 5);
    }
#endif // _LIBCPP_NO_EXCEPTIONS
  }
#endif // TEST_HAS_NO_EXCEPTIONS
}

int tc_utilities_expected_expected_expected_assign_assign_move(void) {
  test();
  static_assert(test());
  testException();
  return 0;
}
