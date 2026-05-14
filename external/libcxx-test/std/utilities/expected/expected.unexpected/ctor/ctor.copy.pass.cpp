//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr unexpected(const unexpected&) = default;

#include <cassert>
#include <expected>
#include "libcxx_tc_common.h"

struct Error {
  int i;
  constexpr Error(int ii) : i(ii) {}
};

constexpr bool test() {
  const std::unexpected<Error> unex(5);
  auto unex2 = unex;
  TC_ASSERT_EXPR(unex2.error().i == 5);
  return true;
}

int tc_utilities_expected_expected_unexpected_ctor_ctor_copy(void) {
  test();
  static_assert(test());
  return 0;
}
