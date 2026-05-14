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

// <mdspan>

// template <class... Integrals>
// explicit extents(Integrals...) -> see below;
//   Constraints: (is_convertible_v<Integrals, size_t> && ...) is true.
//
// Remarks: The deduced type is dextents<size_t, sizeof...(Integrals)>.

#include <mdspan>
#include <cassert>

#include "../ConvertibleToIntegral.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class E, class Expected>
constexpr void test(E e, Expected expected) {
  ASSERT_SAME_TYPE(E, Expected);
  TC_ASSERT_EXPR(e == expected);
}

constexpr bool test() {
  constexpr std::size_t D = std::dynamic_extent;

  test(std::extents(), std::extents<size_t>());
  test(std::extents(1), std::extents<std::size_t, D>(1));
  test(std::extents(1, 2u), std::extents<std::size_t, D, D>(1, 2u));
  test(std::extents(1, 2u, 3, 4, 5, 6, 7, 8, 9),
       std::extents<std::size_t, D, D, D, D, D, D, D, D, D>(1, 2u, 3, 4, 5, 6, 7, 8, 9));
  return true;
}

int tc_containers_views_mdspan_extents_ctad(void) {
  test();
  static_assert(test());

  return 0;
}
