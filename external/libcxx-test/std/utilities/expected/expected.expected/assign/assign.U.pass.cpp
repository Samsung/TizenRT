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
// XFAIL: clang-14, apple-clang-14

//  template<class U = T>
//   constexpr expected& operator=(U&& v);
//
// Constraints:
// - is_same_v<expected, remove_cvref_t<U>> is false; and
// - remove_cvref_t<U> is not a specialization of unexpected; and
// - is_constructible_v<T, U> is true; and
// - is_assignable_v<T&, U> is true; and
// - is_nothrow_constructible_v<T, U> || is_nothrow_move_constructible_v<T> ||
//   is_nothrow_move_constructible_v<E> is true.
//
// Effects:
// - If has_value() is true, equivalent to: val = std::forward<U>(v);
// - Otherwise, equivalent to:
//   reinit-expected(val, unex, std::forward<U>(v));
//   has_val = true;
// - Returns: *this.

#include <cassert>
#include <concepts>
#include <expected>
#include <type_traits>
#include <utility>

#include "../../types.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

struct NotCopyConstructible {
  NotCopyConstructible(const NotCopyConstructible&)            = delete;
  NotCopyConstructible& operator=(const NotCopyConstructible&) = default;
};

struct NotCopyAssignable {
  NotCopyAssignable(const NotCopyAssignable&)            = default;
  NotCopyAssignable& operator=(const NotCopyAssignable&) = delete;
};

// Test constraints
static_assert(std::is_assignable_v<std::expected<int, int>&, int>);

// is_same_v<expected, remove_cvref_t<U>>
// it is true because it covered by the copy assignment
static_assert(std::is_assignable_v<std::expected<int, int>&, std::expected<int, int>>);

// remove_cvref_t<U> is a specialization of unexpected
// it is true because it covered the unepxected overload
static_assert(std::is_assignable_v<std::expected<int, int>&, std::unexpected<int>>);

// !is_constructible_v<T, U>
struct NoCtorFromInt {
  NoCtorFromInt(int) = delete;
  NoCtorFromInt& operator=(int);
};
static_assert(!std::is_assignable_v<std::expected<NoCtorFromInt, int>&, int>);

// !is_assignable_v<T&, U>
struct NoAssignFromInt {
  explicit NoAssignFromInt(int);
  NoAssignFromInt& operator=(int) = delete;
};
static_assert(!std::is_assignable_v<std::expected<NoAssignFromInt, int>&, int>);

template <bool moveNoexcept, bool convertNoexcept>
struct MaybeNoexcept {
  explicit MaybeNoexcept(int) noexcept(convertNoexcept);
  MaybeNoexcept(MaybeNoexcept&&) noexcept(moveNoexcept);
  MaybeNoexcept& operator=(MaybeNoexcept&&) = default;
  MaybeNoexcept& operator=(int);
};

// !is_nothrow_constructible_v<T, U> && !is_nothrow_move_constructible_v<T> &&
// is_nothrow_move_constructible_v<E>
static_assert(std::is_assignable_v<std::expected<MaybeNoexcept<false, false>, int>&, int>);

// is_nothrow_constructible_v<T, U> && !is_nothrow_move_constructible_v<T> &&
// !is_nothrow_move_constructible_v<E>
static_assert(std::is_assignable_v<std::expected<MaybeNoexcept<false, true>, MaybeNoexcept<false, false>>&, int>);

// !is_nothrow_constructible_v<T, U> && is_nothrow_move_constructible_v<T> &&
// !is_nothrow_move_constructible_v<E>
static_assert(std::is_assignable_v<std::expected<MaybeNoexcept<true, false>, MaybeNoexcept<false, false>>&, int>);

// !is_nothrow_constructible_v<T, U> && !is_nothrow_move_constructible_v<T> &&
// !is_nothrow_move_constructible_v<E>
static_assert(!std::is_assignable_v<std::expected<MaybeNoexcept<false, false>, MaybeNoexcept<false, false>>&, int>);

