//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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

// iterator insert(const_iterator position, size_type n, const value_type& x);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include <cstddef>


int tc_libcxx_containers_vector_bool_insert_iter_size_value(void)
{
    {
        std::vector<bool> v(100);
        std::vector<bool>::iterator i = v.insert(v.cbegin() + 10, 5, 1);
        TC_ASSERT_EXPR(v.size() == 105);
        TC_ASSERT_EXPR(i == v.begin() + 10);
        std::size_t j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        for (; j < 15; ++j)
            TC_ASSERT_EXPR(v[j] == 1);
        for (++j; j < v.size(); ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        std::vector<bool> v(100);
        while(v.size() < v.capacity()) v.push_back(false);
        size_t sz = v.size();
        std::vector<bool>::iterator i = v.insert(v.cbegin() + 10, 5, 1);
        TC_ASSERT_EXPR(v.size() == sz + 5);
        TC_ASSERT_EXPR(i == v.begin() + 10);
        std::size_t j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        for (; j < 15; ++j)
            TC_ASSERT_EXPR(v[j] == 1);
        for (++j; j < v.size(); ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    {
        std::vector<bool> v(100);
        while(v.size() < v.capacity()) v.push_back(false);
        v.pop_back(); v.pop_back();
        size_t sz = v.size();
        std::vector<bool>::iterator i = v.insert(v.cbegin() + 10, 5, 1);
        TC_ASSERT_EXPR(v.size() == sz + 5);
        TC_ASSERT_EXPR(i == v.begin() + 10);
        std::size_t j;
        for (j = 0; j < 10; ++j)
            TC_ASSERT_EXPR(v[j] == 0);
        for (; j < 15; ++j)
            TC_ASSERT_EXPR(v[j] == 1);
        for (++j; j < v.size(); ++j)
            TC_ASSERT_EXPR(v[j] == 0);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
