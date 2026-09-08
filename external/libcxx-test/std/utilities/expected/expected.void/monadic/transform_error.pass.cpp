//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// GCC has a issue for `Guaranteed copy elision for potentially-overlapping non-static data members`,
// please refer to: https://gcc.gnu.org/bugzilla/show_bug.cgi?id=108333, but we have a workaround to
// avoid this issue.

// <expected>

// template<class F> constexpr auto transform_error(F&& f) &;
// template<class F> constexpr auto transform_error(F&& f) const &;
// template<class F> constexpr auto transform_error(F&& f) &&;
// template<class F> constexpr auto transform_error(F&& f) const &&;

#include <expected>
#include <concepts>
#include <cassert>
#include <type_traits>
#include <utility>
#include "libcxx_tc_common.h"

struct NonCopy {
  int value;
  constexpr explicit NonCopy(int val) : value(val) {}
  NonCopy(const NonCopy&) = delete;
};

constexpr void test_val_types() {
  // Test & overload
  {
    auto l = [](auto) -> int { return 1; };
    std::expected<void, int> v(std::unexpected<int>(2));
    std::same_as<std::expected<void, int>> decltype(auto) val = v.transform_error(l);
    TC_ASSERT_EXPR(val.error() == 1);
  }

  // Test const& overload
  {
    auto l = [](auto) -> int { return 1; };
    const std::expected<void, int> v(std::unexpected<int>(2));
    std::same_as<std::expected<void, int>> decltype(auto) val = v.transform_error(l);
    TC_ASSERT_EXPR(val.error() == 1);
  }

  // Test && overload
  {
    auto l = [](auto) -> int { return 1; };
    std::expected<void, int> v(std::unexpected<int>(2));
    std::same_as<std::expected<void, int>> decltype(auto) val = std::move(v).transform_error(l);
    TC_ASSERT_EXPR(val.error() == 1);
  }

  // Test const&& overload
  {
    auto l = [](auto) -> int { return 1; };
    const std::expected<void, int> v(std::unexpected<int>(2));
    std::same_as<std::expected<void, int>> decltype(auto) val = std::move(v).transform_error(l);
    TC_ASSERT_EXPR(val.error() == 1);
  }
}

constexpr void test_fail() {
  // Test & overload
  {
    auto l = [](auto) -> int {
      TC_ASSERT_EXPR(false);
      return 0;
    };
    std::expected<void, int> v;
    std::same_as<std::expected<void, int>> decltype(auto) val = v.transform_error(l);
    TC_ASSERT_EXPR(val.has_value());
  }

  // Test const& overload
  {
    auto l = [](auto) -> int {
      TC_ASSERT_EXPR(false);
      return 0;
    };
    const std::expected<void, int> v;
    std::same_as<std::expected<void, int>> decltype(auto) val = v.transform_error(l);
    TC_ASSERT_EXPR(val.has_value());
  }

  // Test && overload
  {
    auto l = [](auto) -> int {
      TC_ASSERT_EXPR(false);
      return 0;
    };
    std::expected<void, int> v;
    std::same_as<std::expected<void, int>> decltype(auto) val = std::move(v).transform_error(l);
    TC_ASSERT_EXPR(val.has_value());
  }

  // Test const&& overload
  {
    auto l = [](auto) -> int {
      TC_ASSERT_EXPR(false);
      return 0;
    };
    const std::expected<void, int> v;
    std::same_as<std::expected<void, int>> decltype(auto) val = std::move(v).transform_error(l);
    TC_ASSERT_EXPR(val.has_value());
  }
}

// check unex member is direct-non-list-initialized with invoke(std::forward<F>(f))
constexpr void test_direct_non_list_init() {
  auto x = [](int i) { return NonCopy(i); };
  std::expected<void, int> v(std::unexpected<int>(2));
  std::expected<void, NonCopy> nv = v.transform_error(x);
  TC_ASSERT_EXPR(nv.error().value == 2);
}

constexpr bool test() {
  test_fail();
  test_val_types();
  test_direct_non_list_init();
  return true;
}

int tc_utilities_expected_expected_void_monadic_transform_error(void) {
  test();
  static_assert(test());

  return 0;
}
