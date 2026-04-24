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
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <array>

// template<class T, size_t N>
//   constexpr synth-three-way-result<T>
//     operator<=>(const array<T, N>& x, const array<T, N>& y);

#include <array>
#include <cassert>

#include "test_comparisons.h"
#include "libcxx_tc_common.h"

// SFINAE

constexpr std::size_t N{1};

// The container should fulfill `std::three_way_comparable`
static_assert(std::three_way_comparable<std::array<int, N>>);

// Thanks to SFINAE, the following is not a compiler error but returns `false`
struct NonComparable {};
static_assert(!std::three_way_comparable<std::array<NonComparable, N>>);

// Implementation detail of `test_sequence_container_array_spaceship`
template <typename Elem, typename Order>
constexpr void test_sequence_container_array_spaceship_with_type() {
  // Empty containers
  {
    std::array<Elem, 0> l1 = {};
    std::array<Elem, 0> l2 = {};
    TC_ASSERT_EXPR(testOrder(l1, l2, Order::equivalent));
  }
  // Identical contents
  {
    std::array l1{Elem{1}, Elem{1}};
    std::array l2{Elem{1}, Elem{1}};
    TC_ASSERT_EXPR(testOrder(l1, l2, Order::equivalent));
  }
  // Less, due to contained values
  {
    std::array l1{Elem{1}, Elem{1}};
    std::array l2{Elem{1}, Elem{2}};
    TC_ASSERT_EXPR(testOrder(l1, l2, Order::less));
  }
  // Greater, due to contained values
  {
    std::array l1{Elem{1}, Elem{3}};
    std::array l2{Elem{1}, Elem{2}};
    TC_ASSERT_EXPR(testOrder(l1, l2, Order::greater));
  }
  // Shorter list - unsupported - containers must be of equal lengths
  // Longer list - unsupported - containers must be of equal lengths
  // Unordered
  if constexpr (std::is_same_v<Elem, PartialOrder>) {
    std::array l1{Elem{1}, Elem{std::numeric_limits<int>::min()}};
    std::array l2{Elem{1}, Elem{2}};
    TC_ASSERT_EXPR(testOrder(l1, l2, Order::unordered));
  }
}

// Tests the `operator<=>` on sequence containers `array`
constexpr bool test_sequence_container_array_spaceship() {
  // Test different comparison categories
  test_sequence_container_array_spaceship_with_type<int, std::strong_ordering>();
  test_sequence_container_array_spaceship_with_type<StrongOrder, std::strong_ordering>();
  test_sequence_container_array_spaceship_with_type<WeakOrder, std::weak_ordering>();
  test_sequence_container_array_spaceship_with_type<PartialOrder, std::partial_ordering>();

  // `LessAndEqComp` does not have `operator<=>`. Ordering is synthesized based on `operator<`
  test_sequence_container_array_spaceship_with_type<LessAndEqComp, std::weak_ordering>();

  return true;
}

int tc_containers_sequences_array_compare_three_way(void) {
  TC_ASSERT_EXPR(test_sequence_container_array_spaceship());
  static_assert(test_sequence_container_array_spaceship());
  return 0;
}
