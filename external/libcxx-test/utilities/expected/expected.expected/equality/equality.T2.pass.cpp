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

// template<class T2> friend constexpr bool operator==(const expected& x, const T2& v);

#include <cassert>
#include <concepts>
#include <expected>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct Data {
  int i;
  constexpr Data(int ii) : i(ii) {}

  friend constexpr bool operator==(const Data& data, int ii) { return data.i == ii; }
};

constexpr bool test() {
  // x.has_value()
  {
    const std::expected<Data, int> e1(std::in_place, 5);
    int i2 = 10;
    int i3 = 5;
    TC_ASSERT_EXPR(e1 != i2);
    TC_ASSERT_EXPR(e1 == i3);
  }

  // !x.has_value()
  {
    const std::expected<Data, int> e1(std::unexpect, 5);
    int i2 = 10;
    int i3 = 5;
    TC_ASSERT_EXPR(e1 != i2);
    TC_ASSERT_EXPR(e1 != i3);
  }

  return true;
}

int tc_utilities_expected_expected_expected_equality_equality_T2(void) {
  test();
  static_assert(test());
  return 0;
}
