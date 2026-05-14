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

// Test default iteration:
//
// template<class... Indices>
//   constexpr index_type operator()(Indices...) const noexcept;
//
// Constraints:
//   * sizeof...(Indices) == extents_type::rank() is true,
//   * (is_convertible_v<Indices, index_type> && ...) is true, and
//   * (is_nothrow_constructible_v<index_type, Indices> && ...) is true.
//
// Preconditions:
//   * extents_type::index-cast(i) is a multidimensional index in extents_.

#include <mdspan>
#include <cassert>
#include <cstdint>

#include "test_macros.h"

#include "../ConvertibleToIntegral.h"
#include "libcxx_tc_common.h"

template<class Mapping, class ... Indices>
concept operator_constraints = requires(Mapping m, Indices ... idxs) {
  {std::is_same_v<decltype(m(idxs...)), typename Mapping::index_type>};
};

template<class Mapping, class ... Indices>
  requires(
    operator_constraints<Mapping, Indices...>
  )
constexpr bool check_operator_constraints(Mapping m, Indices ... idxs) {
  (void) m(idxs...);
  return true;
}

template<class Mapping, class ... Indices>
constexpr bool check_operator_constraints(Mapping, Indices ...) {
  return false;
}

template <class M, class T, class... Args>
constexpr void iterate_left(M m, T& count, Args... args) {
  constexpr int r = static_cast<int>(M::extents_type::rank()) - 1 - static_cast<int>(sizeof...(Args));
  if constexpr (-1 == r) {
    ASSERT_NOEXCEPT(m(args...));
    TC_ASSERT_EXPR(count == m(args...));
    count++;
  } else {
    for (typename M::index_type i = 0; i < m.extents().extent(r); i++) {
      iterate_left(m, count, i, args...);
    }
  }
}

template <class E, class... Args>
constexpr void test_iteration(Args... args) {
  using M = std::layout_left::mapping<E>;
  M m(E(args...));

  typename E::index_type count = 0;
  iterate_left(m, count);
}

constexpr bool test() {
  constexpr size_t D = std::dynamic_extent;
  test_iteration<std::extents<int>>();
  test_iteration<std::extents<unsigned, D>>(1);
  test_iteration<std::extents<unsigned, D>>(7);
  test_iteration<std::extents<unsigned, 7>>();
  test_iteration<std::extents<unsigned, 7, 8>>();
  test_iteration<std::extents<char, D, D, D, D>>(1, 1, 1, 1);

  // Check operator constraint for number of arguments
  static_assert(check_operator_constraints(std::layout_left::mapping<std::extents<int, D>>(std::extents<int, D>(1)), 0));
  static_assert(!check_operator_constraints(std::layout_left::mapping<std::extents<int, D>>(std::extents<int, D>(1)), 0, 0));

  // Check operator constraint for convertibility of arguments to index_type
  static_assert(check_operator_constraints(std::layout_left::mapping<std::extents<int, D>>(std::extents<int, D>(1)), IntType(0)));
  static_assert(!check_operator_constraints(std::layout_left::mapping<std::extents<unsigned, D>>(std::extents<unsigned, D>(1)), IntType(0)));

  // Check operator constraint for no-throw-constructibility of index_type from arguments
  static_assert(!check_operator_constraints(std::layout_left::mapping<std::extents<unsigned char, D>>(std::extents<unsigned char, D>(1)), IntType(0)));

  return true;
}

constexpr bool test_large() {
  constexpr size_t D = std::dynamic_extent;
  test_iteration<std::extents<int64_t, D, 8, D, D>>(7, 9, 10);
  test_iteration<std::extents<int64_t, D, 8, 1, D>>(7, 10);
  return true;
}

int tc_containers_views_mdspan_layout_left_index_operator(void) {
  test();
  static_assert(test());

  // The large test iterates over ~10k loop indices.
  // With assertions enabled this triggered the maximum default limit
  // for steps in consteval expressions. Assertions roughly double the
  // total number of instructions, so this was already close to the maximum.
  test_large();
  return 0;
}
