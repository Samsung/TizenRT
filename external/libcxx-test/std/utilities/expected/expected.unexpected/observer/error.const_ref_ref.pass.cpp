//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// constexpr const E&& error() const && noexcept;

#include <cassert>
#include <concepts>
#include <expected>
#include <utility>
#include "libcxx_tc_common.h"

template <class T>
concept ErrorNoexcept =
    requires(const T&& t) {
      { std::move(t).error() } noexcept;
    };

static_assert(!ErrorNoexcept<int>);
static_assert(ErrorNoexcept<std::unexpected<int>>);

constexpr bool test() {
  const std::unexpected<int> unex(5);
  decltype(auto) i = std::move(unex).error();
  static_assert(std::same_as<decltype(i), const int&&>);
  TC_ASSERT_EXPR(i == 5);
  return true;
}

int tc_utilities_expected_expected_unexpected_observer_error_const_ref_ref(void) {
  test();
  static_assert(test());
  return 0;
}
