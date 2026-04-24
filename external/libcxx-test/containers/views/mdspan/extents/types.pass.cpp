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

// template<class IndexType, size_t... Extents>
// class extents {
// public:
//  // types
//  using index_type = IndexType;
//  using size_type = make_unsigned_t<index_type>;
//  using rank_type = size_t;
//
//  static constexpr rank_type rank() noexcept { return sizeof...(Extents); }
//  static constexpr rank_type rank_dynamic() noexcept { return dynamic-index(rank()); }
//  ...
//  }

#include <mdspan>
#include <type_traits>
#include <concepts>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class E, class IndexType, size_t... Extents>
void testExtents() {
  ASSERT_SAME_TYPE(typename E::index_type, IndexType);
  ASSERT_SAME_TYPE(typename E::size_type, std::make_unsigned_t<IndexType>);
  ASSERT_SAME_TYPE(typename E::rank_type, size_t);

  static_assert(sizeof...(Extents) == E::rank());
  static_assert((static_cast<size_t>(Extents == std::dynamic_extent) + ...) == E::rank_dynamic());

  static_assert(std::regular<E>);
  static_assert(std::is_trivially_copyable_v<E>);

// Did never find a way to make this true on windows
#ifndef _WIN32
  LIBCPP_STATIC_ASSERT(std::is_empty_v<E> == (E::rank_dynamic() == 0));
#endif
}

template <class IndexType, size_t... Extents>
void testExtents() {
  testExtents<std::extents<IndexType, Extents...>, IndexType, Extents...>();
}

template <class T>
void test() {
  constexpr size_t D = std::dynamic_extent;
  testExtents<T, D>();
  testExtents<T, 3>();
  testExtents<T, 3, 3>();
  testExtents<T, 3, D>();
  testExtents<T, D, 3>();
  testExtents<T, D, D>();
  testExtents<T, 3, 3, 3>();
  testExtents<T, 3, 3, D>();
  testExtents<T, 3, D, D>();
  testExtents<T, D, 3, D>();
  testExtents<T, D, D, D>();
  testExtents<T, 3, D, 3>();
  testExtents<T, D, 3, 3>();
  testExtents<T, D, D, 3>();

  testExtents<T, 9, 8, 7, 6, 5, 4, 3, 2, 1>();
  testExtents<T, 9, D, 7, 6, D, D, 3, D, D>();
  testExtents<T, D, D, D, D, D, D, D, D, D>();
}

int tc_containers_views_mdspan_extents_types(void) {
  test<int>();
  test<unsigned>();
  test<char>();
  test<long long>();
  test<size_t>();
  return 0;
}
