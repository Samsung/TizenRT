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
// UNSUPPORTED: no-exceptions

// <format>

// constexpr void check_arg_id(size_t id);

#include <format>

#include <cassert>
#include <cstring>
#include <string_view>

#include "test_macros.h"
#include "libcxx_tc_common.h"

constexpr bool test() {
  std::format_parse_context context("", 10);
  for (std::size_t i = 0; i < 10; ++i)
    context.check_arg_id(i);

  return true;
}

void test_exception() {
  [] {
    std::format_parse_context context("", 1);
    TEST_IGNORE_NODISCARD context.next_arg_id();
#ifndef _LIBCPP_NO_EXCEPTIONS
    try {
      context.check_arg_id(0);
      TC_ASSERT_EXPR(false);
    } catch ([[maybe_unused]] const std::format_error& e) {
      LIBCPP_ASSERT(std::strcmp(e.what(), "Using manual argument numbering in automatic argument numbering mode") == 0);
      return;
    }
#endif // _LIBCPP_NO_EXCEPTIONS
    TC_ASSERT_EXPR(false);
  }();

  auto test_arg = [](std::size_t num_args) {
    std::format_parse_context context("", num_args);
    // Out of bounds access is valid if !std::is_constant_evaluated()
    for (std::size_t i = 0; i <= num_args; ++i)
      context.check_arg_id(i);
  };
  for (std::size_t i = 0; i < 10; ++i)
    test_arg(i);
}

int tc_utilities_format_format_formatter_format_parse_ctx_check_arg_id(void) {
  test();
  test_exception();
  static_assert(test());

  return 0;
}
