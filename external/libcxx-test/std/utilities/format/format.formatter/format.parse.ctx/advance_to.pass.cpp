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

// constexpr void advance_to(const_iterator it);

#include <format>

#include <cassert>
#include <string_view>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class CharT>
constexpr void test(const CharT* fmt) {
  {
    std::basic_format_parse_context<CharT> context(fmt);

    context.advance_to(context.begin() + 1);
    TC_ASSERT_EXPR(std::to_address(context.begin()) == fmt + 1);

    context.advance_to(context.begin() + 1);
    TC_ASSERT_EXPR(std::to_address(context.begin()) == fmt + 2);

    context.advance_to(context.begin() + 1);
    TC_ASSERT_EXPR(context.begin() == context.end());
  }
  {
    std::basic_string_view view{fmt};
    std::basic_format_parse_context context(view);

    context.advance_to(context.begin() + 1);
    TC_ASSERT_EXPR(std::to_address(context.begin()) == fmt + 1);

    context.advance_to(context.begin() + 1);
    TC_ASSERT_EXPR(std::to_address(context.begin()) == fmt + 2);

    context.advance_to(context.begin() + 1);
    TC_ASSERT_EXPR(context.begin() == context.end());
  }
}

constexpr bool test() {
  test("abc");
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test(L"abc");
#endif
#ifndef TEST_HAS_NO_CHAR8_T
  test(u8"abc");
#endif
  test(u"abc");
  test(U"abc");

  return true;
}

int tc_utilities_format_format_formatter_format_parse_ctx_advance_to(void) {
  test();
  static_assert(test());

  return 0;
}
