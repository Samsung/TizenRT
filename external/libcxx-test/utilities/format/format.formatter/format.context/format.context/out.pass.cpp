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

// <format>

// iterator out();

#include <format>
#include <cassert>
#include <string>

#include "test_macros.h"
#include "test_format_context.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

template <class OutIt, class CharT>
void test(
    std::basic_format_args<std::basic_format_context<OutIt, CharT>> args) {
  {
    std::basic_string<CharT> str      = std::basic_string<CharT>(3, CharT(' '));
    std::basic_format_context context = test_format_context_create(OutIt{str.begin()}, args);

    // Note this operation is moving the iterator
    OutIt out_it = context.out();
    *out_it++    = CharT('a');
    *out_it++    = CharT('b');
    *out_it++    = CharT('c');

    TC_ASSERT_EXPR(str.size() == 3);
    TC_ASSERT_EXPR(str[0] == CharT('a'));
    TC_ASSERT_EXPR(str[1] == CharT('b'));
    TC_ASSERT_EXPR(str[2] == CharT('c'));
  }
}

void test() {
  test(std::basic_format_args(
      std::make_format_args<
          std::basic_format_context<cpp20_output_iterator<std::basic_string<char>::iterator>, char>>()));

#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test(std::basic_format_args(
      std::make_format_args<
          std::basic_format_context<cpp20_output_iterator<std::basic_string<wchar_t>::iterator>, wchar_t>>()));
#endif
#ifndef TEST_HAS_NO_CHAR8_T
  test(std::basic_format_args(
      std::make_format_args<
          std::basic_format_context<cpp20_output_iterator<std::basic_string<char8_t>::iterator>, char8_t>>()));
#endif
  test(std::basic_format_args(
      std::make_format_args<
          std::basic_format_context<cpp20_output_iterator<std::basic_string<char16_t>::iterator>, char16_t>>()));
  test(std::basic_format_args(
      std::make_format_args<
          std::basic_format_context<cpp20_output_iterator<std::basic_string<char32_t>::iterator>, char32_t>>()));
}

int tc_utilities_format_format_formatter_format_context_format_context_out(void) {
  test();

  return 0;
}
