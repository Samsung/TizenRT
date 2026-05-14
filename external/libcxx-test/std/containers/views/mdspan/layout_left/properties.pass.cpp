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

// namespace std {
//   template<class Extents>
//   class layout_left::mapping {
//
//     ...
//     static constexpr bool is_always_unique() noexcept { return true; }
//     static constexpr bool is_always_exhaustive() noexcept { return true; }
//     static constexpr bool is_always_strided() noexcept { return true; }
//
//     static constexpr bool is_unique() noexcept { return true; }
//     static constexpr bool is_exhaustive() noexcept { return true; }
//     static constexpr bool is_strided() noexcept { return true; }
//     ...
//   };
// }

#include <mdspan>
#include <type_traits>
#include <concepts>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class E>
constexpr void test_layout_mapping_left() {
  using M = std::layout_left::template mapping<E>;
  TC_ASSERT_EXPR(M::is_unique() == true);
  TC_ASSERT_EXPR(M::is_exhaustive() == true);
  TC_ASSERT_EXPR(M::is_strided() == true);
  TC_ASSERT_EXPR(M::is_always_unique() == true);
  TC_ASSERT_EXPR(M::is_always_exhaustive() == true);
  TC_ASSERT_EXPR(M::is_always_strided() == true);
  ASSERT_NOEXCEPT(std::declval<M>().is_unique());
  ASSERT_NOEXCEPT(std::declval<M>().is_exhaustive());
  ASSERT_NOEXCEPT(std::declval<M>().is_strided());
  ASSERT_NOEXCEPT(M::is_always_unique());
  ASSERT_NOEXCEPT(M::is_always_exhaustive());
  ASSERT_NOEXCEPT(M::is_always_strided());
}

constexpr bool test() {
  constexpr size_t D = std::dynamic_extent;
  test_layout_mapping_left<std::extents<int>>();
  test_layout_mapping_left<std::extents<char, 4, 5>>();
  test_layout_mapping_left<std::extents<unsigned, D, 4>>();
  test_layout_mapping_left<std::extents<size_t, D, D, D, D>>();
  return true;
}

int tc_containers_views_mdspan_layout_left_properties(void) {
  test();
  static_assert(test());
  return 0;
}
