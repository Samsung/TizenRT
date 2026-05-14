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

// constexpr const T* operator->() const noexcept;
// constexpr T* operator->() noexcept;

#include <cassert>
#include <concepts>
#include <expected>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

// Test noexcept
template <class T>
concept ArrowNoexcept =
    requires(T t) {
      { t.operator->() } noexcept;
    };

static_assert(!ArrowNoexcept<int>);

static_assert(ArrowNoexcept<std::expected<int, int>>);
static_assert(ArrowNoexcept<const std::expected<int, int>>);

constexpr bool test() {
  // const
  {
    const std::expected<int, int> e(5);
    std::same_as<const int*> decltype(auto) x = e.operator->();
    TC_ASSERT_EXPR(x == &(e.value()));
    TC_ASSERT_EXPR(*x == 5);
  }

  // non-const
  {
    std::expected<int, int> e(5);
    std::same_as<int*> decltype(auto) x = e.operator->();
    TC_ASSERT_EXPR(x == &(e.value()));
    TC_ASSERT_EXPR(*x == 5);
  }

  return true;
}

int tc_utilities_expected_expected_expected_observers_arrow(void) {
  test();
  static_assert(test());
  return 0;
}
