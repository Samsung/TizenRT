//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// [utility.underlying], to_underlying
// template <class T>
//     constexpr underlying_type_t<T> to_underlying( T value ) noexcept; // C++23

#include <utility>
#include <cassert>
#include <cstdint>
#include <limits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

enum class e_default { a = 0, b = 1, c = 2 };
enum class e_ushort : unsigned short { d = 10, e = 25, f = 50 };
enum class e_longlong : long long {
  low = std::numeric_limits<long long>::min(),
  high = std::numeric_limits<long long>::max()
};
enum e_non_class { enum_a = 10, enum_b = 11, enum_c = 12 };
enum e_int : int {
  enum_min = std::numeric_limits<int>::min(),
  enum_max = std::numeric_limits<int>::max()
};
enum class e_bool : std::uint8_t { f = 0, t = 1 };

struct WithBitfieldEnums {
  e_default e1 : 3;
  e_ushort e2 : 6;
  e_bool e3 : 1;
};

constexpr bool test() {
  ASSERT_NOEXCEPT(std::to_underlying(e_default::a));
  ASSERT_SAME_TYPE(int, decltype(std::to_underlying(e_default::a)));
  ASSERT_SAME_TYPE(unsigned short, decltype(std::to_underlying(e_ushort::d)));
  ASSERT_SAME_TYPE(long long, decltype(std::to_underlying(e_longlong::low)));
  ASSERT_SAME_TYPE(int, decltype(std::to_underlying(enum_min)));
  ASSERT_SAME_TYPE(int, decltype(std::to_underlying(enum_max)));

  TC_ASSERT_EXPR(0 == std::to_underlying(e_default::a));
  TC_ASSERT_EXPR(1 == std::to_underlying(e_default::b));
  TC_ASSERT_EXPR(2 == std::to_underlying(e_default::c));

  TC_ASSERT_EXPR(10 == std::to_underlying(e_ushort::d));
  TC_ASSERT_EXPR(25 == std::to_underlying(e_ushort::e));
  TC_ASSERT_EXPR(50 == std::to_underlying(e_ushort::f));

  // Check no truncating.
  TC_ASSERT_EXPR(std::numeric_limits<long long>::min() ==
         std::to_underlying(e_longlong::low));
  TC_ASSERT_EXPR(std::numeric_limits<long long>::max() ==
         std::to_underlying(e_longlong::high));

  TC_ASSERT_EXPR(10 == std::to_underlying(enum_a));
  TC_ASSERT_EXPR(11 == std::to_underlying(enum_b));
  TC_ASSERT_EXPR(12 == std::to_underlying(enum_c));
  TC_ASSERT_EXPR(std::numeric_limits<int>::min() == std::to_underlying(enum_min));
  TC_ASSERT_EXPR(std::numeric_limits<int>::max() == std::to_underlying(enum_max));

  WithBitfieldEnums bf;
  bf.e1 = static_cast<e_default>(3);
  bf.e2 = e_ushort::e;
  bf.e3 = e_bool::t;
  TC_ASSERT_EXPR(3 == std::to_underlying(bf.e1));
  TC_ASSERT_EXPR(25 == std::to_underlying(bf.e2));
  TC_ASSERT_EXPR(1 == std::to_underlying(bf.e3));

  return true;
}

int tc_utilities_utility_utility_underlying_to_underlying(void) {
  test();
  static_assert(test());

  return 0;
}
