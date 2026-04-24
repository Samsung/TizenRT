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
// UNSUPPORTED: no-localization
// UNSUPPORTED: GCC-ALWAYS_INLINE-FIXME

// XFAIL: availability-fp_to_chars-missing

// <format>

// string vformat(const locale& loc, string_view fmt, format_args args);
// wstring vformat(const locale& loc, wstring_view fmt, wformat_args args);

#include <format>
#include <cassert>
#include <vector>

#include "test_macros.h"
#include "format_tests.h"
#include "string_literal.h"
#include "assert_macros.h"
#include "concat_macros.h"
#include "libcxx_tc_common.h"

auto test = []<class CharT, class... Args>(
                std::basic_string_view<CharT> expected, std::basic_string_view<CharT> fmt, Args&&... args) constexpr {
  std::basic_string<CharT> out = std::vformat(std::locale(), fmt, std::make_format_args<context_t<CharT>>(args...));
  TEST_REQUIRE(out == expected,
               TEST_WRITE_CONCATENATED(
                   "\nFormat string   ", fmt, "\nExpected output ", expected, "\nActual output   ", out, '\n'));
};

auto test_exception =
    []<class CharT, class... Args>(
        [[maybe_unused]] std::string_view what,
        [[maybe_unused]] std::basic_string_view<CharT> fmt,
        [[maybe_unused]] Args&&... args) {
      TEST_VALIDATE_EXCEPTION(
          std::format_error,
          [&]([[maybe_unused]] const std::format_error& e) {
            TEST_LIBCPP_REQUIRE(
                e.what() == what,
                TEST_WRITE_CONCATENATED(
                    "\nFormat string   ", fmt, "\nExpected exception ", what, "\nActual exception   ", e.what(), '\n'));
          },
          TEST_IGNORE_NODISCARD std::vformat(std::locale(), fmt, std::make_format_args<context_t<CharT>>(args...)));
    };

int tc_utilities_format_format_functions_vformat_locale(void) {
  format_tests<char, execution_modus::full>(test, test_exception);

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  format_tests_char_to_wchar_t(test);
  format_tests<wchar_t, execution_modus::full>(test, test_exception);
#endif

  return 0;
}
