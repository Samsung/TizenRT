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
// UNSUPPORTED: GCC-ALWAYS_INLINE-FIXME

// XFAIL: availability-fp_to_chars-missing

// <format>

// template<class Out, class... Args>
//   Out format_to(Out out, format-string<Args...> fmt, const Args&... args);
// template<class Out, class... Args>
//   Out format_to(Out out, wformat-string<Args...> fmt, const Args&... args);

#include <format>
#include <algorithm>
#include <cassert>
#include <iterator>
#include <list>
#include <vector>

#include "test_macros.h"
#include "format_tests.h"
#include "string_literal.h"
#include "test_format_string.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

auto test =
    []<class CharT, class... Args>(
        std::basic_string_view<CharT> expected, test_format_string<CharT, Args...> fmt, Args&&... args) constexpr {
      {
        std::basic_string<CharT> out(expected.size(), CharT(' '));
        auto it = std::format_to(out.begin(), fmt, std::forward<Args>(args)...);
        TC_ASSERT_EXPR(it == out.end());
        TC_ASSERT_EXPR(out == expected);
      }
      {
        std::list<CharT> out;
        std::format_to(std::back_inserter(out), fmt, std::forward<Args>(args)...);
        TC_ASSERT_EXPR(std::equal(out.begin(), out.end(), expected.begin(), expected.end()));
      }
      {
        std::vector<CharT> out;
        std::format_to(std::back_inserter(out), fmt, std::forward<Args>(args)...);
        TC_ASSERT_EXPR(std::equal(out.begin(), out.end(), expected.begin(), expected.end()));
      }
      {
        TC_ASSERT_EXPR(expected.size() < 4096 && "Update the size of the buffer.");
        CharT out[4096];
        cpp20_output_iterator<CharT*> it = std::format_to(cpp20_output_iterator{out}, fmt, std::forward<Args>(args)...);
        TC_ASSERT_EXPR(std::distance(out, base(it)) == int(expected.size()));
        // Convert to std::string since output contains '\0' for boolean tests.
        TC_ASSERT_EXPR(std::basic_string<CharT>(out, base(it)) == expected);
      }
    };

auto test_exception = []<class CharT, class... Args>(std::string_view, std::basic_string_view<CharT>, Args&&...) {
  // After P2216 most exceptions thrown by std::format become ill-formed.
  // Therefore this tests does nothing.
  // A basic ill-formed test is done in format.verify.cpp
  // The exceptions are tested by other functions that don't use the basic-format-string as fmt argument.
};

int tc_utilities_format_format_functions_format_to(void) {
  format_tests<char, execution_modus::partial>(test, test_exception);

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  format_tests_char_to_wchar_t(test);
  format_tests<wchar_t, execution_modus::partial>(test, test_exception);
#endif

  return 0;
}
