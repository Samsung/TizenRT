//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// struct identity;

#include <functional>

#include <cassert>
#include <concepts>
#include <utility>

#include "MoveOnly.h"
#include "libcxx_tc_common.h"

static_assert(std::semiregular<std::identity>);
static_assert(requires { typename std::identity::is_transparent; });

constexpr bool test() {
  std::identity id;
  int i = 42;
  TC_ASSERT_EXPR(id(i) == 42);
  TC_ASSERT_EXPR(id(std::move(i)) == 42);

  MoveOnly m1 = 2;
  MoveOnly m2 = id(std::move(m1));
  TC_ASSERT_EXPR(m2.get() == 2);

  TC_ASSERT_EXPR(&id(i) == &i);
  static_assert(&id(id) == &id);

  const std::identity idc;
  TC_ASSERT_EXPR(idc(1) == 1);
  TC_ASSERT_EXPR(std::move(id)(1) == 1);
  TC_ASSERT_EXPR(std::move(idc)(1) == 1);

  id = idc; // run-time checks assignment
  static_assert(std::is_same_v<decltype(id(i)), int&>);
  static_assert(std::is_same_v<decltype(id(std::declval<int&&>())), int&&>);
  static_assert(
      std::is_same_v<decltype(id(std::declval<int const&>())), int const&>);
  static_assert(
      std::is_same_v<decltype(id(std::declval<int const&&>())), int const&&>);
  static_assert(std::is_same_v<decltype(id(std::declval<int volatile&>())),
                               int volatile&>);
  static_assert(std::is_same_v<decltype(id(std::declval<int volatile&&>())),
                               int volatile&&>);
  static_assert(
      std::is_same_v<decltype(id(std::declval<int const volatile&>())),
                     int const volatile&>);
  static_assert(
      std::is_same_v<decltype(id(std::declval<int const volatile&&>())),
                     int const volatile&&>);

  struct S {
    constexpr S() = default;
    constexpr S(S&&) noexcept(false) {}
    constexpr S(S const&) noexcept(false) {}
  };
  S x;
  static_assert(noexcept(id(x)));
  static_assert(noexcept(id(S())));

  return true;
}

int tc_utilities_function_objects_func_identity_identity(void) {
  test();
  static_assert(test());

  return 0;
}
