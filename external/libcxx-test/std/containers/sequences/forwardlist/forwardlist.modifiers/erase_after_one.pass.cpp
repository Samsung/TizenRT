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

// iterator erase_after(const_iterator p);

#include <forward_list>
#include <cassert>
#include "libcxx_tc_common.h"


int tc_libcxx_containers_forwardlist_modifiers_erase_after_one(void)
{
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t[] = {0, 1, 2, 3, 4};
        C c(std::begin(t), std::end(t));

        C::iterator i = c.erase_after(next(c.cbefore_begin(), 4));
        TC_ASSERT_EXPR(i == c.end());
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 4);
        TC_ASSERT_EXPR(*next(c.begin(), 0) == 0);
        TC_ASSERT_EXPR(*next(c.begin(), 1) == 1);
        TC_ASSERT_EXPR(*next(c.begin(), 2) == 2);
        TC_ASSERT_EXPR(*next(c.begin(), 3) == 3);

        i = c.erase_after(next(c.cbefore_begin(), 0));
        TC_ASSERT_EXPR(i == c.begin());
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 3);
        TC_ASSERT_EXPR(*next(c.begin(), 0) == 1);
        TC_ASSERT_EXPR(*next(c.begin(), 1) == 2);
        TC_ASSERT_EXPR(*next(c.begin(), 2) == 3);

        i = c.erase_after(next(c.cbefore_begin(), 1));
        TC_ASSERT_EXPR(i == next(c.begin()));
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 2);
        TC_ASSERT_EXPR(*next(c.begin(), 0) == 1);
        TC_ASSERT_EXPR(*next(c.begin(), 1) == 3);

        i = c.erase_after(next(c.cbefore_begin(), 1));
        TC_ASSERT_EXPR(i == c.end());
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 1);
        TC_ASSERT_EXPR(*next(c.begin(), 0) == 1);

        i = c.erase_after(next(c.cbefore_begin(), 0));
        TC_ASSERT_EXPR(i == c.begin());
        TC_ASSERT_EXPR(i == c.end());
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 0);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
