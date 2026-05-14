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
//   constexpr explicit unexpected(in_place_t, Args&&... args);
//
// Constraints: is_constructible_v<E, Args...> is true.
//
// Effects: Direct-non-list-initializes unex with std::forward<Args>(args)....
//
// Throws: Any exception thrown by the initialization of unex.

#include <cassert>
#include <concepts>
#include <expected>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

// Test Constraints:
static_assert(std::constructible_from<std::unexpected<int>, std::in_place_t, int>);

// !is_constructible_v<E, Args...>
struct Foo {};
static_assert(!std::constructible_from<std::unexpected<Foo>, std::in_place_t, int>);

// test explicit
template <class T>
void conversion_test(T);

template <class T, class... Args>
concept ImplicitlyConstructible = requires(Args&&... args) { conversion_test<T>({std::forward<Args>(args)...}); };

static_assert(ImplicitlyConstructible<int, int>);
static_assert(!ImplicitlyConstructible<std::unexpected<int>, std::in_place_t, int>);

struct Arg {
  int i;
  constexpr Arg(int ii) : i(ii) {}
  constexpr Arg(const Arg& other) : i(other.i) {}
  constexpr Arg(Arg&& other) : i(other.i) { other.i = 0; }
};

struct Error {
  Arg arg;
  constexpr explicit Error(const Arg& a) : arg(a) {}
  constexpr explicit Error(Arg&& a) : arg(std::move(a)) {}
  Error(std::initializer_list<Error>) :arg(0){ TC_ASSERT_EXPR(false); }
};

constexpr bool test() {
  // lvalue
  {
    Arg a{5};
    std::unexpected<Error> unex(std::in_place, a);
    TC_ASSERT_EXPR(unex.error().arg.i == 5);
    TC_ASSERT_EXPR(a.i == 5);
  }

  // rvalue
  {
    Arg a{5};
    std::unexpected<Error> unex(std::in_place, std::move(a));
    TC_ASSERT_EXPR(unex.error().arg.i == 5);
    TC_ASSERT_EXPR(a.i == 0);
  }

  // Direct-non-list-initializes: does not trigger initializer_list overload
  {
    Error e(5);
    [[maybe_unused]] std::unexpected<Error> unex(std::in_place, e);
  }
  return true;
}

void testException() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  struct Except {};

  struct Throwing {
    Throwing(int) { throw Except{}; }
  };

#ifndef _LIBCPP_NO_EXCEPTIONS
  try {
    std::unexpected<Throwing> u(std::in_place, 5);
    TC_ASSERT_EXPR(false);
  } catch (Except) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
#endif // TEST_HAS_NO_EXCEPTIONS
}

int tc_utilities_expected_expected_unexpected_ctor_ctor_inplace(void) {
  test();
  static_assert(test());
  testException();
  return 0;
}
