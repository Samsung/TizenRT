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
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <optional>

// [optional.nullops], comparison with nullopt

// template<class T>
//   constexpr strong_ordering operator<=>(const optional<T>&, nullopt_t) noexcept;

#include <cassert>
#include <compare>
#include <optional>

#include "test_comparisons.h"
#include "libcxx_tc_common.h"

constexpr bool test() {
  {
    std::optional<int> op;
    TC_ASSERT_EXPR((std::nullopt <=> op) == std::strong_ordering::equal);
    TC_ASSERT_EXPR(testOrder(std::nullopt, op, std::strong_ordering::equal));
    TC_ASSERT_EXPR((op <=> std::nullopt) == std::strong_ordering::equal);
    TC_ASSERT_EXPR(testOrder(op, std::nullopt, std::strong_ordering::equal));
  }
  {
    std::optional<int> op{1};
    TC_ASSERT_EXPR((std::nullopt <=> op) == std::strong_ordering::less);
    TC_ASSERT_EXPR(testOrder(std::nullopt, op, std::strong_ordering::less));
  }
  {
    std::optional<int> op{1};
    TC_ASSERT_EXPR((op <=> std::nullopt) == std::strong_ordering::greater);
    TC_ASSERT_EXPR(testOrder(op, std::nullopt, std::strong_ordering::greater));
  }

  return true;
}

int tc_utilities_optional_optional_nullops_compare_three_way(void) {
  TC_ASSERT_EXPR(test());
  static_assert(test());
  return 0;
}
