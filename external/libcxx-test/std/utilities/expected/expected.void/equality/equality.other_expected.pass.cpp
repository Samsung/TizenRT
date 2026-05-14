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

// template<class T2, class E2> requires (is_void_v<T2>)
//   friend constexpr bool operator==(const expected& x, const expected<T2, E2>& y);

#include <cassert>
#include <concepts>
#include <expected>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

// Test constraint
template <class T1, class T2>
concept CanCompare = requires(T1 t1, T2 t2) { t1 == t2; };

struct Foo{};
static_assert(!CanCompare<Foo, Foo>);

static_assert(CanCompare<std::expected<void, int>, std::expected<void, int>>);
static_assert(CanCompare<std::expected<void, int>, std::expected<void, short>>);

// Note this is true because other overloads in expected<non-void> are unconstrained
static_assert(CanCompare<std::expected<void, int>, std::expected<int, int>>);

constexpr bool test() {
  // x.has_value() && y.has_value()
  {
    const std::expected<void, int> e1;
    const std::expected<void, int> e2;
    TC_ASSERT_EXPR(e1 == e2);
  }

  // !x.has_value() && y.has_value()
  {
    const std::expected<void, int> e1(std::unexpect, 5);
    const std::expected<void, int> e2;
    TC_ASSERT_EXPR(e1 != e2);
  }

  // x.has_value() && !y.has_value()
  {
    const std::expected<void, int> e1;
    const std::expected<void, int> e2(std::unexpect, 10);
    const std::expected<void, int> e3(std::unexpect, 5);
    TC_ASSERT_EXPR(e1 != e2);
    TC_ASSERT_EXPR(e1 != e3);
  }

  // !x.has_value() && !y.has_value()
  {
    const std::expected<void, int> e1(std::unexpect, 5);
    const std::expected<void, int> e2(std::unexpect, 10);
    const std::expected<void, int> e3(std::unexpect, 5);
    TC_ASSERT_EXPR(e1 != e2);
    TC_ASSERT_EXPR(e1 == e3);
  }

  return true;
}

int tc_utilities_expected_expected_void_equality_equality_other_expected(void) {
  test();
  static_assert(test());
  return 0;
}
