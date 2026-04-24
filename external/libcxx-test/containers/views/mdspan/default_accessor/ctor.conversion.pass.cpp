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
//
// Test converting constructor:
//
// template<class OtherElementType>
//   constexpr default_accessor(default_accessor<OtherElementType>) noexcept {}
//
// Constraints: is_convertible_v<OtherElementType(*)[], element_type(*)[]> is true.

#include <mdspan>
#include <cassert>
#include <cstdint>
#include <type_traits>

#include "test_macros.h"

#include "../MinimalElementType.h"
#include "libcxx_tc_common.h"

struct Base {};
struct Derived: public Base {};

template <class FromT, class ToT>
constexpr void test_conversion() {
  std::default_accessor<FromT> acc_from;
  ASSERT_NOEXCEPT(std::default_accessor<ToT>(acc_from));
  [[maybe_unused]] std::default_accessor<ToT> acc_to(acc_from);
}

constexpr bool test() {
  // default accessor conversion largely behaves like pointer conversion
  test_conversion<int, int>();
  test_conversion<int, const int>();
  test_conversion<const int, const int>();
  test_conversion<MinimalElementType, MinimalElementType>();
  test_conversion<MinimalElementType, const MinimalElementType>();
  test_conversion<const MinimalElementType, const MinimalElementType>();

  // char is convertible to int, but accessors are not
  static_assert(!std::is_constructible_v<std::default_accessor<int>, std::default_accessor<char>>);
  // don't allow conversion from const elements to non-const
  static_assert(!std::is_constructible_v<std::default_accessor<int>, std::default_accessor<const int>>);
  // MinimalElementType is constructible from int, but accessors should not be convertible
  static_assert(!std::is_constructible_v<std::default_accessor<MinimalElementType>, std::default_accessor<int>>);
  // don't allow conversion from const elements to non-const
  static_assert(!std::is_constructible_v<std::default_accessor<MinimalElementType>, std::default_accessor<const MinimalElementType>>);
  // don't allow conversion from Base to Derived
  static_assert(!std::is_constructible_v<std::default_accessor<Derived>, std::default_accessor<Base>>);
  // don't allow conversion from Derived to Base
  static_assert(!std::is_constructible_v<std::default_accessor<Base>, std::default_accessor<Derived>>);

  return true;
}

int tc_containers_views_mdspan_default_accessor_ctor_conversion(void) {
  test();
  static_assert(test());
  return 0;
}
