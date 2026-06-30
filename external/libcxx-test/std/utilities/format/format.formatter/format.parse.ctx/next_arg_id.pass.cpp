//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17
// UNSUPPORTED: no-exceptions

// <format>

// constexpr size_t next_arg_id();

#include <format>

#include <cassert>
#include <cstring>
#include <string_view>

#include "test_macros.h"
#include "libcxx_tc_common.h"

constexpr bool test() {
  std::format_parse_context context("", 10);
  for (std::size_t i = 0; i < 10; ++i)
    TC_ASSERT_EXPR(i == context.next_arg_id());

  return true;
}

void test_exception() {
  std::format_parse_context context("", 1);
  context.check_arg_id(0);

#ifndef _LIBCPP_NO_EXCEPTIONS
  try {
    TEST_IGNORE_NODISCARD context.next_arg_id();
    TC_ASSERT_EXPR(false);
  } catch ([[maybe_unused]] const std::format_error& e) {
    LIBCPP_ASSERT(std::strcmp(e.what(), "Using automatic argument numbering in manual argument numbering mode") == 0);
    return;
  }
#endif // _LIBCPP_NO_EXCEPTIONS
  TC_ASSERT_EXPR(false);
}

int tc_utilities_format_format_formatter_format_parse_ctx_next_arg_id(void) {
  test();
  test_exception();
  static_assert(test());

  return 0;
}
