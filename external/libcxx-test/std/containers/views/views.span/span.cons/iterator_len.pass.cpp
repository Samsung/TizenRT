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

// <span>

// template <class It>
// constexpr explicit(Extent != dynamic_extent) span(It first, size_type count);
//  If Extent is not equal to dynamic_extent, then count shall be equal to Extent.
//


#include <span>
#include <cassert>
#include <iterator>
#include <type_traits>
#include "libcxx_tc_common.h"

template <std::size_t Extent>
constexpr void test_constructibility() {
  struct Other {};
  static_assert(std::is_constructible_v<std::span<int, Extent>, int*, std::size_t>);
  static_assert(!std::is_constructible_v<std::span<int, Extent>, const int*, std::size_t>);
  static_assert(std::is_constructible_v<std::span<const int, Extent>, int*, std::size_t>);
  static_assert(std::is_constructible_v<std::span<const int, Extent>, const int*, std::size_t>);
  static_assert(!std::is_constructible_v<std::span<int, Extent>, volatile int*, std::size_t>);
  static_assert(!std::is_constructible_v<std::span<int, Extent>, const volatile int*, std::size_t>);
  static_assert(!std::is_constructible_v<std::span<const int, Extent>, volatile int*, std::size_t>);
  static_assert(!std::is_constructible_v<std::span<const int, Extent>, const volatile int*, std::size_t>);
  static_assert(!std::is_constructible_v<std::span<volatile int, Extent>, const int*, std::size_t>);
  static_assert(!std::is_constructible_v<std::span<volatile int, Extent>, const volatile int*, std::size_t>);
  static_assert(
      !std::is_constructible_v<std::span<int, Extent>, double*, std::size_t>); // iterator type differs from span type
  static_assert(!std::is_constructible_v<std::span<int, Extent>, std::size_t, size_t>);
  static_assert(!std::is_constructible_v<std::span<int, Extent>, Other*, std::size_t>); // unrelated iterator type
}

template <class T>
constexpr bool test_ctor() {
  T val[2] = {};
  auto s1 = std::span<T>(val, 2);
  auto s2 = std::span<T, 2>(val, 2);
  TC_ASSERT_EXPR(s1.data() == std::data(val) && s1.size() == std::size(val));
  TC_ASSERT_EXPR(s2.data() == std::data(val) && s2.size() == std::size(val));
  return true;
}

constexpr bool test() {
  test_constructibility<std::dynamic_extent>();
  test_constructibility<3>();

  struct A {};
  test_ctor<int>();
  test_ctor<A>();

  return true;
}

int tc_containers_views_views_span_span_cons_iterator_len(void) {
  test();
  static_assert(test());

  return 0;
}
