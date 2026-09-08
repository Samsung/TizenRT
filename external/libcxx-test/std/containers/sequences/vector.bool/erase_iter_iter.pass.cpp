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

// iterator erase(const_iterator first, const_iterator last);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


int tc_libcxx_containers_vector_bool_erase_iter_iter(void)
{
    bool a1[] = {1, 0, 1};
    {
        std::vector<bool> l1(a1, a1+3);
        std::vector<bool>::iterator i = l1.erase(l1.cbegin(), l1.cbegin());
        TC_ASSERT_EXPR(l1.size() == 3);
        TC_ASSERT_EXPR(distance(l1.cbegin(), l1.cend()) == 3);
        TC_ASSERT_EXPR(i == l1.begin());
    }
    {
        std::vector<bool> l1(a1, a1+3);
        std::vector<bool>::iterator i = l1.erase(l1.cbegin(), next(l1.cbegin()));
        TC_ASSERT_EXPR(l1.size() == 2);
        TC_ASSERT_EXPR(distance(l1.cbegin(), l1.cend()) == 2);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR(l1 == std::vector<bool>(a1+1, a1+3));
    }
    {
        std::vector<bool> l1(a1, a1+3);
        std::vector<bool>::iterator i = l1.erase(l1.cbegin(), next(l1.cbegin(), 2));
        TC_ASSERT_EXPR(l1.size() == 1);
        TC_ASSERT_EXPR(distance(l1.cbegin(), l1.cend()) == 1);
        TC_ASSERT_EXPR(i == l1.begin());
        TC_ASSERT_EXPR(l1 == std::vector<bool>(a1+2, a1+3));
    }
    {
        std::vector<bool> l1(a1, a1+3);
        std::vector<bool>::iterator i = l1.erase(l1.cbegin(), next(l1.cbegin(), 3));
        TC_ASSERT_EXPR(l1.size() == 0);
        TC_ASSERT_EXPR(distance(l1.cbegin(), l1.cend()) == 0);
        TC_ASSERT_EXPR(i == l1.begin());
    }
    TC_SUCCESS_RESULT();
    return 0;
}
