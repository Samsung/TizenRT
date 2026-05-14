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

// template<class G = E> constexpr E error_or(G&& e) const &;
// template<class G = E> constexpr E error_or(G&& e) &&;

#include <cassert>
#include <concepts>
#include <expected>
#include <type_traits>
#include <utility>
#include "libcxx_tc_common.h"

struct ConstructFromInt {
  int value;
  constexpr ConstructFromInt(int v) : value(v) {}
};

constexpr bool test_default_template_arg() {
  // const &, has_value()
  {
    const std::expected<int, ConstructFromInt> e(5);
    std::same_as<ConstructFromInt> decltype(auto) x = e.error_or(10);
    TC_ASSERT_EXPR(x.value == 10);
  }

  // const &, !has_value()
  {
    const std::expected<int, ConstructFromInt> e(std::unexpect, 5);
    std::same_as<ConstructFromInt> decltype(auto) x = e.error_or(10);
    TC_ASSERT_EXPR(x.value == 5);
  }

  // &&, has_value()
  {
    const std::expected<int, ConstructFromInt> e(5);
    std::same_as<ConstructFromInt> decltype(auto) x = std::move(e).error_or(10);
    TC_ASSERT_EXPR(x.value == 10);
  }

  // &&, !has_value()
  {
    const std::expected<int, ConstructFromInt> e(std::unexpect, 5);
    std::same_as<ConstructFromInt> decltype(auto) x = std::move(e).error_or(10);
    TC_ASSERT_EXPR(x.value == 5);
  }

  return true;
}

constexpr bool test() {
  // const &, has_value()
  {
    const std::expected<int, int> e(5);
    std::same_as<int> decltype(auto) x = e.error_or(10);
    TC_ASSERT_EXPR(x == 10);
  }

  // const &, !has_value()
  {
    const std::expected<int, int> e(std::unexpect, 5);
    std::same_as<int> decltype(auto) x = e.error_or(10);
    TC_ASSERT_EXPR(x == 5);
  }

  // &&, has_value()
  {
    std::expected<int, int> e(5);
    std::same_as<int> decltype(auto) x = std::move(e).error_or(10);
    TC_ASSERT_EXPR(x == 10);
  }

  // &&, !has_value()
  {
    std::expected<int, int> e(std::unexpect, 5);
    std::same_as<int> decltype(auto) x = std::move(e).error_or(10);
    TC_ASSERT_EXPR(x == 5);
  }

  return true;
}

int tc_utilities_expected_expected_expected_observers_error_or(void) {
  test();
  static_assert(test());
  test_default_template_arg();
  static_assert(test_default_template_arg());

  return 0;
}
