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

// constexpr index_type required_span_size() const noexcept;
//
// Returns: extents().fwd-prod-of-extents(extents_type::rank()).


#include <mdspan>
#include <cassert>
#include <cstdint>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class E>
constexpr void test_required_span_size(E e, typename E::index_type expected_size) {
  using M = std::layout_right::mapping<E>;
  const M m(e);

  ASSERT_NOEXCEPT(m.required_span_size());
  TC_ASSERT_EXPR(m.required_span_size() == expected_size);
}

constexpr bool test() {
  constexpr size_t D = std::dynamic_extent;
  test_required_span_size(std::extents<int>(), 1);
  test_required_span_size(std::extents<unsigned, D>(0), 0);
  test_required_span_size(std::extents<unsigned, D>(1), 1);
  test_required_span_size(std::extents<unsigned, D>(7), 7);
  test_required_span_size(std::extents<unsigned, 7>(), 7);
  test_required_span_size(std::extents<unsigned, 7, 8>(), 56);
  test_required_span_size(std::extents<int64_t, D, 8, D, D>(7, 9, 10), 5040);
  test_required_span_size(std::extents<int64_t, 1, 8, D, D>(9, 10), 720);
  test_required_span_size(std::extents<int64_t, 1, 0, D, D>(9, 10), 0);
  return true;
}

int tc_containers_views_mdspan_layout_right_required_span_size(void) {
  test();
  static_assert(test());
  return 0;
}
