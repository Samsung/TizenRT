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

// template<class U, class G>
//   constexpr explicit(!is_convertible_v<const G&, E>) expected(const expected<U, G>& rhs);
//
// Let GF be const G&
//
// Constraints:
// - is_void_v<U> is true; and
// - is_constructible_v<E, GF> is true; and
// - is_constructible_v<unexpected<E>, expected<U, G>&> is false; and
// - is_constructible_v<unexpected<E>, expected<U, G>> is false; and
// - is_constructible_v<unexpected<E>, const expected<U, G>&> is false; and
// - is_constructible_v<unexpected<E>, const expected<U, G>> is false.
//
// Effects: If rhs.has_value() is false, direct-non-list-initializes unex with std::forward<GF>(rhs.error()).
//
// Postconditions: rhs.has_value() is unchanged; rhs.has_value() == this->has_value() is true.
//
// Throws: Any exception thrown by the initialization of unex.

#include <cassert>
#include <concepts>
#include <expected>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "../../types.h"
#include "libcxx_tc_common.h"

// Test Constraints:
template <class T1, class Err1, class T2, class Err2>
concept canCstrFromExpected = std::is_constructible_v<std::expected<T1, Err1>, const std::expected<T2, Err2>&>;

struct CtorFromInt {
  CtorFromInt(int);
};

static_assert(canCstrFromExpected<void, CtorFromInt, void, int>);

struct NoCtorFromInt {};

// !is_void_v<E>
static_assert(!canCstrFromExpected<void, int, int, int>);

// !is_constructible_v<E, GF>
static_assert(!canCstrFromExpected<void, NoCtorFromInt, void, int>);

template <class T>
struct CtorFrom {
  explicit CtorFrom(int)
    requires(!std::same_as<T, int>);
  explicit CtorFrom(T);
  explicit CtorFrom(auto&&) = delete;
};

// Note for below 4 tests, because their E is constructible from cvref of std::expected<void, int>,
// unexpected<E> will be constructible from cvref of std::expected<void, int>
// is_constructible_v<unexpected<E>, expected<U, G>&>
static_assert(!canCstrFromExpected<void, CtorFrom<std::expected<void, int>&>, void, int>);

// is_constructible_v<unexpected<E>, expected<U, G>>
static_assert(!canCstrFromExpected<void, CtorFrom<std::expected<void, int>&&>, void, int>);

// is_constructible_v<unexpected<E>, const expected<U, G>&> is false
static_assert(!canCstrFromExpected<void, CtorFrom<std::expected<void, int> const&>, void, int>);

// is_constructible_v<unexpected<E>, const expected<U, G>>
static_assert(!canCstrFromExpected<void, CtorFrom<std::expected<void, int> const&&>, void, int>);

// test explicit
static_assert(std::is_convertible_v<const std::expected<void, int>&, std::expected<void, long>>);

// !is_convertible_v<GF, E>.
static_assert(std::is_constructible_v<std::expected<void, CtorFrom<int>>, const std::expected<void, int>&>);
static_assert(!std::is_convertible_v<const std::expected<void, int>&, std::expected<void, CtorFrom<int>>>);

struct Data {
  int i;
  constexpr Data(int ii) : i(ii) {}
};

constexpr bool test() {
  // convert the error
  {
    const std::expected<void, int> e1(std::unexpect, 5);
    std::expected<void, Data> e2 = e1;
    TC_ASSERT_EXPR(!e2.has_value());
    TC_ASSERT_EXPR(e2.error().i == 5);
    TC_ASSERT_EXPR(!e1.has_value());
    TC_ASSERT_EXPR(e1.error() == 5);
  }

  // convert TailClobberer
  {
    const std::expected<void, TailClobbererNonTrivialMove<1>> e1(std::unexpect);
    std::expected<void, TailClobberer<1>> e2 = e1;
    TC_ASSERT_EXPR(!e2.has_value());
    TC_ASSERT_EXPR(!e1.has_value());
  }

  return true;
}

void testException() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  struct ThrowingInt {
    ThrowingInt(int) { throw Except{}; }
  };

  // throw on converting error
  {
    const std::expected<void, int> e1(std::unexpect);
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      [[maybe_unused]] std::expected<void, ThrowingInt> e2 = e1;
      TC_ASSERT_EXPR(false);
    } catch (Except) {
    }
#endif // _LIBCPP_NO_EXCEPTIONS
  }

#endif // TEST_HAS_NO_EXCEPTIONS
}

int tc_utilities_expected_expected_void_ctor_ctor_convert_copy(void) {
  test();
  static_assert(test());
  testException();
  return 0;
}
