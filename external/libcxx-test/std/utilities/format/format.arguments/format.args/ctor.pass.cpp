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

// basic_format_args() noexcept;
// template<class... Args>
//   basic_format_args(const format-arg-store<Context, Args...>& store) noexcept;

#include <format>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class CharT>
void test() {
  using Context = std::basic_format_context<CharT*, CharT>;
  {
    ASSERT_NOEXCEPT(std::basic_format_args<Context>{});

    std::basic_format_args<Context> format_args{};
    TC_ASSERT_EXPR(!format_args.get(0));
  }
  {
    auto store = std::make_format_args<Context>(1);
    ASSERT_NOEXCEPT(std::basic_format_args<Context>{store});
    std::basic_format_args<Context> format_args{store};
    TC_ASSERT_EXPR(format_args.get(0));
    TC_ASSERT_EXPR(!format_args.get(1));
  }
  {
    auto store = std::make_format_args<Context>(1, 'c');
    ASSERT_NOEXCEPT(std::basic_format_args<Context>{store});
    std::basic_format_args<Context> format_args{store};
    TC_ASSERT_EXPR(format_args.get(0));
    TC_ASSERT_EXPR(format_args.get(1));
    TC_ASSERT_EXPR(!format_args.get(2));
  }
  {
    auto store = std::make_format_args<Context>(1, 'c', nullptr);
    ASSERT_NOEXCEPT(std::basic_format_args<Context>{store});
    std::basic_format_args<Context> format_args{store};
    TC_ASSERT_EXPR(format_args.get(0));
    TC_ASSERT_EXPR(format_args.get(1));
    TC_ASSERT_EXPR(format_args.get(2));
    TC_ASSERT_EXPR(!format_args.get(3));
  }
}

void test() {
  test<char>();
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
  test<wchar_t>();
#endif
}

int tc_utilities_format_format_arguments_format_args_ctor(void) {
  test();

  return 0;
}
