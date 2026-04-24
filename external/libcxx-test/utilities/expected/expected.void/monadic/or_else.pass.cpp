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

// template<class F> constexpr auto or_else(F&& f) &;
// template<class F> constexpr auto or_else(F&& f) const &;
// template<class F> constexpr auto or_else(F&& f) &&;
// template<class F> constexpr auto or_else(F&& f) const &&;

#include <expected>
#include <concepts>
#include <cassert>
#include <type_traits>
#include <utility>
#include "libcxx_tc_common.h"

constexpr void test_val_types() {
  // Test & overload
  {
    auto l = [](auto) -> std::expected<void, long> { return {}; };
    std::expected<void, int> v(std::unexpected<int>(1));
    std::same_as<std::expected<void, long>> decltype(auto) val = v.or_else(l);
    TC_ASSERT_EXPR(val.has_value());
  }

  // Test const& overload
  {
    auto l = [](auto) -> std::expected<void, long> { return {}; };
    const std::expected<void, int> v(std::unexpected<int>(1));
    std::same_as<std::expected<void, long>> decltype(auto) val = v.or_else(l);
    TC_ASSERT_EXPR(val.has_value());
  }

  // Test && overload
  {
    auto l = [](auto) -> std::expected<void, long> { return {}; };
    std::expected<void, int> v(std::unexpected<int>(1));
    std::same_as<std::expected<void, long>> decltype(auto) val = std::move(v).or_else(l);
    TC_ASSERT_EXPR(val.has_value());
  }

  // Test const&& overload
  {
    auto l = [](auto) -> std::expected<void, long> { return {}; };
    const std::expected<void, int> v(std::unexpected<int>(1));
    std::same_as<std::expected<void, long>> decltype(auto) val = std::move(v).or_else(l);
    TC_ASSERT_EXPR(val.has_value());
  }
}

constexpr void test_fail() {
  auto never_called = [](auto) -> std::expected<void, long> {
    TC_ASSERT_EXPR(false);
    return std::expected<void, long>(std::unexpected<long>(5));
  };

  // Test & overload
  {
    std::expected<void, int> v;
    std::same_as<std::expected<void, long>> decltype(auto) val = v.or_else(never_called);
    TC_ASSERT_EXPR(val.has_value());
  }

  // Test const& overload
  {
    const std::expected<void, int> v;
    std::same_as<std::expected<void, long>> decltype(auto) val = v.or_else(never_called);
    TC_ASSERT_EXPR(val.has_value());
  }

  // Test && overload
  {
    std::expected<void, int> v;
    std::same_as<std::expected<void, long>> decltype(auto) val = std::move(v).or_else(never_called);
    TC_ASSERT_EXPR(val.has_value());
  }

  // Test const&& overload
  {
    const std::expected<void, int> v;
    std::same_as<std::expected<void, long>> decltype(auto) val = std::move(v).or_else(never_called);
    TC_ASSERT_EXPR(val.has_value());
  }
}

constexpr bool test() {
  test_fail();
  test_val_types();
  return true;
}

int tc_utilities_expected_expected_void_monadic_or_else(void) {
  test();
  static_assert(test());

  return 0;
}
