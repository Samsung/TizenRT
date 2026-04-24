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

// Test default construction:
//
// constexpr mapping() noexcept = default;

#include <mdspan>
#include <cassert>
#include <cstdint>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class E>
constexpr void test_construction() {
  using M = std::layout_left::mapping<E>;
  ASSERT_NOEXCEPT(M{});
  M m;
  E e;

  // check correct extents are returned
  ASSERT_NOEXCEPT(m.extents());
  TC_ASSERT_EXPR(m.extents() == e);

  // check required_span_size()
  typename E::index_type expected_size = 1;
  for (typename E::rank_type r = 0; r < E::rank(); r++)
    expected_size *= e.extent(r);
  TC_ASSERT_EXPR(m.required_span_size() == expected_size);
}

constexpr bool test() {
  constexpr size_t D = std::dynamic_extent;
  test_construction<std::extents<int>>();
  test_construction<std::extents<unsigned, D>>();
  test_construction<std::extents<unsigned, 7>>();
  test_construction<std::extents<unsigned, 7, 8>>();
  test_construction<std::extents<int64_t, D, 8, D, D>>();
  return true;
}

int tc_containers_views_mdspan_layout_left_ctor_default(void) {
  test();
  static_assert(test());
  return 0;
}
