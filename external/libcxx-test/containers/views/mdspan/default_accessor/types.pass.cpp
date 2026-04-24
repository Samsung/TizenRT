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

//  template<class ElementType>
//  struct default_accessor {
//    using offset_policy = default_accessor;
//    using element_type = ElementType;
//    using reference = ElementType&;
//    using data_handle_type = ElementType*;
//    ...
//  };
//
//  Each specialization of default_accessor is a trivially copyable type that models semiregular.


#include <mdspan>
#include <type_traits>
#include <concepts>
#include <cassert>

#include "test_macros.h"

#include "../MinimalElementType.h"
#include "libcxx_tc_common.h"

template <class T>
void test() {
  using A = std::default_accessor<T>;
  ASSERT_SAME_TYPE(typename A::offset_policy, A);
  ASSERT_SAME_TYPE(typename A::element_type, T);
  ASSERT_SAME_TYPE(typename A::reference, T&);
  ASSERT_SAME_TYPE(typename A::data_handle_type, T*);

  static_assert(std::semiregular<A>);
  static_assert(std::is_trivially_copyable_v<A>);

  LIBCPP_STATIC_ASSERT(std::is_empty_v<A>);
}

int tc_containers_views_mdspan_default_accessor_types(void) {
  test<int>();
  test<const int>();
  test<MinimalElementType>();
  test<const MinimalElementType>();
  return 0;
}
