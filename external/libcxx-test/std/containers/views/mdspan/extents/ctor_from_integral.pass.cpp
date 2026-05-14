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

// Test construction from integral:
//
// template<class ... OtherIndexTypes>
//     constexpr explicit extents(OtherIndexTypes ... exts) noexcept;
//
// Let N be sizeof...(OtherIndexTypes), and let
// exts_arr be array<index_type, N>{static_cast<index_type>(std::move(exts))...}.
//
// Constraints:
//   * (is_convertible_v<OtherIndexTypes, index_type> && ...) is true,
//   * (is_nothrow_constructible_v<index_type, OtherIndexType> && ...) is true, and
//   * N == rank_dynamic() || N == rank() is true.
//
// Preconditions:
//   * If N != rank_dynamic() is true, exts_arr[r] equals Er for each r for which
//     Er is a static extent, and
//   * either
//     - sizeof...(exts) == 0 is true, or
//     - each element of exts is nonnegative and is representable as a value of type index_type.
//

#include <mdspan>
#include <cassert>
#include <type_traits>

#include "../ConvertibleToIntegral.h"
#include "CtorTestCombinations.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

struct IntegralCtorTest {
  template <class E, class AllExtents, class Extents, size_t... Indices>
  static constexpr void test_construction(AllExtents all_ext, Extents ext, std::index_sequence<Indices...>) {
    // construction from indices
    ASSERT_NOEXCEPT(E(ext[Indices]...));
    test_runtime_observers(E(ext[Indices]...), all_ext);
  }
};

int tc_containers_views_mdspan_extents_ctor_from_integral(void) {
  test_index_type_combo<IntegralCtorTest>();
  static_assert(test_index_type_combo<IntegralCtorTest>());

  constexpr size_t D = std::dynamic_extent;
  using E            = std::extents<int, 1, D, 3, D>;

  // check can't construct from too few arguments
  static_assert(!std::is_constructible_v<E, int>, "extents constructible from illegal arguments");
  // check can't construct from rank_dynamic < #args < rank
  static_assert(!std::is_constructible_v<E, int, int, int>, "extents constructible from illegal arguments");
  // check can't construct from too many arguments
  static_assert(!std::is_constructible_v<E, int, int, int, int, int>, "extents constructible from illegal arguments");

  // test construction fails from types not convertible to index_type but convertible to other integer types
  static_assert(std::is_convertible_v<IntType, int>, "Test helper IntType unexpectedly not convertible to int");
  static_assert(!std::is_constructible_v< std::extents<unsigned long, D>, IntType>,
                "extents constructible from illegal arguments");
  return 0;
}
