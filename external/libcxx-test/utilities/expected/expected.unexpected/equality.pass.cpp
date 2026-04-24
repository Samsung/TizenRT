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

// template<class E2>
// friend constexpr bool operator==(const unexpected& x, const unexpected<E2>& y);
//
// Mandates: The expression x.error() == y.error() is well-formed and its result is convertible to bool.
//
// Returns: x.error() == y.error().

#include <cassert>
#include <concepts>
#include <expected>
#include <utility>
#include "libcxx_tc_common.h"

struct Error{
  int i;
  friend constexpr bool operator==(const Error&, const Error&) = default;
};

constexpr bool test() {
  std::unexpected<Error> unex1(Error{2});
  std::unexpected<Error> unex2(Error{3});
  std::unexpected<Error> unex3(Error{2});
  TC_ASSERT_EXPR(unex1 == unex3);
  TC_ASSERT_EXPR(unex1 != unex2);
  TC_ASSERT_EXPR(unex2 != unex3);
  return true;
}

int tc_utilities_expected_expected_unexpected_equality(void) {
  test();
  static_assert(test());
  return 0;
}
