//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr const T& value() const &;
// constexpr T& value() &;
// constexpr T&& value() &&;
// constexpr const T&& value() const &&;

#include <cassert>
#include <concepts>
#include <expected>
#include <type_traits>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

constexpr bool test() {
  // non-const &
  {
    std::expected<int, int> e(5);
    decltype(auto) x = e.value();
    static_assert(std::same_as<decltype(x), int&>);
    TC_ASSERT_EXPR(&x == &(*e));
    TC_ASSERT_EXPR(x == 5);
  }

  // const &
  {
    const std::expected<int, int> e(5);
    decltype(auto) x = e.value();
    static_assert(std::same_as<decltype(x), const int&>);
    TC_ASSERT_EXPR(&x == &(*e));
    TC_ASSERT_EXPR(x == 5);
  }

  // non-const &&
  {
    std::expected<int, int> e(5);
    decltype(auto) x = std::move(e).value();
    static_assert(std::same_as<decltype(x), int&&>);
    TC_ASSERT_EXPR(&x == &(*e));
    TC_ASSERT_EXPR(x == 5);
  }

  // const &&
  {
    const std::expected<int, int> e(5);
    decltype(auto) x = std::move(e).value();
    static_assert(std::same_as<decltype(x), const int&&>);
    TC_ASSERT_EXPR(&x == &(*e));
    TC_ASSERT_EXPR(x == 5);
  }

  return true;
}

void testException() {
#ifndef TEST_HAS_NO_EXCEPTIONS

  // int
  {
    const std::expected<int, int> e(std::unexpect, 5);
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      (void) e.value();
      TC_ASSERT_EXPR(false);
    } catch (const std::bad_expected_access<int>& ex) {
      TC_ASSERT_EXPR(ex.error() == 5);
    }
#endif // _LIBCPP_NO_EXCEPTIONS
  }

#endif // TEST_HAS_NO_EXCEPTIONS
}

void testAsConst() {
#ifndef TEST_HAS_NO_EXCEPTIONS
  struct Error {
    enum { Default, MutableRefCalled, ConstRefCalled } From = Default;
    Error()                                                 = default;
    Error(const Error&) { From = ConstRefCalled; }
    Error(Error&) { From = MutableRefCalled; }
    Error(Error&& e) { From = e.From; }
  };

  // Test & overload
  {
    std::expected<int, Error> e(std::unexpect, Error());
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      (void)e.value();
      TC_ASSERT_EXPR(false);
    } catch (const std::bad_expected_access<Error>& ex) {
      TC_ASSERT_EXPR(ex.error().From == Error::ConstRefCalled);
    }
#endif // _LIBCPP_NO_EXCEPTIONS
  }

  // There are no effects for `const &` overload.

#endif // TEST_HAS_NO_EXCEPTIONS
}

int tc_utilities_expected_expected_expected_observers_value(void) {
  test();
  static_assert(test());
  testException();
  testAsConst();

  return 0;
}
