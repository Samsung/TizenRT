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

// libc++ supports basic_format_string in C++20 as an extension
// UNSUPPORTED: !stdlib=libc++ && c++20

// <format>

// template<class charT, class... Args>
// class basic_format_string<charT, type_identity_t<Args>...>
//
// constexpr basic_string_view<charT> get() const noexcept { return str; }

#include <format>

#include <cassert>
#include <concepts>
#include <string_view>

#include "test_macros.h"
#include "make_string.h"
#include "libcxx_tc_common.h"

#define CSTR(S) MAKE_CSTRING(CharT, S)
#define SV(S) MAKE_STRING_VIEW(CharT, S)

template <class CharT>
constexpr bool test() {
  TC_ASSERT_EXPR((std::basic_format_string<CharT>{CSTR("foo")}.get() == SV("foo")));
  TC_ASSERT_EXPR((std::basic_format_string<CharT, int>{CSTR("{}")}.get() == SV("{}")));
  TC_ASSERT_EXPR((std::basic_format_string<CharT, int, char>{CSTR("{} {:*>6}")}.get() == SV("{} {:*>6}")));

  // Embedded NUL character
  TC_ASSERT_EXPR((std::basic_format_string<CharT, void*, bool>{SV("{}\0{}")}.get() == SV("{}\0{}")));
  return true;
}

int tc_utilities_format_format_fmt_string_get(void) {
  test<char>();
  static_assert(test<char>());
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
  static_assert(test<wchar_t>());
#endif
  return 0;
}
