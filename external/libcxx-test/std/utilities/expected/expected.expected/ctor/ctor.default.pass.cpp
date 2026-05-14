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

//
// constexpr expected();

// Constraints: is_default_constructible_v<T> is true.
//
// Effects: Value-initializes val.
// Postconditions: has_value() is true.
//
// Throws: Any exception thrown by the initialization of val.

#include <cassert>
#include <expected>
#include <type_traits>

#include "test_macros.h"
#include "../../types.h"
#include "libcxx_tc_common.h"

struct NoDedefaultCtor {
  NoDedefaultCtor() = delete;
};

// Test constraints
static_assert(std::is_default_constructible_v<std::expected<int, int>>);
static_assert(!std::is_default_constructible_v<std::expected<NoDedefaultCtor, int>>);

struct MyInt {
  int i;
  friend constexpr bool operator==(const MyInt&, const MyInt&) = default;
};

template <class T, class E>
constexpr void testDefaultCtor() {
  std::expected<T, E> e;
  TC_ASSERT_EXPR(e.has_value());
  TC_ASSERT_EXPR(e.value() == T());
}

template <class T>
constexpr void testTypes() {
  testDefaultCtor<T, bool>();
  testDefaultCtor<T, int>();
  testDefaultCtor<T, NoDedefaultCtor>();
}

constexpr bool test() {
  testTypes<int>();
  testTypes<MyInt>();
  testTypes<TailClobberer<0>>();
  return true;
}

void testException() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  struct Throwing {
    Throwing() { throw Except{}; };
  };

#ifndef _LIBCPP_NO_EXCEPTIONS
  try {
    std::expected<Throwing, int> u;
    TC_ASSERT_EXPR(false);
  } catch (Except) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
#endif // TEST_HAS_NO_EXCEPTIONS
}

int tc_utilities_expected_expected_expected_ctor_ctor_default(void) {
  test();
  static_assert(test());
  testException();
  return 0;
}
