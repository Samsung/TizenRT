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

// E& error() & noexcept;
// const E& error() const & noexcept;
// E&& error() && noexcept;
// const E&& error() const && noexcept;

#include <cassert>
#include <concepts>
#include <expected>
#include <utility>
#include "libcxx_tc_common.h"

template <class T>
concept ErrorNoexcept =
    requires(T&& t) {
      { std::forward<T>(t).error() } noexcept;
    };

static_assert(!ErrorNoexcept<int>);
static_assert(ErrorNoexcept<std::bad_expected_access<int>&>);
static_assert(ErrorNoexcept<std::bad_expected_access<int> const&>);
static_assert(ErrorNoexcept<std::bad_expected_access<int>&&>);
static_assert(ErrorNoexcept<std::bad_expected_access<int> const&&>);

void test() {
  // &
  {
    std::bad_expected_access<int> e(5);
    decltype(auto) i = e.error();
    static_assert(std::same_as<decltype(i), int&>);
    TC_ASSERT_EXPR(i == 5);
  }

  // const &
  {
    const std::bad_expected_access<int> e(5);
    decltype(auto) i = e.error();
    static_assert(std::same_as<decltype(i), const int&>);
    TC_ASSERT_EXPR(i == 5);
  }

  // &&
  {
    std::bad_expected_access<int> e(5);
    decltype(auto) i = std::move(e).error();
    static_assert(std::same_as<decltype(i), int&&>);
    TC_ASSERT_EXPR(i == 5);
  }

  // const &&
  {
    const std::bad_expected_access<int> e(5);
    decltype(auto) i = std::move(e).error();
    static_assert(std::same_as<decltype(i), const int&&>);
    TC_ASSERT_EXPR(i == 5);
  }
}

int tc_utilities_expected_expected_bad_error_member(void) {
  test();
  return 0;
}
