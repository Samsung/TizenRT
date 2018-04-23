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

// <vector>

// vector(vector&& c);

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"
#include "MoveOnly.h"
#include "test_allocator.h"
#include "asan_testing.h"

int tc_libcxx_containers_vector_cons_move(void)
{
    {
        std::vector<MoveOnly, test_allocator<MoveOnly> > l(test_allocator<MoveOnly>(5));
        std::vector<MoveOnly, test_allocator<MoveOnly> > lo(test_allocator<MoveOnly>(5));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
        for (int i = 1; i <= 3; ++i)
        {
            l.push_back(i);
            lo.push_back(i);
        }
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
        std::vector<MoveOnly, test_allocator<MoveOnly> > l2 = std::move(l);
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == lo.get_allocator());
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l2));
    }
    {
        std::vector<MoveOnly, other_allocator<MoveOnly> > l(other_allocator<MoveOnly>(5));
        std::vector<MoveOnly, other_allocator<MoveOnly> > lo(other_allocator<MoveOnly>(5));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
        for (int i = 1; i <= 3; ++i)
        {
            l.push_back(i);
            lo.push_back(i);
        }
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(lo));
        std::vector<MoveOnly, other_allocator<MoveOnly> > l2 = std::move(l);
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == lo.get_allocator());
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(l2));
    }
    {
        int a1[] = {1, 3, 7, 9, 10};
        std::vector<int> c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c1));
        std::vector<int>::const_iterator i = c1.begin();
        std::vector<int> c2 = std::move(c1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
        std::vector<int>::iterator j = c2.erase(i);
        TC_ASSERT_EXPR(*j == 3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c2));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
