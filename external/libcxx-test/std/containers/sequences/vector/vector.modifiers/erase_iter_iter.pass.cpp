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

// <vector>

// iterator erase(const_iterator first, const_iterator last);

#include <vector>
#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"

#include "asan_testing.h"

int tc_libcxx_containers_vector_modifiers_erase_iter_iter(void)
{
    int a1[] = {1, 2, 3};
    {
        std::vector<int> l1(a1, a1+3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
        std::vector<int>::iterator i = l1.erase(l1.cbegin(), l1.cbegin());
        TC_ASSERT_EXPR(l1.size() == 3);
        TC_ASSERT_EXPR(distance(l1.cbegin(), l1.cend()) == 3);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    }
    {
        std::vector<int> l1(a1, a1+3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
        std::vector<int>::iterator i = l1.erase(l1.cbegin(), next(l1.cbegin()));
        TC_ASSERT_EXPR(l1.size() == 2);
        TC_ASSERT_EXPR(distance(l1.cbegin(), l1.cend()) == 2);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR(l1 == std::vector<int>(a1+1, a1+3));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    }
    {
        std::vector<int> l1(a1, a1+3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
        std::vector<int>::iterator i = l1.erase(l1.cbegin(), next(l1.cbegin(), 2));
        TC_ASSERT_EXPR(l1.size() == 1);
        TC_ASSERT_EXPR(distance(l1.cbegin(), l1.cend()) == 1);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR(l1 == std::vector<int>(a1+2, a1+3));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    }
    {
        std::vector<int> l1(a1, a1+3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
        std::vector<int>::iterator i = l1.erase(l1.cbegin(), next(l1.cbegin(), 3));
        TC_ASSERT_EXPR(l1.size() == 0);
        TC_ASSERT_EXPR(distance(l1.cbegin(), l1.cend()) == 0);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l1));
    }
    {
        std::vector<std::vector<int> > outer(2, std::vector<int>(1));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer[0]));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer[1]));
        outer.erase(outer.begin(), outer.begin());
        TC_ASSERT_EXPR(outer.size() == 2);
        TC_ASSERT_EXPR(outer[0].size() == 1);
        TC_ASSERT_EXPR(outer[1].size() == 1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer[0]));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(outer[1]));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
