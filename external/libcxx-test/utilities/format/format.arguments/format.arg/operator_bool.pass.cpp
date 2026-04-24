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

// explicit operator bool() const noexcept
//
// Note more testing is done in the unit test for:
// template<class Visitor, class Context>
//   see below visit_format_arg(Visitor&& vis, basic_format_arg<Context> arg);

#include <format>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class CharT>
void test() {
  using Context = std::basic_format_context<CharT*, CharT>;
  {
    std::basic_format_arg<Context> format_arg{};
    ASSERT_NOEXCEPT(!format_arg);
    TC_ASSERT_EXPR(!format_arg);
    ASSERT_NOEXCEPT(static_cast<bool>(format_arg));
    TC_ASSERT_EXPR(!static_cast<bool>(format_arg));
  }
}

int tc_utilities_format_format_arguments_format_arg_operator_bool(void) {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif

  return 0;
}
