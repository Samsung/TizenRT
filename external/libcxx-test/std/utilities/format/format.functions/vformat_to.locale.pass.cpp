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

// template<class Out>
//   Out vformat_to(Out out, const locale& loc, string_view fmt,
//                  format_args args);
// template<class Out>
//    Out vformat_to(Out out, const locale& loc, wstring_view fmt,
//                   wformat_args args);

#include <format>
#include <algorithm>
#include <cassert>
#include <list>
#include <vector>

#include "assert_macros.h"
#include "concat_macros.h"
#include "format_tests.h"
#include "string_literal.h"
#include "libcxx_tc_common.h"

auto test = []<class CharT, class... Args>(
                std::basic_string_view<CharT> expected, std::basic_string_view<CharT> fmt, Args&&... args) constexpr {
  {
    std::basic_string<CharT> out(expected.size(), CharT(' '));
    auto it = std::vformat_to(out.begin(), std::locale(), fmt, std::make_format_args<context_t<CharT>>(args...));
    TC_ASSERT_EXPR(it == out.end());
    TC_ASSERT_EXPR(out == expected);
  }
  {
    std::list<CharT> out;
    std::vformat_to(std::back_inserter(out), std::locale(), fmt, std::make_format_args<context_t<CharT>>(args...));
    TC_ASSERT_EXPR(std::equal(out.begin(), out.end(), expected.begin(), expected.end()));
  }
  {
    std::vector<CharT> out;
    std::vformat_to(std::back_inserter(out), std::locale(), fmt, std::make_format_args<context_t<CharT>>(args...));
    TC_ASSERT_EXPR(std::equal(out.begin(), out.end(), expected.begin(), expected.end()));
  }
  {
    TC_ASSERT_EXPR(expected.size() < 4096 && "Update the size of the buffer.");
    CharT out[4096];
    CharT* it = std::vformat_to(out, std::locale(), fmt, std::make_format_args<context_t<CharT>>(args...));
    TC_ASSERT_EXPR(std::distance(out, it) == int(expected.size()));
    // Convert to std::string since output contains '\0' for boolean tests.
    TC_ASSERT_EXPR(std::basic_string<CharT>(out, it) == expected);
  }
};

auto test_exception = []<class CharT, class... Args>([[maybe_unused]] std::string_view what,
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
      [&] {
        std::basic_string<CharT> out;
        std::vformat_to(std::back_inserter(out), std::locale(), fmt, std::make_format_args<context_t<CharT>>(args...));
      }());
};

int tc_utilities_format_format_functions_vformat_to_locale(void) {
  format_tests<char, execution_modus::partial>(test, test_exception);

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  format_tests_char_to_wchar_t(test);
  format_tests<wchar_t, execution_modus::partial>(test, test_exception);
#endif

  return 0;
}
