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

// constexpr data_handle_type offset(data_handle_type p, size_t i) const noexcept;
//
// Effects: Equivalent to: return p+i;

#include <mdspan>
#include <cassert>
#include <type_traits>

#include "test_macros.h"

#include "../MinimalElementType.h"
#include "libcxx_tc_common.h"

template <class T>
constexpr void test_offset() {
  ElementPool<std::remove_const_t<T>, 10> data;
  T* ptr = data.get_ptr();
  std::default_accessor<T> acc;
  for(int i = 0; i < 10; i++) {
    static_assert(std::is_same_v<decltype(acc.offset(ptr, i)), typename std::default_accessor<T>::data_handle_type>);
    ASSERT_NOEXCEPT(acc.offset(ptr, i));
    TC_ASSERT_EXPR(acc.offset(ptr, i) == ptr + i);
  }
}

constexpr bool test() {
  test_offset<int>();
  test_offset<const int>();
  test_offset<MinimalElementType>();
  test_offset<const MinimalElementType>();
  return true;
}

int tc_containers_views_mdspan_default_accessor_offset(void) {
  test();
  static_assert(test());
  return 0;
}
