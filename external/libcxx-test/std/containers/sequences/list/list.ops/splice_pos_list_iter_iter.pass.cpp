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

// <list>

// static int splice(const_iterator position, list& x, iterator first, iterator last);

#include <list>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

int tc_libcxx_containers_list_ops_splice_pos_list_iter_iter(void)
{
    int a1[] = {1, 2, 3};
    int a2[] = {4, 5, 6};
    {
        std::list<int> l1(a1, a1+3);
        l1.splice(l1.begin(), l1, next(l1.begin()), next(l1.begin()));
        TC_ASSERT_EXPR(l1.size() == 3);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 3);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
    }
    {
        std::list<int> l1(a1, a1+3);
        l1.splice(l1.begin(), l1, next(l1.begin()), next(l1.begin(), 2));
        TC_ASSERT_EXPR(l1.size() == 3);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 3);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
    }
    {
        std::list<int> l1(a1, a1+3);
        l1.splice(l1.begin(), l1, next(l1.begin()), next(l1.begin(), 3));
        TC_ASSERT_EXPR(l1.size() == 3);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 3);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
        ++i;
        TC_ASSERT_EXPR(*i == 1);
    }
    {
        std::list<int> l1(a1, a1+3);
        std::list<int> l2(a2, a2+3);
        l1.splice(l1.begin(), l2, next(l2.begin()), l2.end());
        TC_ASSERT_EXPR(l1.size() == 5);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 5);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 5);
        ++i;
        TC_ASSERT_EXPR(*i == 6);
        ++i;
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
        TC_ASSERT_EXPR(l2.size() == 1);
        i = l2.begin();
        TC_ASSERT_EXPR(*i == 4);
    }
    {
        std::list<int> l1(a1, a1+3);
        std::list<int> l2(a2, a2+3);
        l1.splice(next(l1.begin()), l2, next(l2.begin()), l2.end());
        TC_ASSERT_EXPR(l1.size() == 5);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 5);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 5);
        ++i;
        TC_ASSERT_EXPR(*i == 6);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
        TC_ASSERT_EXPR(l2.size() == 1);
        i = l2.begin();
        TC_ASSERT_EXPR(*i == 4);
    }
    {
        std::list<int> l1(a1, a1+3);
        std::list<int> l2(a2, a2+3);
        l1.splice(l1.end(), l2, next(l2.begin()), l2.end());
        TC_ASSERT_EXPR(l1.size() == 5);
        TC_ASSERT_EXPR(distance(l1.begin(), l1.end()) == 5);
        std::list<int>::const_iterator i = l1.begin();
        TC_ASSERT_EXPR(*i == 1);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 3);
        ++i;
        TC_ASSERT_EXPR(*i == 5);
        ++i;
        TC_ASSERT_EXPR(*i == 6);
        TC_ASSERT_EXPR(l2.size() == 1);
        i = l2.begin();
        TC_ASSERT_EXPR(*i == 4);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
