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

// template<class charT, formattable<charT>... Ts>
//   struct formatter<pair-or-tuple<Ts...>, charT>

// constexpr void constexpr void set_brackets(basic_string_view<charT> opening,
//                                            basic_string_view<charT> closing) noexcept;

// Note this tests the basics of this function. It's tested in more detail in
// the format functions tests.

#include <format>
#include <tuple>
#include <utility>

#include "make_string.h"
#include "libcxx_tc_common.h"

#define SV(S) MAKE_STRING_VIEW(CharT, S)

template <class CharT, class Arg>
constexpr void test() {
  std::formatter<Arg, CharT> formatter;
  formatter.set_brackets(SV("open"), SV("close"));
  // Note the SV macro may throw, so can't use it.
  static_assert(noexcept(formatter.set_brackets(std::basic_string_view<CharT>{}, std::basic_string_view<CharT>{})));

  // Note there is no direct way to validate this function modified the object.
}

template <class CharT>
constexpr void test() {
  test<CharT, std::tuple<int>>();
  test<CharT, std::tuple<int, CharT>>();
  test<CharT, std::pair<int, CharT>>();
  test<CharT, std::tuple<int, CharT, bool>>();
}

constexpr bool test() {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif

  return true;
}

int tc_utilities_format_format_tuple_set_brackets(void) {
  test();
  static_assert(test());

  return 0;
}
