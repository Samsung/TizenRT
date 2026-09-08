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

// static int push_front(const value_type& v);

#include <forward_list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


int tc_libcxx_containers_forwardlist_modifiers_push_front_const(void)
{
    {
        typedef int T;
        typedef std::forward_list<T> C;
        C c;
        c.push_front(1);
        TC_ASSERT_EXPR(c.front() == 1);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 1);
        c.push_front(3);
        TC_ASSERT_EXPR(c.front() == 3);
        TC_ASSERT_EXPR(*next(c.begin()) == 1);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 2);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
