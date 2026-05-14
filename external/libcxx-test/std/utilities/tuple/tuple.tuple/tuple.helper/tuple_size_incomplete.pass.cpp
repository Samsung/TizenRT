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
// <tuple>

// template <class... Types> class tuple;

// template <class... Types>
//   struct tuple_size<tuple<Types...>>
//     : public integral_constant<size_t, sizeof...(Types)> { };

// UNSUPPORTED: c++03

#include <tuple>
#include <array>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, std::size_t Size = sizeof(std::tuple_size<T>)>
constexpr bool is_complete(int) { static_assert(Size > 0, ""); return true; }
template <class> constexpr bool is_complete(long) { return false; }
template <class T> constexpr bool is_complete() { return is_complete<T>(0); }

struct Dummy1 {};
struct Dummy2 {};

namespace std {
template <> struct tuple_size<Dummy1> : public integral_constant<std::size_t, 0> {};
}

template <class T>
void test_complete() {
  static_assert(is_complete<T>(), "");
  static_assert(is_complete<const T>(), "");
  static_assert(is_complete<volatile T>(), "");
  static_assert(is_complete<const volatile T>(), "");
}

template <class T>
void test_incomplete() {
  static_assert(!is_complete<T>(), "");
  static_assert(!is_complete<const T>(), "");
  static_assert(!is_complete<volatile T>(), "");
  static_assert(!is_complete<const volatile T>(), "");
}


int tc_utilities_tuple_tuple_tuple_tuple_helper_tuple_size_incomplete(void) {
  test_complete<std::tuple<> >();
  test_complete<std::tuple<int&> >();
  test_complete<std::tuple<int&&, int&, void*>>();
  test_complete<std::pair<int, long> >();
  test_complete<std::array<int, 5> >();
  test_complete<Dummy1>();

  test_incomplete<void>();
  test_incomplete<int>();
  test_incomplete<std::tuple<int>&>();
  test_incomplete<Dummy2>();

  return 0;
}
