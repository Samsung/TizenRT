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

// constexpr explicit expected(in_place_t) noexcept;

#include <cassert>
#include <expected>
#include <type_traits>
#include <utility>
#include "libcxx_tc_common.h"

// test explicit
static_assert(std::is_constructible_v<std::expected<void, int>, std::in_place_t>);
static_assert(!std::is_convertible_v<std::in_place_t, std::expected<void, int>>);

// test noexcept
static_assert(std::is_nothrow_constructible_v<std::expected<void, int>, std::in_place_t>);

constexpr bool test() {
  std::expected<void, int> e(std::in_place);
  TC_ASSERT_EXPR(e.has_value());

  return true;
}

int tc_utilities_expected_expected_void_ctor_ctor_inplace(void) {
  test();
  static_assert(test());
  return 0;
}
