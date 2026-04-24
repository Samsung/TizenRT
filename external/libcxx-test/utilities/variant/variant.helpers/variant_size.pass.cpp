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
// UNSUPPORTED: c++03, c++11, c++14

// <variant>

// template <class T> struct variant_size; // undefined
// template <class T> struct variant_size<const T>;
// template <class T> struct variant_size<volatile T>;
// template <class T> struct variant_size<const volatile T>;
// template <class T> constexpr size_t variant_size_v
//     = variant_size<T>::value;

#include <memory>
#include <type_traits>
#include <variant>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class V, std::size_t E> void test() {
  static_assert(std::variant_size<V>::value == E, "");
  static_assert(std::variant_size<const V>::value == E, "");
  static_assert(std::variant_size<volatile V>::value == E, "");
  static_assert(std::variant_size<const volatile V>::value == E, "");
  static_assert(std::variant_size_v<V> == E, "");
  static_assert(std::variant_size_v<const V> == E, "");
  static_assert(std::variant_size_v<volatile V> == E, "");
  static_assert(std::variant_size_v<const volatile V> == E, "");
  static_assert(std::is_base_of<std::integral_constant<std::size_t, E>,
                                std::variant_size<V>>::value,
                "");
};

int tc_utilities_variant_variant_helpers_variant_size(void) {
  test<std::variant<>, 0>();
  test<std::variant<void *>, 1>();
  test<std::variant<long, long, void *, double>, 4>();

  return 0;
}
