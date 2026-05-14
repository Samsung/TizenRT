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

// Test default construction:
//
// constexpr default_accessor() noexcept = default;

#include <mdspan>
#include <cassert>
#include <cstdint>
#include <type_traits>

#include "test_macros.h"

#include "../MinimalElementType.h"
#include "libcxx_tc_common.h"

template <class T>
constexpr void test_construction() {
  ASSERT_NOEXCEPT(std::default_accessor<T>{});
  [[maybe_unused]] std::default_accessor<T> acc;
  static_assert(std::is_trivially_default_constructible_v<std::default_accessor<T>>);
}

constexpr bool test() {
  test_construction<int>();
  test_construction<const int>();
  test_construction<MinimalElementType>();
  test_construction<const MinimalElementType>();
  return true;
}

int tc_containers_views_mdspan_default_accessor_ctor_default(void) {
  test();
  static_assert(test());
  return 0;
}
