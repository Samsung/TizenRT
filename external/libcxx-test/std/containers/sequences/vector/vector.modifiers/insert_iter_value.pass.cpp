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

// iterator insert(const_iterator position, const value_type& x);

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cstddef>

#include "test_macros.h"
#include "test_allocator.h"
#include "asan_testing.h"

int tc_libcxx_containers_vector_modifiers_insert_iter_value(void)
{
    {
        std::vector<int> v(100);
        std::vector<int>::iterator i = v.insert(v.cbegin() + 10, 1);
        TC_ASSERT_EXPR(v.size() == 101);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        TC_ASSERT_EXPR(v[j] == 1);
        for (++j; j < 101; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        std::vector<int> v(100);
        while(v.size() < v.capacity()) v.push_back(0); // force reallocation
        size_t sz = v.size();
        std::vector<int>::iterator i = v.insert(v.cbegin() + 10, 1);
        TC_ASSERT_EXPR(v.size() == sz + 1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        std::size_t j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        TC_ASSERT_EXPR(v[j] == 1);
        for (++j; j < v.size(); ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        std::vector<int> v(100);
        while(v.size() < v.capacity()) v.push_back(0);
        v.pop_back(); v.pop_back(); // force no reallocation
        size_t sz = v.size();
        std::vector<int>::iterator i = v.insert(v.cbegin() + 10, 1);
        TC_ASSERT_EXPR(v.size() == sz + 1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        std::size_t j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        TC_ASSERT_EXPR(v[j] == 1);
        for (++j; j < v.size(); ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        std::vector<int, limited_allocator<int, 300> > v(100);
        std::vector<int, limited_allocator<int, 300> >::iterator i = v.insert(v.cbegin() + 10, 1);
        TC_ASSERT_EXPR(v.size() == 101);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        TC_ASSERT_EXPR(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        TC_ASSERT_EXPR(v[j] == 1);
        for (++j; j < 101; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
