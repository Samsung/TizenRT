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

// XFAIL: availability-bad_variant_access-missing && !no-exceptions

// <variant>

// template <class ...Types> class variant;

// template <size_t I, class ...Args>
// constexpr explicit variant(in_place_index_t<I>, Args&&...);

#include <cassert>
#include <string>
#include <type_traits>
#include <variant>

#include "test_convertible.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

void test_ctor_sfinae() {
  {
    using V = std::variant<int>;
    static_assert(
        std::is_constructible<V, std::in_place_index_t<0>, int>::value, "");
    static_assert(!test_convertible<V, std::in_place_index_t<0>, int>(), "");
  }
  {
    using V = std::variant<int, long, long long>;
    static_assert(
        std::is_constructible<V, std::in_place_index_t<1>, int>::value, "");
    static_assert(!test_convertible<V, std::in_place_index_t<1>, int>(), "");
  }
  {
    using V = std::variant<int, long, int *>;
    static_assert(
        std::is_constructible<V, std::in_place_index_t<2>, int *>::value, "");
    static_assert(!test_convertible<V, std::in_place_index_t<2>, int *>(), "");
  }
  { // args not convertible to type
    using V = std::variant<int, long, int *>;
    static_assert(
        !std::is_constructible<V, std::in_place_index_t<0>, int *>::value, "");
    static_assert(!test_convertible<V, std::in_place_index_t<0>, int *>(), "");
  }
  { // index not in variant
    using V = std::variant<int, long, int *>;
    static_assert(
        !std::is_constructible<V, std::in_place_index_t<3>, int>::value, "");
    static_assert(!test_convertible<V, std::in_place_index_t<3>, int>(), "");
  }
}

void test_ctor_basic() {
  {
    constexpr std::variant<int> v(std::in_place_index<0>, 42);
    static_assert(v.index() == 0, "");
    static_assert(std::get<0>(v) == 42, "");
  }
  {
    constexpr std::variant<int, long, long> v(std::in_place_index<1>, 42);
    static_assert(v.index() == 1, "");
    static_assert(std::get<1>(v) == 42, "");
  }
  {
    constexpr std::variant<int, const int, long> v(std::in_place_index<1>, 42);
    static_assert(v.index() == 1, "");
    static_assert(std::get<1>(v) == 42, "");
  }
  {
    using V = std::variant<const int, volatile int, int>;
    int x = 42;
    V v(std::in_place_index<0>, x);
    TC_ASSERT_EXPR(v.index() == 0);
    TC_ASSERT_EXPR(std::get<0>(v) == x);
  }
  {
    using V = std::variant<const int, volatile int, int>;
    int x = 42;
    V v(std::in_place_index<1>, x);
    TC_ASSERT_EXPR(v.index() == 1);
    TC_ASSERT_EXPR(std::get<1>(v) == x);
  }
  {
    using V = std::variant<const int, volatile int, int>;
    int x = 42;
    V v(std::in_place_index<2>, x);
    TC_ASSERT_EXPR(v.index() == 2);
    TC_ASSERT_EXPR(std::get<2>(v) == x);
  }
}

int tc_utilities_variant_variant_variant_variant_ctor_in_place_index_args(void) {
  test_ctor_basic();
  test_ctor_sfinae();

  return 0;
}
