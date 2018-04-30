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

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_map

// size_type erase(const key_type& k);

#include <unordered_map>
#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

#if TEST_STD_VER >= 11
template <typename Unordered>
bool only_deletions ( const Unordered &whole, const Unordered &part ) {
    typename Unordered::const_iterator w = whole.begin();
    typename Unordered::const_iterator p = part.begin();

    while ( w != whole.end () && p != part.end()) {
        if ( *w == *p )
            p++;
        w++;
        }

    return p == part.end();
    return 0;
}
#endif


int tc_libcxx_containers_unord_map_modifiers_erase_key(void)
{
    {
        typedef std::unordered_map<int, std::string> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        C c(a, a + sizeof(a)/sizeof(a[0]));
        TC_ASSERT_EXPR(c.erase(5) == 0);
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.at(1) == "one");
        TC_ASSERT_EXPR(c.at(2) == "two");
        TC_ASSERT_EXPR(c.at(3) == "three");
        TC_ASSERT_EXPR(c.at(4) == "four");

        TC_ASSERT_EXPR(c.erase(2) == 1);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.at(1) == "one");
        TC_ASSERT_EXPR(c.at(3) == "three");
        TC_ASSERT_EXPR(c.at(4) == "four");

        TC_ASSERT_EXPR(c.erase(2) == 0);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.at(1) == "one");
        TC_ASSERT_EXPR(c.at(3) == "three");
        TC_ASSERT_EXPR(c.at(4) == "four");

        TC_ASSERT_EXPR(c.erase(4) == 1);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(c.at(1) == "one");
        TC_ASSERT_EXPR(c.at(3) == "three");

        TC_ASSERT_EXPR(c.erase(4) == 0);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(c.at(1) == "one");
        TC_ASSERT_EXPR(c.at(3) == "three");

        TC_ASSERT_EXPR(c.erase(1) == 1);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(c.at(3) == "three");

        TC_ASSERT_EXPR(c.erase(1) == 0);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(c.at(3) == "three");

        TC_ASSERT_EXPR(c.erase(3) == 1);
        TC_ASSERT_EXPR(c.size() == 0);

        TC_ASSERT_EXPR(c.erase(3) == 0);
        TC_ASSERT_EXPR(c.size() == 0);
    }
#if TEST_STD_VER >= 11
    {
    typedef std::unordered_map<int, int> C;
    C m, m2;
    for ( int i = 0; i < 10; ++i ) {
        m[i] = i;
        m2[i] = i;
        }

    C::iterator i = m2.begin();
    int ctr = 0;
    while (i != m2.end()) {
        if (ctr++ % 2 == 0)
            m2.erase(i++);
        else
            ++i;
        }

    TC_ASSERT_EXPR (only_deletions (m, m2));
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
