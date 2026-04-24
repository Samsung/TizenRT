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

// template<class R, class T>
//   constexpr bool in_range(T t) noexcept;               // C++20

#include <utility>
#include <limits>
#include <numeric>
#include <tuple>
#include <cassert>
#include <cstdint>

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
constexpr void test_in_range1() {
  constexpr Tuple<T> tup;
  TC_ASSERT_EXPR(std::in_range<T>(tup.min));
  TC_ASSERT_EXPR(std::in_range<T>(tup.min + 1));
  TC_ASSERT_EXPR(std::in_range<T>(tup.max));
  TC_ASSERT_EXPR(std::in_range<T>(tup.max - 1));
  TC_ASSERT_EXPR(std::in_range<T>(tup.mid));
  TC_ASSERT_EXPR(std::in_range<T>(tup.mid - 1));
  TC_ASSERT_EXPR(std::in_range<T>(tup.mid + 1));
}

constexpr void test_in_range() {
  constexpr Tuple<std::uint8_t> utup8;
  constexpr Tuple<std::int8_t> stup8;
  TC_ASSERT_EXPR(!std::in_range<std::int8_t>(utup8.max));
  TC_ASSERT_EXPR(std::in_range<short>(utup8.max));
  TC_ASSERT_EXPR(!std::in_range<std::uint8_t>(stup8.min));
  TC_ASSERT_EXPR(std::in_range<std::int8_t>(utup8.mid));
  TC_ASSERT_EXPR(!std::in_range<std::uint8_t>(stup8.mid));
  TC_ASSERT_EXPR(!std::in_range<std::uint8_t>(-1));
}

template <class... Ts>
constexpr void test1(const std::tuple<Ts...>&) {
  (test_in_range1<Ts>() , ...);
}

constexpr bool test() {
  std::tuple<
#ifndef TEST_HAS_NO_INT128
      __int128_t, __uint128_t,
#endif
      unsigned long long, long long, unsigned long, long, unsigned int, int,
      unsigned short, short, unsigned char, signed char> types;
  test1(types);
  test_in_range();
  return true;
}

int tc_utilities_utility_utility_intcmp_intcmp_in_range_in_range(void) {
  ASSERT_NOEXCEPT(std::in_range<int>(-1));
  test();
  static_assert(test());
  return 0;
}
