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

// template<class T, class charT = char>
//   requires same_as<remove_cvref_t<T>, T> && formattable<T, charT>
// class range_formatter

// constexpr formatter<T, charT>& underlying() noexcept;
// constexpr const formatter<T, charT>& underlying() const noexcept;

#include <concepts>
#include <format>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class CharT>
constexpr void test_underlying() {
  {
    std::range_formatter<int, CharT> formatter;
    [[maybe_unused]] std::same_as<std::formatter<int, CharT>&> decltype(auto) underlying = formatter.underlying();
    static_assert(noexcept(formatter.underlying()));
  }
  {
    const std::range_formatter<int, CharT> formatter;
    [[maybe_unused]] std::same_as<const std::formatter<int, CharT>&> decltype(auto) underlying = formatter.underlying();
    static_assert(noexcept(formatter.underlying()));
  }
}

constexpr bool test() {
  test_underlying<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test_underlying<wchar_t>();
#endif

  return true;
}

int tc_utilities_format_format_range_format_range_formatter_underlying(void) {
  test();
  static_assert(test());

  return 0;
}
