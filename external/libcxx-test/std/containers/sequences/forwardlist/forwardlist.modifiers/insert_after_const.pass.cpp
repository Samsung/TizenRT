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

// <forward_list>

// iterator insert_after(const_iterator p, const value_type& v);

#include <forward_list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


int tc_libcxx_containers_forwardlist_modifiers_insert_after_const(void)
{
    {
        typedef int T;
        typedef std::forward_list<T> C;
        typedef C::iterator I;
        C c;
        I i = c.insert_after(c.cbefore_begin(), 0);
        TC_ASSERT_EXPR(i == c.begin());
        TC_ASSERT_EXPR(c.front() == 0);
        TC_ASSERT_EXPR(c.front() == 0);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 1);

        i = c.insert_after(c.cbegin(), 1);
        TC_ASSERT_EXPR(i == next(c.begin()));
        TC_ASSERT_EXPR(c.front() == 0);
        TC_ASSERT_EXPR(*next(c.begin()) == 1);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 2);

        i = c.insert_after(next(c.cbegin()), 2);
        TC_ASSERT_EXPR(i == next(c.begin(), 2));
        TC_ASSERT_EXPR(c.front() == 0);
        TC_ASSERT_EXPR(*next(c.begin()) == 1);
        TC_ASSERT_EXPR(*next(c.begin(), 2) == 2);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 3);

        i = c.insert_after(c.cbegin(), 3);
        TC_ASSERT_EXPR(i == next(c.begin()));
        TC_ASSERT_EXPR(c.front() == 0);
        TC_ASSERT_EXPR(*next(c.begin(), 1) == 3);
        TC_ASSERT_EXPR(*next(c.begin(), 2) == 1);
        TC_ASSERT_EXPR(*next(c.begin(), 3) == 2);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 4);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
