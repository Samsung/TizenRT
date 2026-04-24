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

// template<class F> constexpr auto transform_error(F&& f) &;
// template<class F> constexpr auto transform_error(F&& f) const &;
// template<class F> constexpr auto transform_error(F&& f) &&;
// template<class F> constexpr auto transform_error(F&& f) const &&;

#include <expected>
#include <concepts>
#include <cassert>
#include <memory>
#include <type_traits>
#include <utility>
#include "libcxx_tc_common.h"

template <class E, class F>
concept has_transform =
    requires(E&& e, F&& f) {
      { std::forward<E>(e).transform(std::forward<F>(f)) };
    };

// [LWG 3877] https://cplusplus.github.io/LWG/issue3877, check constraint failing but not compile error inside the function body.
static_assert(!has_transform<const std::expected<int, std::unique_ptr<int>>&, int()>);
static_assert(!has_transform<const std::expected<int, std::unique_ptr<int>>&&, int()>);

constexpr void test_val_types() {
  // Test & overload
  {
    auto l = [] -> int { return 1; };
    std::expected<void, int> v;
    std::same_as<std::expected<int, int>> decltype(auto) val = v.transform(l);
    TC_ASSERT_EXPR(val == 1);
  }

  // Test const& overload
  {
    auto l = [] -> int { return 1; };
    const std::expected<void, int> v;
    std::same_as<std::expected<int, int>> decltype(auto) val = v.transform(l);
    TC_ASSERT_EXPR(val == 1);
  }

  // Test && overload
  {
    auto l = [] -> int { return 1; };
    std::expected<void, int> v;
    std::same_as<std::expected<int, int>> decltype(auto) val = std::move(v).transform(l);
    TC_ASSERT_EXPR(val == 1);
  }

  // Test const&& overload
  {
    auto l = [] -> int { return 1; };
    const std::expected<void, int> v;
    std::same_as<std::expected<int, int>> decltype(auto) val = std::move(v).transform(l);
    TC_ASSERT_EXPR(val == 1);
  }
}

constexpr void test_fail() {
  // Test & overload
  {
    auto l = [] -> int {
      TC_ASSERT_EXPR(false);
      return 0;
    };
    std::expected<void, int> v(std::unexpected<int>(5));
    std::same_as<std::expected<int, int>> decltype(auto) val = v.transform(l);
    TC_ASSERT_EXPR(val.error() == 5);
  }

  // Test const& overload
  {
    auto l = [] -> int {
      TC_ASSERT_EXPR(false);
      return 0;
    };
    const std::expected<void, int> v(std::unexpected<int>(5));
    std::same_as<std::expected<int, int>> decltype(auto) val = v.transform(l);
    TC_ASSERT_EXPR(val.error() == 5);
  }

  // Test && overload
  {
    auto l = [] -> int {
      TC_ASSERT_EXPR(false);
      return 0;
    };
    std::expected<void, int> v(std::unexpected<int>(5));
    std::same_as<std::expected<int, int>> decltype(auto) val = std::move(v).transform(l);
    TC_ASSERT_EXPR(val.error() == 5);
  }

  // Test const&& overload
  {
    auto l = [] -> int {
      TC_ASSERT_EXPR(false);
      return 0;
    };
    const std::expected<void, int> v(std::unexpected<int>(5));
    std::same_as<std::expected<int, int>> decltype(auto) val = std::move(v).transform(l);
    TC_ASSERT_EXPR(val.error() == 5);
  }
}

constexpr bool test() {
  test_fail();
  test_val_types();
  return true;
}

int tc_utilities_expected_expected_void_monadic_transform(void) {
  test();
  static_assert(test());

  return 0;
}
