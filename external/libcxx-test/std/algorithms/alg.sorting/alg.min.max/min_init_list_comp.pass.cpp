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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <algorithm>

// template<class T, class Compare>
//   T
//   min(initializer_list<T> t, Compare comp);

#include <algorithm>
#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_algorithms_alg_min_max_min_init_list_comp(void)
{
    int i = std::min({2, 3, 1}, std::greater<int>());
    TC_ASSERT_EXPR(i == 3);
    i = std::min({2, 1, 3}, std::greater<int>());
    TC_ASSERT_EXPR(i == 3);
    i = std::min({3, 1, 2}, std::greater<int>());
    TC_ASSERT_EXPR(i == 3);
    i = std::min({3, 2, 1}, std::greater<int>());
    TC_ASSERT_EXPR(i == 3);
    i = std::min({1, 2, 3}, std::greater<int>());
    TC_ASSERT_EXPR(i == 3);
    i = std::min({1, 3, 2}, std::greater<int>());
    TC_ASSERT_EXPR(i == 3);
#if TEST_STD_VER >= 14
    {
    static_assert(std::min({1, 3, 2}, std::greater<int>()) == 3, "");
    static_assert(std::min({2, 1, 3}, std::greater<int>()) == 3, "");
    static_assert(std::min({3, 2, 1}, std::greater<int>()) == 3, "");
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
