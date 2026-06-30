//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <format>

// constexpr explicit
// basic_format_parse_context(basic_string_view<charT> fmt,
//                            size_t num_args = 0) noexcept

#include <format>

#include <cassert>
#include <string_view>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class CharT>
constexpr void test(const CharT* fmt) {
  // Validate the constructor is explicit.
  static_assert(
      !std::is_convertible_v<std::basic_string_view<CharT>,
                             std::basic_format_parse_context<CharT> >);
  static_assert(
      !std::is_copy_constructible_v<std::basic_format_parse_context<CharT> >);
  static_assert(
      !std::is_copy_assignable_v<std::basic_format_parse_context<CharT> >);
  // The move operations are implicitly deleted due to the
  // deleted copy operations.
  static_assert(
      !std::is_move_constructible_v<std::basic_format_parse_context<CharT> >);
  static_assert(
      !std::is_move_assignable_v<std::basic_format_parse_context<CharT> >);

  ASSERT_NOEXCEPT(std::basic_format_parse_context{std::basic_string_view<CharT>{}});
  ASSERT_NOEXCEPT(std::basic_format_parse_context{std::basic_string_view<CharT>{}, 42});

  {
    std::basic_format_parse_context<CharT> context(fmt);
    TC_ASSERT_EXPR(std::to_address(context.begin()) == &fmt[0]);
    TC_ASSERT_EXPR(std::to_address(context.end()) == &fmt[3]);
  }
  {
    std::basic_string_view view{fmt};
    std::basic_format_parse_context context(view);
    TC_ASSERT_EXPR(context.begin() == view.begin());
    TC_ASSERT_EXPR(context.end() == view.end());
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

int tc_utilities_format_format_formatter_format_parse_ctx_ctor(void) {
  test();
  static_assert(test());

  return 0;
}
