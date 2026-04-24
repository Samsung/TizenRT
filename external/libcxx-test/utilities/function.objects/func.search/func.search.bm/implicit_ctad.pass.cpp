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
// UNSUPPORTED: c++98, c++03, c++11, c++14

// <functional>

// boyer_moore_searcher

// Make sure that the implicitly-generated CTAD works.

#include <functional>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_func_search_func_search_bm_implicit_ctad(void) {
  {
    char const* str = "hello";
    std::boyer_moore_searcher searcher(str, str + 3);
    ASSERT_SAME_TYPE(decltype(searcher), std::boyer_moore_searcher<char const*, std::hash<char>, std::equal_to<>>);
  }
  {
    struct myhash : std::hash<char> { };
    char const* str = "hello";
    std::boyer_moore_searcher searcher(str, str + 3, myhash{}, std::not_equal_to<>());
    ASSERT_SAME_TYPE(decltype(searcher), std::boyer_moore_searcher<char const*, myhash, std::not_equal_to<>>);
  }
  {
    struct myhash : std::hash<char> { };
    char const* str = "hello";
    std::boyer_moore_searcher searcher(str, str + 3, myhash{});
    ASSERT_SAME_TYPE(decltype(searcher), std::boyer_moore_searcher<char const*, myhash, std::equal_to<>>);
  }

  return 0;
}
