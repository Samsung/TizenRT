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

// basic_format_arg() noexcept;

// The class has several exposition only private constructors. These are tested
// in visit_format_arg.pass.cpp

#include <format>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class CharT>
void test() {
  using Context = std::basic_format_context<CharT*, CharT>;

  ASSERT_NOEXCEPT(std::basic_format_arg<Context>{});

  std::basic_format_arg<Context> format_arg{};
  TC_ASSERT_EXPR(!format_arg);
}

void test() {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif
#ifndef TEST_HAS_NO_CHAR8_T
  test<char8_t>();
#endif
  test<char16_t>();
  test<char32_t>();
}

int tc_utilities_format_format_arguments_format_arg_ctor(void) {
  test();

  return 0;
}
