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

// template<class OtherIndexType, size_t... OtherExtents>
//     constexpr explicit(see below) extents(const extents<OtherIndexType, OtherExtents...>&) noexcept;
//
// Constraints:
//   * sizeof...(OtherExtents) == rank() is true.
//   * ((OtherExtents == dynamic_extent || Extents == dynamic_extent ||
//       OtherExtents == Extents) && ...) is true.
//
// Preconditions:
//   * other.extent(r) equals Er for each r for which Er is a static extent, and
//   * either
//      - sizeof...(OtherExtents) is zero, or
//      - other.extent(r) is representable as a value of type index_type for
//        every rank index r of other.
//
// Remarks: The expression inside explicit is equivalent to:
//          (((Extents != dynamic_extent) && (OtherExtents == dynamic_extent)) || ... ) ||
//          (numeric_limits<index_type>::max() < numeric_limits<OtherIndexType>::max())

#include <mdspan>
#include <type_traits>
#include <concepts>
#include <cassert>
#include <limits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class To, class From>
constexpr void test_implicit_conversion(To dest, From src) {
  TC_ASSERT_EXPR(dest == src);
}

template <bool implicit, class To, class From>
constexpr void test_conversion(From src) {
  To dest(src);
  TC_ASSERT_EXPR(dest == src);
  if constexpr (implicit) {
    dest = src;
    TC_ASSERT_EXPR(dest == src);
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
  test_conversion<idx_convertible && false, std::extents<T1, 5, D>>(std::extents<T2, D, D>(5, 5));
  test_conversion<idx_convertible && true,  std::extents<T1, D, D>>(std::extents<T2, D, D>(5, 5));
  test_conversion<idx_convertible && true,  std::extents<T1, D, D>>(std::extents<T2, D, 7>(5));
  test_conversion<idx_convertible && true,  std::extents<T1, 5, 7>>(std::extents<T2, 5, 7>());
  test_conversion<idx_convertible && false, std::extents<T1, 5, D, 8, D, D>>(std::extents<T2, D, D, 8, 9, 1>(5, 7));
  test_conversion<idx_convertible && true,  std::extents<T1, D, D, D, D, D>>(
                                            std::extents<T2, D, D, D, D, D>(5, 7, 8, 9, 1));
  test_conversion<idx_convertible && true,  std::extents<T1, D, D, 8, 9, D>>(std::extents<T2, D, 7, 8, 9, 1>(5));
  test_conversion<idx_convertible && true,  std::extents<T1, 5, 7, 8, 9, 1>>(std::extents<T2, 5, 7, 8, 9, 1>());
  // clang-format on
}

constexpr void test_no_implicit_conversion() {
  constexpr size_t D = std::dynamic_extent;
  // Sanity check that one static to dynamic conversion works
  static_assert(std::is_constructible_v<std::extents<int, D>, std::extents<int, 5>>, "");
  static_assert(std::is_convertible_v<std::extents<int, 5>, std::extents<int, D>>, "");

  // Check that dynamic to static conversion only works explicitly only
  static_assert(std::is_constructible_v<std::extents<int, 5>, std::extents<int, D>>, "");
  static_assert(!std::is_convertible_v<std::extents<int, D>, std::extents<int, 5>>, "");

  // Sanity check that one static to dynamic conversion works
  static_assert(std::is_constructible_v<std::extents<int, D, 7>, std::extents<int, 5, 7>>, "");
  static_assert(std::is_convertible_v<std::extents<int, 5, 7>, std::extents<int, D, 7>>, "");

  // Check that dynamic to static conversion only works explicitly only
  static_assert(std::is_constructible_v<std::extents<int, 5, 7>, std::extents<int, D, 7>>, "");
  static_assert(!std::is_convertible_v<std::extents<int, D, 7>, std::extents<int, 5, 7>>, "");

  // Sanity check that smaller index_type to larger index_type conversion works
  static_assert(std::is_constructible_v<std::extents<size_t, 5>, std::extents<int, 5>>, "");
  static_assert(std::is_convertible_v<std::extents<int, 5>, std::extents<size_t, 5>>, "");

  // Check that larger index_type to smaller index_type conversion works explicitly only
  static_assert(std::is_constructible_v<std::extents<int, 5>, std::extents<size_t, 5>>, "");
  static_assert(!std::is_convertible_v<std::extents<size_t, 5>, std::extents<int, 5>>, "");
}

constexpr void test_rank_mismatch() {
  constexpr size_t D = std::dynamic_extent;

  static_assert(!std::is_constructible_v<std::extents<int, D>, std::extents<int>>, "");
  static_assert(!std::is_constructible_v<std::extents<int>, std::extents<int, D, D>>, "");
  static_assert(!std::is_constructible_v<std::extents<int, D>, std::extents<int, D, D>>, "");
  static_assert(!std::is_constructible_v<std::extents<int, D, D, D>, std::extents<int, D, D>>, "");
}

constexpr void test_static_extent_mismatch() {
  constexpr size_t D = std::dynamic_extent;

  static_assert(!std::is_constructible_v<std::extents<int, D, 5>, std::extents<int, D, 4>>, "");
  static_assert(!std::is_constructible_v<std::extents<int, 5>, std::extents<int, 4>>, "");
  static_assert(!std::is_constructible_v<std::extents<int, 5, D>, std::extents<int, 4, D>>, "");
}

constexpr bool test() {
  test_conversion<int, int>();
  test_conversion<int, size_t>();
  test_conversion<size_t, int>();
  test_conversion<size_t, long>();
  test_no_implicit_conversion();
  test_rank_mismatch();
  test_static_extent_mismatch();
  return true;
}

int tc_containers_views_mdspan_extents_conversion(void) {
  test();
  static_assert(test());

  return 0;
}