constexpr bool test() {
  // If has_value() is true, equivalent to: val = std::forward<U>(v);
  // Copy
  {
    Traced::state oldState{};
    Traced::state newState{};
    std::expected<Traced, int> e1(std::in_place, oldState, 5);
    Traced u(newState, 10);
    decltype(auto) x = (e1 = u);
    static_assert(std::same_as<decltype(x), std::expected<Traced, int>&>);
    TC_ASSERT_EXPR(&x == &e1);

    TC_ASSERT_EXPR(e1.has_value());
    TC_ASSERT_EXPR(e1.value().data_ == 10);
    TC_ASSERT_EXPR(oldState.copyAssignCalled);
  }

  // If has_value() is true, equivalent to: val = std::forward<U>(v);
  // Move
  {
    Traced::state oldState{};
    Traced::state newState{};
    std::expected<Traced, int> e1(std::in_place, oldState, 5);
    Traced u(newState, 10);
    decltype(auto) x = (e1 = std::move(u));
    static_assert(std::same_as<decltype(x), std::expected<Traced, int>&>);
    TC_ASSERT_EXPR(&x == &e1);

    TC_ASSERT_EXPR(e1.has_value());
    TC_ASSERT_EXPR(e1.value().data_ == 10);
    TC_ASSERT_EXPR(oldState.moveAssignCalled);
  }

  // Otherwise, equivalent to:
  //   reinit-expected(val, unex, std::forward<U>(v));
  // is_nothrow_constructible_v<T, U> && !is_nothrow_move_constructible_v<T> &&
  // !is_nothrow_move_constructible_v<E>
  // copy
  //
  //  In this case, it should call the branch
  //    destroy_at(addressof(oldval));
  //    construct_at(addressof(newval), std::forward<Args>(args)...);
  {
    BothMayThrow::state oldState{};
    std::expected<MoveThrowConvNoexcept, BothMayThrow> e1(std::unexpect, oldState, 5);
    const int i      = 10;
    decltype(auto) x = (e1 = i);
    static_assert(std::same_as<decltype(x), std::expected<MoveThrowConvNoexcept, BothMayThrow>&>);
    TC_ASSERT_EXPR(&x == &e1);

    TC_ASSERT_EXPR(e1.has_value());
    TC_ASSERT_EXPR(e1.value().data_ == 10);

    TC_ASSERT_EXPR(!oldState.copyCtorCalled);
    TC_ASSERT_EXPR(!oldState.moveCtorCalled);
    TC_ASSERT_EXPR(oldState.dtorCalled);
    TC_ASSERT_EXPR(e1.value().copiedFromInt);
  }

  // Otherwise, equivalent to:
  //   reinit-expected(val, unex, std::forward<U>(v));
  // is_nothrow_constructible_v<T, U> && !is_nothrow_move_constructible_v<T> &&
  // !is_nothrow_move_constructible_v<E>
  // move
  //
  //  In this case, it should call the branch
  //    destroy_at(addressof(oldval));
  //    construct_at(addressof(newval), std::forward<Args>(args)...);
  {
    BothMayThrow::state oldState{};
    std::expected<MoveThrowConvNoexcept, BothMayThrow> e1(std::unexpect, oldState, 5);
    decltype(auto) x = (e1 = 10);
    static_assert(std::same_as<decltype(x), std::expected<MoveThrowConvNoexcept, BothMayThrow>&>);
    TC_ASSERT_EXPR(&x == &e1);

    TC_ASSERT_EXPR(e1.has_value());
    TC_ASSERT_EXPR(e1.value().data_ == 10);

    TC_ASSERT_EXPR(!oldState.copyCtorCalled);
    TC_ASSERT_EXPR(!oldState.moveCtorCalled);
    TC_ASSERT_EXPR(oldState.dtorCalled);
    TC_ASSERT_EXPR(e1.value().movedFromInt);
  }

  // Otherwise, equivalent to:
  //   reinit-expected(val, unex, std::forward<U>(v));
  // !is_nothrow_constructible_v<T, U> && is_nothrow_move_constructible_v<T> &&
  // !is_nothrow_move_constructible_v<E>
  // copy
  //
  //  In this case, it should call the branch
  //  T tmp(std::forward<Args>(args)...);
  //  destroy_at(addressof(oldval));
  //  construct_at(addressof(newval), std::move(tmp));
  {
    BothMayThrow::state oldState{};
    std::expected<MoveNoexceptConvThrow, BothMayThrow> e1(std::unexpect, oldState, 5);
    const int i      = 10;
    decltype(auto) x = (e1 = i);
    static_assert(std::same_as<decltype(x), std::expected<MoveNoexceptConvThrow, BothMayThrow>&>);
    TC_ASSERT_EXPR(&x == &e1);

    TC_ASSERT_EXPR(e1.has_value());
    TC_ASSERT_EXPR(e1.value().data_ == 10);

    TC_ASSERT_EXPR(!oldState.copyCtorCalled);
    TC_ASSERT_EXPR(!oldState.moveCtorCalled);
    TC_ASSERT_EXPR(oldState.dtorCalled);
    TC_ASSERT_EXPR(!e1.value().copiedFromInt);
    TC_ASSERT_EXPR(e1.value().movedFromTmp);
  }

  // Otherwise, equivalent to:
  //   reinit-expected(val, unex, std::forward<U>(v));
  // !is_nothrow_constructible_v<T, U> && is_nothrow_move_constructible_v<T> &&
  // !is_nothrow_move_constructible_v<E>
  // move
  //
  //  In this case, it should call the branch
  //  T tmp(std::forward<Args>(args)...);
  //  destroy_at(addressof(oldval));
  //  construct_at(addressof(newval), std::move(tmp));
  {
    BothMayThrow::state oldState{};
    std::expected<MoveNoexceptConvThrow, BothMayThrow> e1(std::unexpect, oldState, 5);
    decltype(auto) x = (e1 = 10);
    static_assert(std::same_as<decltype(x), std::expected<MoveNoexceptConvThrow, BothMayThrow>&>);
    TC_ASSERT_EXPR(&x == &e1);

    TC_ASSERT_EXPR(e1.has_value());
    TC_ASSERT_EXPR(e1.value().data_ == 10);

    TC_ASSERT_EXPR(!oldState.copyCtorCalled);
    TC_ASSERT_EXPR(!oldState.moveCtorCalled);
    TC_ASSERT_EXPR(oldState.dtorCalled);
    TC_ASSERT_EXPR(!e1.value().copiedFromInt);
    TC_ASSERT_EXPR(e1.value().movedFromTmp);
  }

  // Otherwise, equivalent to:
  //   reinit-expected(val, unex, std::forward<U>(v));
  // !is_nothrow_constructible_v<T, U> && !is_nothrow_move_constructible_v<T> &&
  // is_nothrow_move_constructible_v<E>
  // copy
  //
  //  In this case, it should call the branch
  //  U tmp(std::move(oldval));
  //  destroy_at(addressof(oldval));
  //  try {
  //    construct_at(addressof(newval), std::forward<Args>(args)...);
  //  } catch (...) {
  //    construct_at(addressof(oldval), std::move(tmp));
  //    throw;
  //  }
  {
    TracedNoexcept::state oldState{};
    std::expected<BothMayThrow, TracedNoexcept> e1(std::unexpect, oldState, 5);
    const int i      = 10;
    decltype(auto) x = (e1 = i);
    static_assert(std::same_as<decltype(x), std::expected<BothMayThrow, TracedNoexcept>&>);
    TC_ASSERT_EXPR(&x == &e1);

    TC_ASSERT_EXPR(e1.has_value());
    TC_ASSERT_EXPR(e1.value().data_ == 10);

    TC_ASSERT_EXPR(!oldState.copyCtorCalled);
    TC_ASSERT_EXPR(oldState.moveCtorCalled);
    TC_ASSERT_EXPR(oldState.dtorCalled);
    TC_ASSERT_EXPR(e1.value().copiedFromInt);
  }

  // Otherwise, equivalent to:
  //   reinit-expected(val, unex, std::forward<U>(v));
  // !is_nothrow_constructible_v<T, U> && !is_nothrow_move_constructible_v<T> &&
  // is_nothrow_move_constructible_v<E>
  // move
  //
  //  In this case, it should call the branch
  //  U tmp(std::move(oldval));
  //  destroy_at(addressof(oldval));
  //  try {
  //    construct_at(addressof(newval), std::forward<Args>(args)...);
  //  } catch (...) {
  //    construct_at(addressof(oldval), std::move(tmp));
  //    throw;
  //  }
  {
    TracedNoexcept::state oldState{};
    std::expected<BothMayThrow, TracedNoexcept> e1(std::unexpect, oldState, 5);
    decltype(auto) x = (e1 = 10);
    static_assert(std::same_as<decltype(x), std::expected<BothMayThrow, TracedNoexcept>&>);
    TC_ASSERT_EXPR(&x == &e1);

    TC_ASSERT_EXPR(e1.has_value());
    TC_ASSERT_EXPR(e1.value().data_ == 10);

    TC_ASSERT_EXPR(!oldState.copyCtorCalled);
    TC_ASSERT_EXPR(oldState.moveCtorCalled);
    TC_ASSERT_EXPR(oldState.dtorCalled);
    TC_ASSERT_EXPR(e1.value().movedFromInt);
  }

  // Test default template argument.
  // Without it, the template parameter cannot be deduced from an initializer list
  {
    struct Bar {
      int i;
      int j;
      constexpr Bar(int ii, int jj) : i(ii), j(jj) {}
    };

    std::expected<Bar, int> e({5, 6});
    e = {7, 8};
    TC_ASSERT_EXPR(e.value().i == 7);
    TC_ASSERT_EXPR(e.value().j == 8);
  }

  return true;
}

void testException() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  std::expected<ThrowOnConvert, int> e1(std::unexpect, 5);
#ifndef _LIBCPP_NO_EXCEPTIONS
  try {
    e1 = 10;
    TC_ASSERT_EXPR(false);
  } catch (Except) {
    TC_ASSERT_EXPR(!e1.has_value());
    TC_ASSERT_EXPR(e1.error() == 5);
  }
#endif // _LIBCPP_NO_EXCEPTIONS

#endif // TEST_HAS_NO_EXCEPTIONS
}

int tc_utilities_expected_expected_expected_assign_assign_U(void) {
  test();
  static_assert(test());
  testException();
  return 0;
}
