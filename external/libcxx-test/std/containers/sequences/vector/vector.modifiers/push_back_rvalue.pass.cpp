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

// static int push_back(value_type&& x);

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cstddef>
#include "MoveOnly.h"
#include "test_allocator.h"
#include "asan_testing.h"

int tc_libcxx_containers_vector_modifiers_push_back_rvalue(void)
{
    {
        std::vector<MoveOnly> c;
        c.push_back(MoveOnly(0));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(1));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(2));
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(3));
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(4));
        TC_ASSERT_EXPR(c.size() == 5);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == MoveOnly(j));
    }
    {
        // libc++ needs 15 because it grows by 2x (1 + 2 + 4 + 8).
        // Use 17 for implementations that dynamically allocate a container proxy
        // and grow by 1.5x (1 for proxy + 1 + 2 + 3 + 4 + 6).
        std::vector<MoveOnly, limited_allocator<MoveOnly, 17> > c;
        c.push_back(MoveOnly(0));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(1));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(2));
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(3));
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == MoveOnly(j));
        c.push_back(MoveOnly(4));
        TC_ASSERT_EXPR(c.size() == 5);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        for (int j = 0; static_cast<std::size_t>(j) < c.size(); ++j)
            TC_ASSERT_EXPR(c[j] == MoveOnly(j));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
