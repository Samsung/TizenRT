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

// iterator erase(const_iterator position);

#include <map>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_containers_multimap_modifiers_erase_iter(void)
{
    {
        typedef std::multimap<int, double> M;
        typedef std::pair<int, double> P;
        typedef M::iterator I;
        P ar[] =
        {
            P(1, 1),
            P(1, 1.5),
            P(1, 2),
            P(2, 1),
            P(2, 1.5),
            P(2, 2),
            P(3, 1),
            P(3, 1.5),
            P(3, 2),
        };
        M m(ar, ar + sizeof(ar)/sizeof(ar[0]));
        TC_ASSERT_EXPR(m.size() == 9);
        I i = m.erase(next(m.cbegin(), 3));
        TC_ASSERT_EXPR(m.size() == 8);
        TC_ASSERT_EXPR(i == next(m.begin(), 3));
        TC_ASSERT_EXPR(m.begin()->first == 1);
        TC_ASSERT_EXPR(m.begin()->second == 1);
        TC_ASSERT_EXPR(next(m.begin())->first == 1);
        TC_ASSERT_EXPR(next(m.begin())->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 2);
        TC_ASSERT_EXPR(next(m.begin(), 3)->first == 2);
        TC_ASSERT_EXPR(next(m.begin(), 3)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 4)->first == 2);
        TC_ASSERT_EXPR(next(m.begin(), 4)->second == 2);
        TC_ASSERT_EXPR(next(m.begin(), 5)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 5)->second == 1);
        TC_ASSERT_EXPR(next(m.begin(), 6)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 6)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 7)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 7)->second == 2);

        i = m.erase(next(m.cbegin(), 0));
        TC_ASSERT_EXPR(m.size() == 7);
        TC_ASSERT_EXPR(i == m.begin());
        TC_ASSERT_EXPR(next(m.begin(), 0)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 0)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 1)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 1)->second == 2);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 2);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 3)->first == 2);
        TC_ASSERT_EXPR(next(m.begin(), 3)->second == 2);
        TC_ASSERT_EXPR(next(m.begin(), 4)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 4)->second == 1);
        TC_ASSERT_EXPR(next(m.begin(), 5)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 5)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 6)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 6)->second == 2);

        i = m.erase(next(m.cbegin(), 5));
        TC_ASSERT_EXPR(m.size() == 6);
        TC_ASSERT_EXPR(i == prev(m.end()));
        TC_ASSERT_EXPR(next(m.begin(), 0)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 0)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 1)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 1)->second == 2);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 2);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 3)->first == 2);
        TC_ASSERT_EXPR(next(m.begin(), 3)->second == 2);
        TC_ASSERT_EXPR(next(m.begin(), 4)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 4)->second == 1);
        TC_ASSERT_EXPR(next(m.begin(), 5)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 5)->second == 2);

        i = m.erase(next(m.cbegin(), 1));
        TC_ASSERT_EXPR(m.size() == 5);
        TC_ASSERT_EXPR(i == next(m.begin()));
        TC_ASSERT_EXPR(next(m.begin(), 0)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 0)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 1)->first == 2);
        TC_ASSERT_EXPR(next(m.begin(), 1)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 2);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 2);
        TC_ASSERT_EXPR(next(m.begin(), 3)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 3)->second == 1);
        TC_ASSERT_EXPR(next(m.begin(), 4)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 4)->second == 2);

        i = m.erase(next(m.cbegin(), 2));
        TC_ASSERT_EXPR(m.size() == 4);
        TC_ASSERT_EXPR(i == next(m.begin(), 2));
        TC_ASSERT_EXPR(next(m.begin(), 0)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 0)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 1)->first == 2);
        TC_ASSERT_EXPR(next(m.begin(), 1)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 1);
        TC_ASSERT_EXPR(next(m.begin(), 3)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 3)->second == 2);

        i = m.erase(next(m.cbegin(), 2));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(i == next(m.begin(), 2));
        TC_ASSERT_EXPR(next(m.begin(), 0)->first == 1);
        TC_ASSERT_EXPR(next(m.begin(), 0)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 1)->first == 2);
        TC_ASSERT_EXPR(next(m.begin(), 1)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 2)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 2)->second == 2);

        i = m.erase(next(m.cbegin(), 0));
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(i == next(m.begin(), 0));
        TC_ASSERT_EXPR(next(m.begin(), 0)->first == 2);
        TC_ASSERT_EXPR(next(m.begin(), 0)->second == 1.5);
        TC_ASSERT_EXPR(next(m.begin(), 1)->first == 3);
        TC_ASSERT_EXPR(next(m.begin(), 1)->second == 2);

        i = m.erase(next(m.cbegin(), 1));
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(i == m.end());
        TC_ASSERT_EXPR(next(m.begin(), 0)->first == 2);
        TC_ASSERT_EXPR(next(m.begin(), 0)->second == 1.5);

        i = m.erase(m.cbegin());
        TC_ASSERT_EXPR(m.size() == 0);
        TC_ASSERT_EXPR(i == m.begin());
        TC_ASSERT_EXPR(i == m.end());
    }
#if TEST_STD_VER >= 14
    {
    //  This is LWG #2059
        typedef TemplateConstructor T;
        typedef std::multimap<T, int> C;
        typedef C::iterator I;

        C c;
        T a{0};
        I it = c.find(a);
        if (it != c.end())
            c.erase(it);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
