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

// constexpr const E& error() const & noexcept;

#include <cassert>
#include <concepts>
#include <expected>
#include <utility>
#include "libcxx_tc_common.h"

template <class T>
concept ErrorNoexcept =
    requires(const T& t) {
      { t.error() } noexcept;
    };

static_assert(!ErrorNoexcept<int>);
static_assert(ErrorNoexcept<std::unexpected<int>>);

constexpr bool test() {
  const std::unexpected<int> unex(5);
  decltype(auto) i = unex.error();
  static_assert(std::same_as<decltype(i), const int&>);
  TC_ASSERT_EXPR(i == 5);
  return true;
}

int tc_utilities_expected_expected_unexpected_observer_error_const_ref(void) {
  test();
  static_assert(test());
  return 0;
}
