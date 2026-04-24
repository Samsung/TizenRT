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
// TODO FMT __builtin_memcpy isn't constexpr in GCC
// UNSUPPORTED: gcc-12, gcc-13

// <format>

// C++23 the formatter is a debug-enabled specialization.
// [format.functions]/25
//   Preconditions: formatter<remove_cvref_t<Ti>, charT> meets the
//   BasicFormatter requirements ([formatter.requirements]) for each Ti in
//   Args.

#include <format>
#include <cassert>
#include <concepts>
#include <iterator>
#include <type_traits>

#include "test_format_context.h"
#include "test_macros.h"
#include "make_string.h"
#include "libcxx_tc_common.h"

#define STR(S) MAKE_STRING(CharT, S)
#define SV(S) MAKE_STRING_VIEW(CharT, S)
#define CSTR(S) MAKE_CSTRING(CharT, S)

// This is based on the method found in
// clang/test/CXX/temp/temp.arg/temp.arg.nontype/p1-cxx20.cpp
template <std::size_t N>
struct Tester {
  // This is not part of the real test, but is used the deduce the size of the input.
  constexpr Tester(const char (&r)[N]) { __builtin_memcpy(text, r, N); }
  char text[N];

  // The size of the array shouldn't include the NUL character.
  static const std::size_t size = N - 1;

  template <class CharT>
  void
  test(const std::basic_string<CharT>& expected, const std::basic_string_view<CharT>& fmt, std::size_t offset) const {
    using Str = CharT[size];
    std::basic_format_parse_context<CharT> parse_ctx{fmt};
    std::formatter<Str, CharT> formatter;
    static_assert(std::semiregular<decltype(formatter)>);

    auto it = formatter.parse(parse_ctx);
    TC_ASSERT_EXPR(it == fmt.end() - offset);

    std::basic_string<CharT> result;
    auto out = std::back_inserter(result);
    using FormatCtxT = std::basic_format_context<decltype(out), CharT>;

    std::basic_string<CharT> buffer{text, text + N};
    // Note not too found of this hack
    Str* data = reinterpret_cast<Str*>(const_cast<CharT*>(buffer.c_str()));

    FormatCtxT format_ctx =
        test_format_context_create<decltype(out), CharT>(out, std::make_format_args<FormatCtxT>(*data));
    formatter.format(*data, format_ctx);
    TC_ASSERT_EXPR(result == expected);
  }

  template <class CharT>
  void test_termination_condition(const std::basic_string<CharT>& expected, const std::basic_string<CharT>& f) const {
    // The format-spec is valid if completely consumed or terminates at a '}'.
    // The valid inputs all end with a '}'. The test is executed twice:
    // - first with the terminating '}',
    // - second consuming the entire input.
    std::basic_string_view<CharT> fmt{f};
    TC_ASSERT_EXPR(fmt.back() == CharT('}') && "Pre-condition failure");

    test(expected, fmt, 1);
    fmt.remove_suffix(1);
    test(expected, fmt, 0);
  }
};

template <std::size_t N>
Tester(const char (&)[N]) -> Tester<N>;

template <Tester t, class CharT>
void test_helper_wrapper(std::basic_string<CharT> expected, std::basic_string<CharT> fmt) {
  t.test_termination_condition(expected, fmt);
}

#if TEST_STD_VER > 20
template <class CharT>
constexpr bool test_set_debug_format() {
  std::formatter<CharT[1], CharT> formatter;
  LIBCPP_ASSERT(formatter.__parser_.__type_ == std::__format_spec::__type::__default);

  formatter.set_debug_format();
  LIBCPP_ASSERT(formatter.__parser_.__type_ == std::__format_spec::__type::__debug);

  std::basic_string_view fmt = SV("s}");
  std::basic_format_parse_context<CharT> parse_ctx{fmt};
  formatter.parse(parse_ctx);
  LIBCPP_ASSERT(formatter.__parser_.__type_ == std::__format_spec::__type::__string);

  formatter.set_debug_format();
  LIBCPP_ASSERT(formatter.__parser_.__type_ == std::__format_spec::__type::__debug);

  return true;
}
#endif

template <class CharT>
void test_array() {
  test_helper_wrapper<" azAZ09,./<>?">(STR(" azAZ09,./<>?"), STR("}"));

  std::basic_string<CharT> s(CSTR("abc\0abc"), 7);
  test_helper_wrapper<"abc\0abc">(s, STR("}"));

  test_helper_wrapper<"world">(STR("world"), STR("}"));
  test_helper_wrapper<"world">(STR("world"), STR("_>}"));

  test_helper_wrapper<"world">(STR("   world"), STR(">8}"));
  test_helper_wrapper<"world">(STR("___world"), STR("_>8}"));
  test_helper_wrapper<"world">(STR("_world__"), STR("_^8}"));
  test_helper_wrapper<"world">(STR("world___"), STR("_<8}"));

  test_helper_wrapper<"world">(STR("world"), STR(".5}"));
  test_helper_wrapper<"universe">(STR("unive"), STR(".5}"));

  test_helper_wrapper<"world">(STR("%world%"), STR("%^7.7}"));
  test_helper_wrapper<"universe">(STR("univers"), STR("%^7.7}"));

#if TEST_STD_VER > 20
  test_set_debug_format<CharT>();
  static_assert(test_set_debug_format<CharT>());
#endif
}

int tc_utilities_format_format_formatter_format_formatter_spec_formatter_char_array(void) {
  test_array<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test_array<wchar_t>();
#endif

  return 0;
}
