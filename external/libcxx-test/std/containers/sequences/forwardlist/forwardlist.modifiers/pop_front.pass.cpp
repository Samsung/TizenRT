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

// <forward_list>

// static int pop_front();

#include <forward_list>
#include <cassert>
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
