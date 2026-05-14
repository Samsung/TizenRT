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

// template<class... Args>
//   constexpr explicit expected(unexpect_t, Args&&... args);
//
// Constraints: is_constructible_v<E, Args...> is true.
//
// Effects: Direct-non-list-initializes unex with std::forward<Args>(args)....
//
// Postconditions: has_value() is false.
//
// Throws: Any exception thrown by the initialization of unex.

#include <cassert>
#include <expected>
#include <tuple>
#include <type_traits>
#include <utility>

#include "MoveOnly.h"
#include "test_macros.h"
#include "../../types.h"
#include "libcxx_tc_common.h"

// Test Constraints:
static_assert(std::is_constructible_v<std::expected<int, int>, std::unexpect_t>);
static_assert(std::is_constructible_v<std::expected<int, int>, std::unexpect_t, int>);

// !is_constructible_v<T, Args...>
struct foo {};
static_assert(!std::is_constructible_v<std::expected<int, foo>, std::unexpect_t, int>);

// test explicit
template <class T>
void conversion_test(T);

template <class T, class... Args>
concept ImplicitlyConstructible = requires(Args&&... args) { conversion_test<T>({std::forward<Args>(args)...}); };
static_assert(ImplicitlyConstructible<int, int>);

static_assert(!ImplicitlyConstructible<std::expected<int, int>, std::unexpect_t>);
static_assert(!ImplicitlyConstructible<std::expected<int, int>, std::unexpect_t, int>);

struct CopyOnly {
  int i;
  constexpr CopyOnly(int ii) : i(ii) {}
  CopyOnly(const CopyOnly&) = default;
  CopyOnly(CopyOnly&&)      = delete;
  friend constexpr bool operator==(const CopyOnly& mi, int ii) { return mi.i == ii; }
};

template <class T, class V = int>
constexpr void testInt() {
  std::expected<V, T> e(std::unexpect, 5);
  TC_ASSERT_EXPR(!e.has_value());
  TC_ASSERT_EXPR(e.error() == 5);
}

template <class T, class V = int>
constexpr void testLValue() {
  T t(5);
  std::expected<V, T> e(std::unexpect, t);
  TC_ASSERT_EXPR(!e.has_value());
  TC_ASSERT_EXPR(e.error() == 5);
}

template <class T, class V = int>
constexpr void testRValue() {
  std::expected<V, T> e(std::unexpect, T(5));
  TC_ASSERT_EXPR(!e.has_value());
  TC_ASSERT_EXPR(e.error() == 5);
}

constexpr bool test() {
  testInt<int>();
  testInt<CopyOnly>();
  testInt<MoveOnly>();
  testInt<TailClobberer<1>, bool>();
  testLValue<int>();
  testLValue<CopyOnly>();
  testLValue<TailClobberer<1>, bool>();
  testRValue<int>();
  testRValue<MoveOnly>();
  testRValue<TailClobberer<1>, bool>();

  // no arg
  {
    std::expected<int, int> e(std::unexpect);
    TC_ASSERT_EXPR(!e.has_value());
    TC_ASSERT_EXPR(e.error() == 0);
  }

  // one arg
  {
    std::expected<int, int> e(std::unexpect, 5);
    TC_ASSERT_EXPR(!e.has_value());
    TC_ASSERT_EXPR(e.error() == 5);
  }

  // multi args
  {
    std::expected<int, std::tuple<int, short, MoveOnly>> e(std::unexpect, 1, short{2}, MoveOnly(3));
    TC_ASSERT_EXPR(!e.has_value());
    TC_ASSERT_EXPR((e.error() == std::tuple<int, short, MoveOnly>(1, short{2}, MoveOnly(3))));
  }

  return true;
}

void testException() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  struct Throwing {
    Throwing(int) { throw Except{}; };
  };

#ifndef _LIBCPP_NO_EXCEPTIONS
  try {
    std::expected<int, Throwing> u(std::unexpect, 5);
    TC_ASSERT_EXPR(false);
  } catch (Except) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
#endif // TEST_HAS_NO_EXCEPTIONS
}

int tc_utilities_expected_expected_expected_ctor_ctor_unexpect(void) {
  test();
  static_assert(test());
  testException();
  return 0;
}
