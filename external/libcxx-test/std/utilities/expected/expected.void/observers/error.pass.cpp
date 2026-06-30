//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr const E& error() const & noexcept;
// constexpr E& error() & noexcept;
// constexpr E&& error() && noexcept;
// constexpr const E&& error() const && noexcept;

#include <cassert>
#include <concepts>
#include <expected>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

// Test noexcept
template <class T>
concept ErrorNoexcept =
    requires(T t) {
      { std::forward<T>(t).error() } noexcept;
    };

static_assert(!ErrorNoexcept<int>);

static_assert(ErrorNoexcept<std::expected<void, int>&>);
static_assert(ErrorNoexcept<const std::expected<void, int>&>);
static_assert(ErrorNoexcept<std::expected<void, int>&&>);
static_assert(ErrorNoexcept<const std::expected<void, int>&&>);

constexpr bool test() {
  // non-const &
  {
    std::expected<void, int> e(std::unexpect, 5);
    decltype(auto) x = e.error();
    static_assert(std::same_as<decltype(x), int&>);
    TC_ASSERT_EXPR(x == 5);
  }

  // const &
  {
    const std::expected<void, int> e(std::unexpect, 5);
    decltype(auto) x = e.error();
    static_assert(std::same_as<decltype(x), const int&>);
    TC_ASSERT_EXPR(x == 5);
  }

  // non-const &&
  {
    std::expected<void, int> e(std::unexpect, 5);
    decltype(auto) x = std::move(e).error();
    static_assert(std::same_as<decltype(x), int&&>);
    TC_ASSERT_EXPR(x == 5);
  }

  // const &&
  {
    const std::expected<void, int> e(std::unexpect, 5);
    decltype(auto) x = std::move(e).error();
    static_assert(std::same_as<decltype(x), const int&&>);
    TC_ASSERT_EXPR(x == 5);
  }

  return true;
}

int tc_utilities_expected_expected_void_observers_error(void) {
  test();
  static_assert(test());
  return 0;
}
