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

// template<class OtherExtents>
//   constexpr explicit(!is_convertible_v<OtherExtents, extents_type>)
//     mapping(const layout_right::mapping<OtherExtents>&) noexcept;

// Constraints:
//   - extents_type::rank() <= 1 is true, and
//   - is_constructible_v<extents_type, OtherExtents> is true.
//
// Preconditions: other.required_span_size() is representable as a value of type index_type

#include <mdspan>
#include <type_traits>
#include <cassert>
#include <limits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class To, class From>
constexpr void test_implicit_conversion(To dest, From src) {
  TC_ASSERT_EXPR(dest.extents() == src.extents());
}

template <bool implicit, class ToE, class FromE>
constexpr void test_conversion(FromE src_exts) {
  using To   = std::layout_left::mapping<ToE>;
  using From = std::layout_right::mapping<FromE>;
  From src(src_exts);

  ASSERT_NOEXCEPT(To(src));
  To dest(src);

  TC_ASSERT_EXPR(dest.extents() == src.extents());
  if constexpr (implicit) {
    dest = src;
    TC_ASSERT_EXPR(dest.extents() == src.extents());
    test_implicit_conversion<To, From>(src, src);
  }
}

template <class T1, class T2>
constexpr void test_conversion() {
  constexpr size_t D = std::dynamic_extent;
  constexpr bool idx_convertible =
      static_cast<size_t>(std::numeric_limits<T1>::max()) >= static_cast<size_t>(std::numeric_limits<T2>::max());

  // clang-format off
  test_conversion<idx_convertible && true,  std::extents<T1>>(std::extents<T2>());
  test_conversion<idx_convertible && true,  std::extents<T1, D>>(std::extents<T2, D>(5));
  test_conversion<idx_convertible && false, std::extents<T1, 5>>(std::extents<T2, D>(5));
  test_conversion<idx_convertible && true,  std::extents<T1, 5>>(std::extents<T2, 5>());
  // clang-format on
}

template <class IdxT, size_t... Extents>
using lr_mapping_t = typename std::layout_right::template mapping<std::extents<IdxT, Extents...>>;
template <class IdxT, size_t... Extents>
using ll_mapping_t = typename std::layout_left::template mapping<std::extents<IdxT, Extents...>>;

constexpr void test_no_implicit_conversion() {
  constexpr size_t D = std::dynamic_extent;

  // Sanity check that one static to dynamic conversion works
  static_assert(std::is_constructible_v<ll_mapping_t<int, D>, lr_mapping_t<int, 5>>);
  static_assert(std::is_convertible_v<lr_mapping_t<int, 5>, ll_mapping_t<int, D>>);

  // Check that dynamic to static conversion only works explicitly
  static_assert(std::is_constructible_v<ll_mapping_t<int, 5>, lr_mapping_t<int, D>>);
  static_assert(!std::is_convertible_v<lr_mapping_t<int, D>, ll_mapping_t<int, 5>>);

  // Sanity check that smaller index_type to larger index_type conversion works
  static_assert(std::is_constructible_v<ll_mapping_t<size_t, 5>, lr_mapping_t<int, 5>>);
  static_assert(std::is_convertible_v<lr_mapping_t<int, 5>, ll_mapping_t<size_t, 5>>);

  // Check that larger index_type to smaller index_type conversion works explicitly only
  static_assert(std::is_constructible_v<ll_mapping_t<int, 5>, lr_mapping_t<size_t, 5>>);
  static_assert(!std::is_convertible_v<lr_mapping_t<size_t, 5>, ll_mapping_t<int, 5>>);
}

constexpr void test_rank_mismatch() {
  constexpr size_t D = std::dynamic_extent;

  static_assert(!std::is_constructible_v<lr_mapping_t<int, D>, lr_mapping_t<int>>);
  static_assert(!std::is_constructible_v<lr_mapping_t<int>, lr_mapping_t<int, D, D>>);
  static_assert(!std::is_constructible_v<lr_mapping_t<int, D>, lr_mapping_t<int, D, D>>);
  static_assert(!std::is_constructible_v<lr_mapping_t<int, D, D, D>, lr_mapping_t<int, D, D>>);
}

constexpr void test_static_extent_mismatch() {
  static_assert(!std::is_constructible_v<ll_mapping_t<int, 5>, lr_mapping_t<int, 4>>);
}

constexpr void test_rank_greater_one() {
  constexpr size_t D = std::dynamic_extent;

  static_assert(!std::is_constructible_v<ll_mapping_t<int, D, D>, lr_mapping_t<int, D, D>>);
  static_assert(!std::is_constructible_v<ll_mapping_t<int, 1, 1>, lr_mapping_t<int, 1, 1>>);
  static_assert(!std::is_constructible_v<ll_mapping_t<int, D, D, D>, lr_mapping_t<int, D, D, D>>);
}

constexpr bool test() {
  test_conversion<int, int>();
  test_conversion<int, size_t>();
  test_conversion<size_t, int>();
  test_conversion<size_t, long>();
  test_no_implicit_conversion();
  test_rank_mismatch();
  test_static_extent_mismatch();
  test_rank_greater_one();
  return true;
}

int tc_containers_views_mdspan_layout_left_ctor_layout_right(void) {
  test();
  static_assert(test());
  return 0;
}
