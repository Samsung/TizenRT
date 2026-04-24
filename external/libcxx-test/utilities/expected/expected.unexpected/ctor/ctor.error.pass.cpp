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

// template<class Err = E>
//   constexpr explicit unexpected(Err&& e);
//
// Constraints:
// - is_same_v<remove_cvref_t<Err>, unexpected> is false; and
// - is_same_v<remove_cvref_t<Err>, in_place_t> is false; and
// - is_constructible_v<E, Err> is true.
//
// Effects: Direct-non-list-initializes unex with std::forward<Err>(e).
// Throws: Any exception thrown by the initialization of unex.

#include <cassert>
#include <concepts>
#include <expected>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

// Test Constraints:
static_assert(std::constructible_from<std::unexpected<int>, int>);

// is_same_v<remove_cvref_t<Err>, unexpected>
struct CstrFromUnexpected {
  CstrFromUnexpected(CstrFromUnexpected const&) = delete;
  CstrFromUnexpected(std::unexpected<CstrFromUnexpected> const&);
};
static_assert(!std::constructible_from<std::unexpected<CstrFromUnexpected>, std::unexpected<CstrFromUnexpected>>);

// is_same_v<remove_cvref_t<Err>, in_place_t>
struct CstrFromInplace {
  CstrFromInplace(std::in_place_t);
};
static_assert(!std::constructible_from<std::unexpected<CstrFromInplace>, std::in_place_t>);

// !is_constructible_v<E, Err>
struct Foo {};
static_assert(!std::constructible_from<std::unexpected<Foo>, int>);

// test explicit
static_assert(std::convertible_to<int, int>);
static_assert(!std::convertible_to<int, std::unexpected<int>>);

struct Error {
  int i;
  constexpr Error(int ii) : i(ii) {}
  constexpr Error(const Error& other) : i(other.i) {}
  constexpr Error(Error&& other) : i(other.i) { other.i = 0; }
  Error(std::initializer_list<Error>) { TC_ASSERT_EXPR(false); }
};

constexpr bool test() {
  // lvalue
  {
    Error e(5);
    std::unexpected<Error> unex(e);
    TC_ASSERT_EXPR(unex.error().i == 5);
    TC_ASSERT_EXPR(e.i == 5);
  }

  // rvalue
  {
    Error e(5);
    std::unexpected<Error> unex(std::move(e));
    TC_ASSERT_EXPR(unex.error().i == 5);
    TC_ASSERT_EXPR(e.i == 0);
  }

  // Direct-non-list-initializes: does not trigger initializer_list overload
  {
    Error e(5);
    [[maybe_unused]] std::unexpected<Error> unex(e);
  }

  // Test default template argument.
  // Without it, the template parameter cannot be deduced from an initializer list
  {
    struct Bar {
      int i;
      int j;
      constexpr Bar(int ii, int jj) : i(ii), j(jj) {}
    };
    std::unexpected<Bar> ue({5, 6});
    TC_ASSERT_EXPR(ue.error().i == 5);
    TC_ASSERT_EXPR(ue.error().j == 6);
  }

  return true;
}

void testException() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  struct Except {};

  struct Throwing {
    Throwing() = default;
    Throwing(const Throwing&) { throw Except{}; }
  };

  Throwing t;
#ifndef _LIBCPP_NO_EXCEPTIONS
  try {
    std::unexpected<Throwing> u(t);
    TC_ASSERT_EXPR(false);
  } catch (Except) {
  }
#endif // _LIBCPP_NO_EXCEPTIONS
#endif // TEST_HAS_NO_EXCEPTIONS
}

int tc_utilities_expected_expected_unexpected_ctor_ctor_error(void) {
  test();
  static_assert(test());
  testException();
  return 0;
}
