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
