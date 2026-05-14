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

// template<class... Args>
//   constexpr T& emplace(Args&&... args) noexcept;
// Constraints: is_nothrow_constructible_v<T, Args...> is true.
//
// Effects: Equivalent to:
// if (has_value()) {
//   destroy_at(addressof(val));
// } else {
//   destroy_at(addressof(unex));
//   has_val = true;
// }
// return *construct_at(addressof(val), std::forward<Args>(args)...);

#include <cassert>
#include <concepts>
#include <expected>
#include <type_traits>
#include <utility>

#include "../../types.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, class... Args>
concept CanEmplace = requires(T t, Args&&... args) { t.emplace(std::forward<Args>(args)...); };

static_assert(CanEmplace<std::expected<int, int>, int>);

template <bool Noexcept>
struct CtorFromInt {
  CtorFromInt(int) noexcept(Noexcept);
  CtorFromInt(int, int) noexcept(Noexcept);
};

static_assert(CanEmplace<std::expected<CtorFromInt<true>, int>, int>);
static_assert(CanEmplace<std::expected<CtorFromInt<true>, int>, int, int>);
static_assert(!CanEmplace<std::expected<CtorFromInt<false>, int>, int>);
static_assert(!CanEmplace<std::expected<CtorFromInt<false>, int>, int, int>);

constexpr bool test() {
  // has_value
  {
    BothNoexcept::state oldState{};
    BothNoexcept::state newState{};
    std::expected<BothNoexcept, int> e(std::in_place, oldState, 5);
    decltype(auto) x = e.emplace(newState, 10);
    static_assert(std::same_as<decltype(x), BothNoexcept&>);
    TC_ASSERT_EXPR(&x == &(*e));

    TC_ASSERT_EXPR(oldState.dtorCalled);
    TC_ASSERT_EXPR(e.has_value());
    TC_ASSERT_EXPR(e.value().data_ == 10);
  }

  // !has_value
  {
    BothMayThrow::state oldState{};
    std::expected<int, BothMayThrow> e(std::unexpect, oldState, 5);
    decltype(auto) x = e.emplace(10);
    static_assert(std::same_as<decltype(x), int&>);
    TC_ASSERT_EXPR(&x == &(*e));

    TC_ASSERT_EXPR(oldState.dtorCalled);
    TC_ASSERT_EXPR(e.has_value());
    TC_ASSERT_EXPR(e.value() == 10);
  }

  // TailClobberer
  {
    std::expected<TailClobberer<0>, bool> e(std::unexpect);
    e.emplace();
    TC_ASSERT_EXPR(e.has_value());
  }

  return true;
}

int tc_utilities_expected_expected_expected_assign_emplace(void) {
  test();
  static_assert(test());
  return 0;
}
