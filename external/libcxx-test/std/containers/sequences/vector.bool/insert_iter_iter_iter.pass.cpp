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
// vector<bool>

// template <class Iter>
//   iterator insert(const_iterator position, Iter first, Iter last);

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cstddef>

#include "test_macros.h"
#include "test_iterators.h"

int tc_libcxx_containers_vector_bool_insert_iter_iter_iter(void)
{
    {
        std::vector<bool> v(100);
        bool a[] = {1, 0, 0, 1, 1};
        const unsigned N = sizeof(a)/sizeof(a[0]);
        std::vector<bool>::iterator i = v.insert(v.cbegin() + 10, input_iterator<const bool*>(a),
                                        input_iterator<const bool*>(a+N));
        TC_ASSERT_EXPR(v.size() == 100 + N);
        TC_ASSERT_EXPR(i == v.begin() + 10);
        std::size_t j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        for (std::size_t k = 0; k < N; ++j, ++k)
            TC_ASSERT_EXPR(v[j] == a[k]);
        for (; j < v.size(); ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        std::vector<bool> v(100);
        bool a[] = {1, 0, 0, 1, 1};
        const unsigned N = sizeof(a)/sizeof(a[0]);
        std::vector<bool>::iterator i = v.insert(v.cbegin() + 10, forward_iterator<const bool*>(a),
                                        forward_iterator<const bool*>(a+N));
        TC_ASSERT_EXPR(v.size() == 100 + N);
        TC_ASSERT_EXPR(i == v.begin() + 10);
        int j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        for (std::size_t k = 0; k < N; ++j, ++k)
            TC_ASSERT_EXPR(v[j] == a[k]);
        for (; j < 105; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        std::vector<bool> v(100);
        while(v.size() < v.capacity()) v.push_back(false);
        size_t sz = v.size();
        bool a[] = {1, 0, 0, 1, 1};
        const unsigned N = sizeof(a)/sizeof(a[0]);
        std::vector<bool>::iterator i = v.insert(v.cbegin() + 10, forward_iterator<const bool*>(a),
                                        forward_iterator<const bool*>(a+N));
        TC_ASSERT_EXPR(v.size() == sz + N);
        TC_ASSERT_EXPR(i == v.begin() + 10);
        std::size_t j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        for (std::size_t k = 0; k < N; ++j, ++k)
            TC_ASSERT_EXPR(v[j] == a[k]);
        for (; j < v.size(); ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        std::vector<bool> v(100);
        while(v.size() < v.capacity()) v.push_back(false);
        v.pop_back(); v.pop_back(); v.pop_back();
        size_t sz = v.size();
        bool a[] = {1, 0, 0, 1, 1};
        const unsigned N = sizeof(a)/sizeof(a[0]);
        std::vector<bool>::iterator i = v.insert(v.cbegin() + 10, forward_iterator<const bool*>(a),
                                        forward_iterator<const bool*>(a+N));
        TC_ASSERT_EXPR(v.size() == sz + N);
        TC_ASSERT_EXPR(i == v.begin() + 10);
        std::size_t j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        for (std::size_t k = 0; k < N; ++j, ++k)
            TC_ASSERT_EXPR(v[j] == a[k]);
        for (; j < v.size(); ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
