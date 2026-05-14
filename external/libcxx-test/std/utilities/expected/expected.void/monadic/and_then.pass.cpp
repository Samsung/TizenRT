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

// <expected>

// template<class F> constexpr auto and_then(F&& f) &;
// template<class F> constexpr auto and_then(F&& f) const &;
// template<class F> constexpr auto and_then(F&& f) &&;
// template<class F> constexpr auto and_then(F&& f) const &&;

#include <expected>
#include <concepts>
#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>
#include "libcxx_tc_common.h"

struct NonCopyable {
  constexpr NonCopyable(int) {}
  NonCopyable(const NonCopyable&) = delete;
};

struct NonMovable {
  constexpr NonMovable(int) {}
  NonMovable(NonMovable&&) = delete;
};

template <class E, class F>
concept has_and_then =
    requires(E&& e, F&& f) {
      { std::forward<E>(e).and_then(std::forward<F>(f)) };
    };

std::expected<void, int> return_int() { return {}; }
std::expected<void, NonCopyable> return_noncopyable() { return {}; }
std::expected<void, NonMovable> return_nonmovable() { return {}; }

static_assert(has_and_then<std::expected<void, int>&, decltype(return_int)>);
static_assert(!has_and_then<std::expected<void, NonCopyable>&, decltype(return_noncopyable)>);
static_assert(has_and_then<const std::expected<void, int>&, decltype(return_int)>);
static_assert(!has_and_then<const std::expected<void, NonCopyable>&, decltype(return_noncopyable)>);
static_assert(has_and_then<std::expected<void, int>&&, decltype(return_int)>);
static_assert(!has_and_then<std::expected<void, NonMovable>&&, decltype(return_nonmovable)>);
static_assert(has_and_then<const std::expected<void, int>&&, decltype(return_int)>);
static_assert(!has_and_then<const std::expected<void, NonMovable>&&, decltype(return_nonmovable)>);

// [LWG 3877] https://cplusplus.github.io/LWG/issue3877, check constraint failing but not compile error inside the function body.
static_assert(!has_and_then<const std::expected<int, std::unique_ptr<int>>&, int()>);
static_assert(!has_and_then<const std::expected<int, std::unique_ptr<int>>&&, int()>);

constexpr void test_val_types() {
  // Test & overload
  {
    auto l = [] -> std::expected<int, int> { return 2; };
    std::expected<void, int> v;
    std::same_as<std::expected<int, int>> decltype(auto) val = v.and_then(l);
    TC_ASSERT_EXPR(val == 2);
  }

  // Test const& overload
  {
    auto l = [] -> std::expected<int, int> { return 2; };
    const std::expected<void, int> v;
    TC_ASSERT_EXPR(v.and_then(l).value() == 2);
    static_assert(std::is_same_v< decltype(v.and_then(l)), std::expected<int, int>>);
  }

  // Test && overload
  {
    auto l = [] -> std::expected<int, int> { return 2; };
    std::expected<void, int> v;
    std::same_as<std::expected<int, int>> decltype(auto) val = std::move(v).and_then(l);
    TC_ASSERT_EXPR(val == 2);
  }

  // Test const&& overload
  {
    auto l = [] -> std::expected<int, int> { return 2; };
    const std::expected<void, int> v;
    std::same_as<std::expected<int, int>> decltype(auto) val = std::move(v).and_then(l);
    TC_ASSERT_EXPR(val == 2);
  }
}

constexpr void test_fail() {
  // Test & overload
  {
    auto f = [] -> std::expected<int, int> {
      TC_ASSERT_EXPR(false);
      return 0;
    };
    std::expected<void, int> v(std::unexpected<int>(2));
    std::same_as<std::expected<int, int>> decltype(auto) val = v.and_then(f);
    TC_ASSERT_EXPR(val.error() == 2);
  }

  // Test const& overload
  {
    auto f = [] -> std::expected<int, int> {
      TC_ASSERT_EXPR(false);
      return 0;
    };
    const std::expected<void, int> v(std::unexpected<int>(2));
    std::same_as<std::expected<int, int>> decltype(auto) val = v.and_then(f);
    TC_ASSERT_EXPR(val.error() == 2);
  }

  // Test && overload
  {
    auto f = [] -> std::expected<int, int> {
      TC_ASSERT_EXPR(false);
      return 0;
    };
    std::expected<void, int> v(std::unexpected<int>(2));
    std::same_as<std::expected<int, int>> decltype(auto) val = std::move(v).and_then(f);
    TC_ASSERT_EXPR(val.error() == 2);
  }

  // Test const&& overload
  {
    auto f = [] -> std::expected<int, int> {
      TC_ASSERT_EXPR(false);
      return 0;
    };
    const std::expected<void, int> v(std::unexpected<int>(2));
    std::same_as<std::expected<int, int>> decltype(auto) val = std::move(v).and_then(f);
    TC_ASSERT_EXPR(val.error() == 2);
  }
}

constexpr bool test() {
  test_fail();
  test_val_types();
  return true;
}

int tc_utilities_expected_expected_void_monadic_and_then(void) {
  test();
  static_assert(test());

  return 0;
}
