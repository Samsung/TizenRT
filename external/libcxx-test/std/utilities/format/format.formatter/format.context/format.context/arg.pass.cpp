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

// <format>

// basic_format_arg<basic_format_context> arg(size_t id) const;

#include <format>
#include <iterator>
#include <cassert>

#include "test_basic_format_arg.h"
#include "test_format_context.h"
#include "test_macros.h"
#include "make_string.h"
#include "libcxx_tc_common.h"

template <class OutIt, class CharT>
void test() {
  std::basic_string<CharT> string = MAKE_STRING(CharT, "string");
  auto store = std::make_format_args<std::basic_format_context<OutIt, CharT>>(
      true, CharT('a'), 42, string);
  std::basic_format_args args = store;

  std::basic_string<CharT> output;
  const std::basic_format_context context = test_format_context_create(OutIt{output}, args);
  LIBCPP_ASSERT(args.__size() == 4);
  ASSERT_NOEXCEPT(context.arg(0));
  for (std::size_t i = 0, e = args.__size(); i != e; ++i) {
    TC_ASSERT_EXPR(context.arg(i));
  }
  TC_ASSERT_EXPR(!context.arg(args.__size()));

  TC_ASSERT_EXPR(test_basic_format_arg(context.arg(0), true));
  TC_ASSERT_EXPR(test_basic_format_arg(context.arg(1), CharT('a')));
  TC_ASSERT_EXPR(test_basic_format_arg(context.arg(2), 42));
  TC_ASSERT_EXPR(test_basic_format_arg(context.arg(3),
                               std::basic_string_view<CharT>(string)));
}

int tc_utilities_format_format_formatter_format_context_format_context_arg(void) {
  test<std::back_insert_iterator<std::basic_string<char>>, char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<std::back_insert_iterator<std::basic_string<wchar_t>>, wchar_t>();
#endif

  return 0;
}
