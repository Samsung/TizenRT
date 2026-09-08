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

// static int pop_front();

#include <forward_list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "MoveOnly.h"

int tc_libcxx_containers_forwardlist_modifiers_pop_front(void)
{
    {
        typedef int T;
        typedef std::forward_list<T> C;
        typedef std::forward_list<T> C;
        C c;
        c.push_front(1);
        c.push_front(3);
        c.pop_front();
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 1);
        TC_ASSERT_EXPR(c.front() == 1);
        c.pop_front();
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 0);
    }
#if TEST_STD_VER >= 11
    {
        typedef MoveOnly T;
        typedef std::forward_list<T> C;
        C c;
        c.push_front(1);
        c.push_front(3);
        c.pop_front();
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 1);
        TC_ASSERT_EXPR(c.front() == 1);
        c.pop_front();
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 0);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
