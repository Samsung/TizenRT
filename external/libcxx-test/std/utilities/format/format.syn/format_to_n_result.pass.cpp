//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <format>

// struct format_to_n_result

// [format.syn]/1
// The class template format_to_n_result has the template parameters, data
// members, and special members specified above. It has no base classes or
// members other than those specified.

#include <format>

#include <cassert>
#include <concepts>
#include <iterator>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class CharT>
constexpr void test() {
  std::format_to_n_result<CharT*> v{nullptr, std::iter_difference_t<CharT*>{42}};

  auto [out, size] = v;
  static_assert(std::same_as<decltype(out), CharT*>);
  TC_ASSERT_EXPR(out == v.out);
  static_assert(std::same_as<decltype(size), std::iter_difference_t<CharT*>>);
  TC_ASSERT_EXPR(size == v.size);
}

constexpr bool test() {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif
  return true;
}

int tc_utilities_format_format_syn_format_to_n_result(void) {
  TC_ASSERT_EXPR(test());
  static_assert(test());

  return 0;
}
