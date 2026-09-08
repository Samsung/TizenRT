//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr unexpected(unexpected&&) = default;

#include <cassert>
#include <expected>
#include <utility>
#include "libcxx_tc_common.h"

struct Error {
  int i;
  constexpr Error(int ii) : i(ii) {}
  constexpr Error(Error&& other) : i(other.i) {other.i = 0;}
};

constexpr bool test() {
  std::unexpected<Error> unex(5);
  auto unex2 = std::move(unex);
  TC_ASSERT_EXPR(unex2.error().i == 5);
  TC_ASSERT_EXPR(unex.error().i == 0);
  return true;
}

int tc_utilities_expected_expected_unexpected_ctor_ctor_move(void) {
  test();
  static_assert(test());
  return 0;
}
