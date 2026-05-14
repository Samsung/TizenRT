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

// <utility>

// template<class T, class U>
//   constexpr bool cmp_not_equal(T t, U u) noexcept;     // C++20

#include <utility>
#include <limits>
#include <numeric>
#include <tuple>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <typename T>
struct Tuple {
  T min;
  T max;
  T mid;
  constexpr Tuple() {
    min = std::numeric_limits<T>::min();
    max = std::numeric_limits<T>::max();
    mid = std::midpoint(min, max);
  }
};

template <typename T>
constexpr void test_cmp_not_equal1() {
  constexpr Tuple<T> tup;
  TC_ASSERT_EXPR(!std::cmp_not_equal(T(0), T(0)));
  TC_ASSERT_EXPR(!std::cmp_not_equal(T(10), T(10)));
  TC_ASSERT_EXPR(!std::cmp_not_equal(tup.min, tup.min));
  TC_ASSERT_EXPR(!std::cmp_not_equal(tup.max, tup.max));
  TC_ASSERT_EXPR(std::cmp_not_equal(T(0), T(1)));
  TC_ASSERT_EXPR(std::cmp_not_equal(T(1), T(0)));
  TC_ASSERT_EXPR(std::cmp_not_equal(T(5), T(10)));
  TC_ASSERT_EXPR(std::cmp_not_equal(T(10), T(5)));
  TC_ASSERT_EXPR(std::cmp_not_equal(tup.min, tup.max));
  TC_ASSERT_EXPR(std::cmp_not_equal(tup.max, tup.min));
  TC_ASSERT_EXPR(std::cmp_not_equal(1, tup.max));
  TC_ASSERT_EXPR(std::cmp_not_equal(tup.max, 1));
  TC_ASSERT_EXPR(std::cmp_not_equal(1, tup.min));
  TC_ASSERT_EXPR(std::cmp_not_equal(tup.min, 1));
  TC_ASSERT_EXPR(std::cmp_not_equal(-2, tup.min));
  TC_ASSERT_EXPR(std::cmp_not_equal(tup.min, -2));
  TC_ASSERT_EXPR(std::cmp_not_equal(-2, tup.max));
  TC_ASSERT_EXPR(std::cmp_not_equal(tup.max, -2));
}

template <typename T, typename U>
constexpr void test_cmp_not_equal2() {
  constexpr Tuple<T> ttup;
  constexpr Tuple<U> utup;
  TC_ASSERT_EXPR(!std::cmp_not_equal(T(0), U(0)));
  TC_ASSERT_EXPR(!std::cmp_not_equal(T(10), U(10)));
  TC_ASSERT_EXPR(std::cmp_not_equal(T(0), U(1)));
  TC_ASSERT_EXPR(std::cmp_not_equal(T(1), U(0)));
  TC_ASSERT_EXPR(std::cmp_not_equal(T(5), U(10)));
  TC_ASSERT_EXPR(std::cmp_not_equal(T(10), U(5)));
  TC_ASSERT_EXPR(std::cmp_not_equal(ttup.min, utup.max));
  TC_ASSERT_EXPR(std::cmp_not_equal(utup.min, ttup.max));
}

template <class... Ts>
constexpr void test1(const std::tuple<Ts...>&) {
  (test_cmp_not_equal1<Ts>() , ...);
}

template <class T, class... Us>
constexpr void test2_impl(const std::tuple<Us...>&) {
  (test_cmp_not_equal2<T, Us>() , ...);
}

template <class... Ts, class UTuple>
constexpr void test2(const std::tuple<Ts...>&, const UTuple& utuple) {
  (test2_impl<Ts>(utuple) , ...);
}

constexpr bool test() {
  std::tuple<
#ifndef TEST_HAS_NO_INT128
      __int128_t, __uint128_t,
#endif
      unsigned long long, long long, unsigned long, long, unsigned int, int,
      unsigned short, short, unsigned char, signed char> types;
  test1(types);
  test2(types, types);
  return true;
}

int tc_utilities_utility_utility_intcmp_intcmp_cmp_not_equal_cmp_not_equal(void) {
  ASSERT_NOEXCEPT(std::cmp_not_equal(0, 0));
  test();
  static_assert(test());
  return 0;
}
