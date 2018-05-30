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
//   pair<T, T>
//   minmax(initializer_list<T> t, Compare comp);
//
//  Complexity: At most (3/2) * t.size() applications of the corresponding predicate.

#include <algorithm>
#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"
#include "counting_predicates.hpp"

bool all_equal(int, int) { return false; } // everything is equal

static int test_all_equal(std::initializer_list<int> il)
{
    binary_counting_predicate<bool(*)(int, int), int, int> pred (all_equal);
    std::pair<int, int> p = std::minmax(il, std::ref(pred));
    const int *ptr = il.end();
    TC_ASSERT_EXPR(p.first == *il.begin());
    TC_ASSERT_EXPR(p.second == *--ptr);
    TC_ASSERT_EXPR(pred.count() <= ((3 * il.size()) / 2));
    return 0;
}

int tc_libcxx_algorithms_alg_min_max_minmax_init_list_comp(void)
{
    TC_ASSERT_EXPR((std::minmax({1, 2, 3}, std::greater<int>()) == std::pair<int, int>(3, 1)));
    TC_ASSERT_EXPR((std::minmax({1, 3, 2}, std::greater<int>()) == std::pair<int, int>(3, 1)));
    TC_ASSERT_EXPR((std::minmax({2, 1, 3}, std::greater<int>()) == std::pair<int, int>(3, 1)));
    TC_ASSERT_EXPR((std::minmax({2, 3, 1}, std::greater<int>()) == std::pair<int, int>(3, 1)));
    TC_ASSERT_EXPR((std::minmax({3, 1, 2}, std::greater<int>()) == std::pair<int, int>(3, 1)));
    TC_ASSERT_EXPR((std::minmax({3, 2, 1}, std::greater<int>()) == std::pair<int, int>(3, 1)));
    TC_ASSERT_EXPR((std::minmax({1, 2, 3}, all_equal          ) == std::pair<int, int>(1, 3)));

    binary_counting_predicate<std::greater<int>, int, int> pred ((std::greater<int>()));
    TC_ASSERT_EXPR((std::minmax({1, 2, 2, 3, 3, 3, 5, 5, 5, 5, 5, 3}, std::ref(pred)) == std::pair<int, int>(5, 1)));
    TC_ASSERT_EXPR(pred.count() <= 18); // size == 12

    TC_ASSERT_FUNC((test_all_equal({0})));
    TC_ASSERT_FUNC((test_all_equal({0,1})));
    TC_ASSERT_FUNC((test_all_equal({0,1,2})));
    TC_ASSERT_FUNC((test_all_equal({0,1,2,3})));
    TC_ASSERT_FUNC((test_all_equal({0,1,2,3,4})));
    TC_ASSERT_FUNC((test_all_equal({0,1,2,3,4,5})));
    TC_ASSERT_FUNC((test_all_equal({0,1,2,3,4,5,6})));
    TC_ASSERT_FUNC((test_all_equal({0,1,2,3,4,5,6,7})));
    TC_ASSERT_FUNC((test_all_equal({0,1,2,3,4,5,6,7,8})));
    TC_ASSERT_FUNC((test_all_equal({0,1,2,3,4,5,6,7,8,9})));
    TC_ASSERT_FUNC((test_all_equal({0,1,2,3,4,5,6,7,8,9,10})));
    TC_ASSERT_FUNC((test_all_equal({0,1,2,3,4,5,6,7,8,9,10,11})));

#if TEST_STD_VER >= 14
    {
    static_assert((std::minmax({1, 2, 3}, std::greater<int>()) == std::pair<int, int>(3, 1)), "");
    static_assert((std::minmax({1, 3, 2}, std::greater<int>()) == std::pair<int, int>(3, 1)), "");
    static_assert((std::minmax({2, 1, 3}, std::greater<int>()) == std::pair<int, int>(3, 1)), "");
    static_assert((std::minmax({2, 3, 1}, std::greater<int>()) == std::pair<int, int>(3, 1)), "");
    static_assert((std::minmax({3, 1, 2}, std::greater<int>()) == std::pair<int, int>(3, 1)), "");
    static_assert((std::minmax({3, 2, 1}, std::greater<int>()) == std::pair<int, int>(3, 1)), "");
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
