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

// Test construction from span:
//
// template<class OtherIndexType, size_t N>
//     constexpr explicit(N != rank_dynamic()) extents(span<OtherIndexType, N> exts) noexcept;
//
// Constraints:
//   * is_convertible_v<const OtherIndexType&, index_type> is true,
//   * is_nothrow_constructible_v<index_type, const OtherIndexType&> is true, and
//   * N == rank_dynamic() || N == rank() is true.
//
// Preconditions:
//   * If N != rank_dynamic() is true, exts[r] equals Er for each r for which
//     Er is a static extent, and
//   * either
//     - N is zero, or
//     - exts[r] is nonnegative and is representable as a value of type index_type
//       for every rank index r.
//

#include <mdspan>
#include <cassert>
#include <array>
#include <span>
#include <type_traits>

#include "../ConvertibleToIntegral.h"
#include "CtorTestCombinations.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

struct SpanCtorTest {
  template <class E, class T, size_t N, class Extents, size_t... Indices>
  static constexpr void test_construction(std::array<T, N> all_ext, Extents ext, std::index_sequence<Indices...>) {
    ASSERT_NOEXCEPT(E(ext));
    if constexpr (N == E::rank_dynamic()) {
      test_implicit_construction_call<E>(std::span(ext), all_ext);
    }
    test_runtime_observers(E(std::span(ext)), all_ext);
  }
};

template <class E>
struct implicit_construction {
  bool value;
  implicit_construction(E) : value(true) {}
  template <class T>
  implicit_construction(T) : value(false) {}
};

int tc_containers_views_mdspan_extents_ctor_from_span(void) {
  test_index_type_combo<SpanCtorTest>();
  static_assert(test_index_type_combo<SpanCtorTest>());

  constexpr size_t D = std::dynamic_extent;
  using E            = std::extents<int, 1, D, 3, D>;

  // check can't construct from too few arguments
  static_assert(!std::is_constructible_v<E, std::span<int, 1>>, "extents constructible from illegal arguments");
  // check can't construct from rank_dynamic < #args < rank
  static_assert(!std::is_constructible_v<E, std::span<int, 3>>, "extents constructible from illegal arguments");
  // check can't construct from too many arguments
  static_assert(!std::is_constructible_v<E, std::span<int, 5>>, "extents constructible from illegal arguments");

  // test implicit construction fails from span and array if all extents are given
  std::array a5{3, 4, 5, 6, 7};
  std::span<int, 5> s5(a5.data(), 5);
  // check that explicit construction works, i.e. no error
  static_assert(std::is_constructible_v< std::extents<int, D, D, 5, D, D>, decltype(s5)>,
                "extents unexpectectly not constructible");
  // check that implicit construction doesn't work
  TC_ASSERT_EXPR((implicit_construction<std::extents<int, D, D, 5, D, D>>(s5).value == false));

  // test construction fails from types not convertible to index_type but convertible to other integer types
  static_assert(std::is_convertible_v<IntType, int>, "Test helper IntType unexpectedly not convertible to int");
  static_assert(!std::is_constructible_v< std::extents<unsigned long, D>, std::span<IntType, 1>>,
                "extents constructible from illegal arguments");

  // index_type is not nothrow constructible
  static_assert(std::is_convertible_v<IntType, unsigned char>);
  static_assert(std::is_convertible_v<const IntType&, unsigned char>);
  static_assert(!std::is_nothrow_constructible_v<unsigned char, const IntType&>);
  static_assert(!std::is_constructible_v<std::dextents<unsigned char, 2>, std::span<IntType, 2>>);

  // convertible from non-const to index_type but not  from const
  static_assert(std::is_convertible_v<IntTypeNC, int>);
  static_assert(!std::is_convertible_v<const IntTypeNC&, int>);
  static_assert(std::is_nothrow_constructible_v<int, IntTypeNC>);
  static_assert(!std::is_constructible_v<std::dextents<int, 2>, std::span<IntTypeNC, 2>>);
  return 0;
}
