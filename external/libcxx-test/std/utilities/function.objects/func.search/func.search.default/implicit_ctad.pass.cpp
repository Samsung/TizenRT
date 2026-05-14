//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++98, c++03, c++11, c++14

// <functional>

// default searcher

// Make sure that the implicitly-generated CTAD works.

#include <functional>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_func_search_func_search_default_implicit_ctad(void) {
  {
    char const* str = "hello";
    std::default_searcher searcher(str, str + 3);
    ASSERT_SAME_TYPE(decltype(searcher), std::default_searcher<char const*, std::equal_to<>>);
  }
  {
    char const* str = "hello";
    std::default_searcher searcher(str, str + 3, std::not_equal_to<>());
    ASSERT_SAME_TYPE(decltype(searcher), std::default_searcher<char const*, std::not_equal_to<>>);
  }

  return 0;
}
