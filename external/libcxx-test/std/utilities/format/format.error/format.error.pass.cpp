//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <format>

// class format_error;

#include <format>
#include <type_traits>
#include <cstring>
#include <string>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_format_format_error_format_error(void) {
  static_assert(std::is_base_of_v<std::runtime_error, std::format_error>);
  static_assert(std::is_polymorphic_v<std::format_error>);

  {
    const char* msg = "format_error message c-string";
    std::format_error e(msg);
    TC_ASSERT_EXPR(std::strcmp(e.what(), msg) == 0);
    std::format_error e2(e);
    TC_ASSERT_EXPR(std::strcmp(e2.what(), msg) == 0);
    e2 = e;
    TC_ASSERT_EXPR(std::strcmp(e2.what(), msg) == 0);
  }
  {
    std::string msg("format_error message std::string");
    std::format_error e(msg);
    TC_ASSERT_EXPR(e.what() == msg);
    std::format_error e2(e);
    TC_ASSERT_EXPR(e2.what() == msg);
    e2 = e;
    TC_ASSERT_EXPR(e2.what() == msg);
  }

  return 0;
}
