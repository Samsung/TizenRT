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

// constexpr void value() const &;
// constexpr void value() &&;

#include <cassert>
#include <concepts>
#include <expected>
#include <type_traits>
#include <utility>

#include "MoveOnly.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

constexpr bool test() {
  // const &
  {
    const std::expected<void, int> e;
    e.value();
    static_assert(std::is_same_v<decltype(e.value()), void>);
  }

  // &
  {
    std::expected<void, int> e;
    e.value();
    static_assert(std::is_same_v<decltype(e.value()), void>);
  }

  // &&
  {
    std::expected<void, int> e;
    std::move(e).value();
    static_assert(std::is_same_v<decltype(std::move(e).value()), void>);
  }

  // const &&
  {
    const std::expected<void, int> e;
    std::move(e).value();
    static_assert(std::is_same_v<decltype(std::move(e).value()), void>);
  }

  return true;
}

void testException() {
#ifndef TEST_HAS_NO_EXCEPTIONS

  // int
  {
    const std::expected<void, int> e(std::unexpect, 5);
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      e.value();
      TC_ASSERT_EXPR(false);
    } catch (const std::bad_expected_access<int>& ex) {
      TC_ASSERT_EXPR(ex.error() == 5);
    }
#endif // _LIBCPP_NO_EXCEPTIONS
  }

  // MoveOnly
  {
    std::expected<void, MoveOnly> e(std::unexpect, 5);
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      std::move(e).value();
      TC_ASSERT_EXPR(false);
    } catch (const std::bad_expected_access<MoveOnly>& ex) {
      TC_ASSERT_EXPR(ex.error() == 5);
    }
#endif // _LIBCPP_NO_EXCEPTIONS
  }

#endif // TEST_HAS_NO_EXCEPTIONS
}

int tc_utilities_expected_expected_void_observers_value(void) {
  test();
  static_assert(test());
  testException();
  return 0;
}
