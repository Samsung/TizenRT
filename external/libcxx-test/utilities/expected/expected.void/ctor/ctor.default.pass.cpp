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

// constexpr expected() noexcept;

#include <cassert>
#include <expected>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

// Test noexcept

struct NoDefaultCtor {
  constexpr NoDefaultCtor() = delete;
};

static_assert(std::is_nothrow_default_constructible_v<std::expected<void, int>>);
static_assert(std::is_nothrow_default_constructible_v<std::expected<void, NoDefaultCtor>>);

struct MyInt {
  int i;
  friend constexpr bool operator==(const MyInt&, const MyInt&) = default;
};

constexpr bool test() {
  // default constructible
  {
    std::expected<void, int> e;
    TC_ASSERT_EXPR(e.has_value());
  }

  // non-default constructible
  {
    std::expected<void, NoDefaultCtor> e;
    TC_ASSERT_EXPR(e.has_value());
  }

  return true;
}

int tc_utilities_expected_expected_void_ctor_ctor_default(void) {
  test();
  static_assert(test());
  return 0;
}
