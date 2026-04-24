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

// template<class G>
//   constexpr expected& operator=(const unexpected<G>& e);
//
// Let GF be const G&
// Constraints:
// - is_constructible_v<E, GF> is true; and
// - is_assignable_v<E&, GF> is true; and
// - is_nothrow_constructible_v<E, GF> || is_nothrow_move_constructible_v<T> ||
//   is_nothrow_move_constructible_v<E> is true.
//
// Effects:
// - If has_value() is true, equivalent to:
//   reinit-expected(unex, val, std::forward<GF>(e.error()));
//   has_val = false;
// - Otherwise, equivalent to: unex = std::forward<GF>(e.error());
// Returns: *this.

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

struct MoveMayThrow {
  MoveMayThrow(MoveMayThrow const&)            = default;
  MoveMayThrow& operator=(const MoveMayThrow&) = default;
  MoveMayThrow(MoveMayThrow&&) noexcept(false) {}
  MoveMayThrow& operator=(MoveMayThrow&&) noexcept(false) { return *this; }
};

// Test constraints
static_assert(std::is_assignable_v<std::expected<int, int>&, const std::unexpected<int>&>);

// !is_constructible_v<E, GF>
static_assert(
    !std::is_assignable_v<std::expected<int, NotCopyConstructible>&, const std::unexpected<NotCopyConstructible>&>);

// !is_assignable_v<E&, GF>
static_assert(!std::is_assignable_v<std::expected<int, NotCopyAssignable>&, const std::unexpected<NotCopyAssignable>&>);

template <bool moveNoexcept, bool convertNoexcept>
struct MaybeNoexcept {
  explicit MaybeNoexcept(int) noexcept(convertNoexcept);
  MaybeNoexcept(MaybeNoexcept&&) noexcept(moveNoexcept);
  MaybeNoexcept& operator=(MaybeNoexcept&&) = default;
  MaybeNoexcept& operator=(int);
};

// !is_nothrow_constructible_v<E, GF> && !is_nothrow_move_constructible_v<T> &&
// is_nothrow_move_constructible_v<E>
static_assert(std::is_assignable_v<std::expected<MaybeNoexcept<false, false>, MaybeNoexcept<true, false>>&,
                                   const std::unexpected<int>&>);

// is_nothrow_constructible_v<E, GF> && !is_nothrow_move_constructible_v<T> &&
// !is_nothrow_move_constructible_v<E>
static_assert(std::is_assignable_v<std::expected<MaybeNoexcept<false, false>, MaybeNoexcept<false, true>>&,
                                   const std::unexpected<int>&>);

// !is_nothrow_constructible_v<E, GF> && is_nothrow_move_constructible_v<T> &&
// !is_nothrow_move_constructible_v<E>
static_assert(std::is_assignable_v<std::expected<MaybeNoexcept<true, true>, MaybeNoexcept<false, false>>&,
                                   const std::unexpected<int>&>);

// !is_nothrow_constructible_v<E, GF> && !is_nothrow_move_constructible_v<T> &&
// !is_nothrow_move_constructible_v<E>
static_assert(!std::is_assignable_v<std::expected<MaybeNoexcept<false, false>, MaybeNoexcept<false, false>>&,
                                    const std::unexpected<int>&>);

constexpr bool test() {
  // - If has_value() is true, equivalent to:
  //   reinit-expected(unex, val, std::forward<GF>(e.error()));
  // is_nothrow_constructible_v<E, GF>
  //
  //  In this case, it should call the branch
  //    destroy_at(addressof(oldval));
  //    construct_at(addressof(newval), std::forward<Args>(args)...);
  {
    BothNoexcept::state oldState{};
    std::expected<BothNoexcept, BothNoexcept> e(std::in_place, oldState, 5);
    const std::unexpected<int> un(10);
    decltype(auto) x = (e = un);
    static_assert(std::same_as<decltype(x), std::expected<BothNoexcept, BothNoexcept>&>);
    TC_ASSERT_EXPR(&x == &e);

    TC_ASSERT_EXPR(!oldState.moveCtorCalled);
    TC_ASSERT_EXPR(oldState.dtorCalled);
    TC_ASSERT_EXPR(e.error().copiedFromInt);
  }

  // - If has_value() is true, equivalent to:
  //   reinit-expected(unex, val, std::forward<GF>(e.error()));
  // !is_nothrow_constructible_v<E, GF> && is_nothrow_move_constructible_v<E>
  //
  //  In this case, it should call the branch
  //  T tmp(std::forward<Args>(args)...);
  //  destroy_at(addressof(oldval));
  //  construct_at(addressof(newval), std::move(tmp));
  {
    BothNoexcept::state oldState{};
    std::expected<BothNoexcept, MoveNoexceptConvThrow> e(std::in_place, oldState, 5);
    const std::unexpected<int> un(10);
    decltype(auto) x = (e = un);
    static_assert(std::same_as<decltype(x), std::expected<BothNoexcept, MoveNoexceptConvThrow>&>);
    TC_ASSERT_EXPR(&x == &e);

    TC_ASSERT_EXPR(!oldState.moveCtorCalled);
    TC_ASSERT_EXPR(oldState.dtorCalled);
    TC_ASSERT_EXPR(!e.error().copiedFromInt);
    TC_ASSERT_EXPR(e.error().movedFromTmp);
  }

  // - If has_value() is true, equivalent to:
  //   reinit-expected(unex, val, std::forward<GF>(e.error()));
  // !is_nothrow_constructible_v<E, GF> && !is_nothrow_move_constructible_v<E>
  // is_nothrow_move_constructible_v<T>
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
    BothNoexcept::state oldState{};
    std::expected<BothNoexcept, BothMayThrow> e(std::in_place, oldState, 5);
    const std::unexpected<int> un(10);
    decltype(auto) x = (e = un);
    static_assert(std::same_as<decltype(x), std::expected<BothNoexcept, BothMayThrow>&>);
    TC_ASSERT_EXPR(&x == &e);

    TC_ASSERT_EXPR(oldState.moveCtorCalled);
    TC_ASSERT_EXPR(oldState.dtorCalled);
    TC_ASSERT_EXPR(e.error().copiedFromInt);
  }

  // Otherwise, equivalent to: unex = std::forward<GF>(e.error());
  {
    Traced::state oldState{};
    Traced::state newState{};
    std::expected<int, Traced> e1(std::unexpect, oldState, 5);
    const std::unexpected<Traced> e(std::in_place, newState, 10);
    decltype(auto) x = (e1 = e);
    static_assert(std::same_as<decltype(x), std::expected<int, Traced>&>);
    TC_ASSERT_EXPR(&x == &e1);

    TC_ASSERT_EXPR(!e1.has_value());
    TC_ASSERT_EXPR(e1.error().data_ == 10);
    TC_ASSERT_EXPR(oldState.copyAssignCalled);
  }
  return true;
}

void testException() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  std::expected<int, ThrowOnConvert> e1(std::in_place, 5);
  const std::unexpected<int> un(10);
#ifndef _LIBCPP_NO_EXCEPTIONS
  try {
    e1 = un;
    TC_ASSERT_EXPR(false);
  } catch (Except) {
    TC_ASSERT_EXPR(e1.has_value());
    TC_ASSERT_EXPR(*e1 == 5);
  }
#endif // _LIBCPP_NO_EXCEPTIONS
#endif // TEST_HAS_NO_EXCEPTIONS
}

int tc_utilities_expected_expected_expected_assign_assign_unexpected_copy(void) {
  test();
  static_assert(test());
  testException();
  return 0;
}
