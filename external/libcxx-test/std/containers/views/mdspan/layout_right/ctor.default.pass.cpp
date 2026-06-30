//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
  using M = std::layout_right::mapping<E>;
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

int tc_containers_views_mdspan_layout_right_ctor_default(void) {
  test();
  static_assert(test());
  return 0;
}
