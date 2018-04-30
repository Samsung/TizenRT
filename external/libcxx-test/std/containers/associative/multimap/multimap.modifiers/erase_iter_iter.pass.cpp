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

// <map>

// class multimap

// iterator erase(const_iterator first, const_iterator last);

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_libcxx_containers_multimap_modifiers_erase_iter_iter(void)
{
    {
        typedef std::multimap<int, double> M;
        typedef std::pair<int, double> P;
        typedef M::iterator I;
        P ar[] =
        {
            P(1, 1.5),
            P(2, 2.5),
            P(3, 3.5),
            P(4, 4.5),
            P(5, 5.5),
            P(6, 6.5),
            P(7, 7.5),
            P(8, 8.5),
        };
        M m(ar, ar + sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(m.size() == 8);
        I i = m.erase(m.cbegin(), m.cbegin());
        TC_ASSERT_EXPR(m.size() == 8);
        TC_ASSERT_EXPR(i == m.begin());
        TC_ASSERT_EXPR(m.begin()->first == 1);
        TC_ASSERT_EXPR(m.begin()->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin())->first == 2);
        TC_ASSERT_EXPR(next(m.begin())->second == 2.5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 3.5);
        TC_ASSERT_EXPR(next(m.begin(), 3)->first == 4);
        TC_ASSERT_EXPR(next(m.begin(), 3)->second == 4.5);
        TC_ASSERT_EXPR(next(m.begin(), 4)->first == 5);
        TC_ASSERT_EXPR(next(m.begin(), 4)->second == 5.5);
        TC_ASSERT_EXPR(next(m.begin(), 5)->first == 6);
        TC_ASSERT_EXPR(next(m.begin(), 5)->second == 6.5);
        TC_ASSERT_EXPR(next(m.begin(), 6)->first == 7);
        TC_ASSERT_EXPR(next(m.begin(), 6)->second == 7.5);
        TC_ASSERT_EXPR(next(m.begin(), 7)->first == 8);
        TC_ASSERT_EXPR(next(m.begin(), 7)->second == 8.5);

        i = m.erase(m.cbegin(), next(m.cbegin(), 2));
        TC_ASSERT_EXPR(m.size() == 6);
        TC_ASSERT_EXPR(i == m.begin());
        TC_ASSERT_EXPR(next(m.begin(), 0)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 0)->second == 3.5);
        TC_ASSERT_EXPR(next(m.begin(), 1)->first == 4);
        TC_ASSERT_EXPR(next(m.begin(), 1)->second == 4.5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 5.5);
        TC_ASSERT_EXPR(next(m.begin(), 3)->first == 6);
        TC_ASSERT_EXPR(next(m.begin(), 3)->second == 6.5);
        TC_ASSERT_EXPR(next(m.begin(), 4)->first == 7);
        TC_ASSERT_EXPR(next(m.begin(), 4)->second == 7.5);
        TC_ASSERT_EXPR(next(m.begin(), 5)->first == 8);
        TC_ASSERT_EXPR(next(m.begin(), 5)->second == 8.5);

        i = m.erase(next(m.cbegin(), 2), next(m.cbegin(), 6));
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(i == next(m.begin(), 2));
        TC_ASSERT_EXPR(next(m.begin(), 0)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 0)->second == 3.5);
        TC_ASSERT_EXPR(next(m.begin(), 1)->first == 4);
        TC_ASSERT_EXPR(next(m.begin(), 1)->second == 4.5);

        i = m.erase(m.cbegin(), m.cend());
        TC_ASSERT_EXPR(m.size() == 0);
        TC_ASSERT_EXPR(i == m.begin());
        TC_ASSERT_EXPR(i == m.end());
    }
    TC_SUCCESS_RESULT();
    return 0;
}
