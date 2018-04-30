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

// iterator insert(const_iterator position, value_type&& x);

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_allocator.h"
#include "MoveOnly.h"
#include "asan_testing.h"

int tc_libcxx_containers_vector_modifiers_insert_iter_rvalue(void)
{
    {
        std::vector<MoveOnly> v(100);
        std::vector<MoveOnly>::iterator i = v.insert(v.cbegin() + 10, MoveOnly(3));
        TC_ASSERT_EXPR(v.size() == 101);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == MoveOnly());
        TC_ASSERT_EXPR(v[j] == MoveOnly(3));
        for (++j; j < 101; ++j)
            TC_ASSERT_EXPR(v[j] == MoveOnly());
    }
    {
        std::vector<MoveOnly, limited_allocator<MoveOnly, 300> > v(100);
        std::vector<MoveOnly, limited_allocator<MoveOnly, 300> >::iterator i = v.insert(v.cbegin() + 10, MoveOnly(3));
        TC_ASSERT_EXPR(v.size() == 101);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == MoveOnly());
        TC_ASSERT_EXPR(v[j] == MoveOnly(3));
        for (++j; j < 101; ++j)
            TC_ASSERT_EXPR(v[j] == MoveOnly());
    }
    TC_SUCCESS_RESULT();
    return 0;
}
