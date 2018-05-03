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

// <forward_list>

// iterator insert_after(const_iterator p, value_type&& v);

#include <forward_list>
#include <cassert>
#include "libcxx_tc_common.h"

#include "MoveOnly.h"

int tc_libcxx_containers_forwardlist_modifiers_insert_after_rv(void)
{
    {
        typedef MoveOnly T;
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
