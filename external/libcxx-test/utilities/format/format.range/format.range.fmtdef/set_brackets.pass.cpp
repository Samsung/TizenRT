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
// UNSUPPORTED: c++03, c++11, c++14, c++17, c++20

// UNSUPPORTED: GCC-ALWAYS_INLINE-FIXME

// <format>

// template<ranges::input_range R, class charT>
//   struct range-default-formatter<range_format::sequence, R, charT>

// constexpr void constexpr void set_brackets(basic_string_view<charT> opening,
//                                            basic_string_view<charT> closing) noexcept;

#include <format>
#include <cassert>
#include <iterator>
#include <type_traits>
#include <vector>

#include "make_string.h"
#include "test_format_context.h"
#include "libcxx_tc_common.h"

#define SV(S) MAKE_STRING_VIEW(CharT, S)

template <class CharT>
constexpr void test_setter() {
  std::formatter<std::vector<int>, CharT> formatter;
  formatter.set_brackets(SV("open"), SV("close"));
  // Note the SV macro may throw, so can't use it.
  static_assert(noexcept(formatter.set_brackets(std::basic_string_view<CharT>{}, std::basic_string_view<CharT>{})));

  // Note there is no direct way to validate this function modified the object.
  if (!std::is_constant_evaluated()) {
    using String     = std::basic_string<CharT>;
    using OutIt      = std::back_insert_iterator<String>;
    using FormatCtxT = std::basic_format_context<OutIt, CharT>;

    String result;
    OutIt out             = std::back_inserter(result);
    FormatCtxT format_ctx = test_format_context_create<OutIt, CharT>(out, std::make_format_args<FormatCtxT>());
    formatter.format(std::vector<int>{0, 42, 99}, format_ctx);
    TC_ASSERT_EXPR(result == SV("open0, 42, 99close"));
  }
}

constexpr bool test() {
  test_setter<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test_setter<wchar_t>();
#endif

  return true;
}

int tc_utilities_format_format_range_format_range_fmtdef_set_brackets(void) {
  test();
  static_assert(test());

  return 0;
}
