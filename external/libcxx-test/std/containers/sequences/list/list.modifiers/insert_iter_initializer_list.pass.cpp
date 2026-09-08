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

// UNSUPPORTED: c++98, c++03

// <list>

// iterator insert(const_iterator p, initializer_list<value_type> il);

#include <list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


int tc_libcxx_containers_list_modifiers_insert_iter_initializer_list(void)
{
    {
    std::list<int> d(10, 1);
    std::list<int>::iterator i = d.insert(next(d.cbegin(), 2), {3, 4, 5, 6});
    TC_ASSERT_EXPR(d.size() == 14);
    TC_ASSERT_EXPR(i == next(d.begin(), 2));
    i = d.begin();
    TC_ASSERT_EXPR(*i++ == 1);
    TC_ASSERT_EXPR(*i++ == 1);
    TC_ASSERT_EXPR(*i++ == 3);
    TC_ASSERT_EXPR(*i++ == 4);
    TC_ASSERT_EXPR(*i++ == 5);
    TC_ASSERT_EXPR(*i++ == 6);
    TC_ASSERT_EXPR(*i++ == 1);
    TC_ASSERT_EXPR(*i++ == 1);
    TC_ASSERT_EXPR(*i++ == 1);
    TC_ASSERT_EXPR(*i++ == 1);
    TC_ASSERT_EXPR(*i++ == 1);
    TC_ASSERT_EXPR(*i++ == 1);
    TC_ASSERT_EXPR(*i++ == 1);
    TC_ASSERT_EXPR(*i++ == 1);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
