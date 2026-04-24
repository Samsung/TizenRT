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
// XFAIL: no-wide-characters

// <format>

// template<class... Args>
//   format-arg-store<wformat_context, Args...>
//   make_wformat_args(const Args&... args);

#include <cassert>
#include <format>

#include "test_basic_format_arg.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_format_format_arguments_format_arg_store_make_wformat_args(void) {
  [[maybe_unused]] auto store = std::make_wformat_args(42, nullptr, false, 'x');

  LIBCPP_STATIC_ASSERT(
      std::same_as<decltype(store), std::__format_arg_store<std::wformat_context, int, nullptr_t, bool, char>>);

  return 0;
}
