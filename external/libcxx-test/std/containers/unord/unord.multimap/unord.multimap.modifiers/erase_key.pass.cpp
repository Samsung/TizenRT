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
// class unordered_multimap

// size_type erase(const key_type& k);

#include <unordered_map>
#include <string>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cstddef>
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

int tc_libcxx_containers_unord_multimap_modifiers_erase_key(void)
{
    {
        typedef std::unordered_multimap<int, std::string> C;
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
        TC_ASSERT_EXPR(c.size() == 6);
        typedef std::pair<C::const_iterator, C::const_iterator> Eq;
        Eq eq = c.equal_range(1);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 2);
        C::const_iterator k = eq.first;
        TC_ASSERT_EXPR(k->first == 1);
        TC_ASSERT_EXPR(k->second == "one");
        ++k;
        TC_ASSERT_EXPR(k->first == 1);
        TC_ASSERT_EXPR(k->second == "four");
        eq = c.equal_range(2);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 2);
        k = eq.first;
        TC_ASSERT_EXPR(k->first == 2);
        TC_ASSERT_EXPR(k->second == "two");
        ++k;
        TC_ASSERT_EXPR(k->first == 2);
        TC_ASSERT_EXPR(k->second == "four");
        eq = c.equal_range(3);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 1);
        k = eq.first;
        TC_ASSERT_EXPR(k->first == 3);
        TC_ASSERT_EXPR(k->second == "three");
        eq = c.equal_range(4);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 1);
        k = eq.first;
        TC_ASSERT_EXPR(k->first == 4);
        TC_ASSERT_EXPR(k->second == "four");
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());

        TC_ASSERT_EXPR(c.erase(2) == 2);
        TC_ASSERT_EXPR(c.size() == 4);
        eq = c.equal_range(1);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 2);
        k = eq.first;
        TC_ASSERT_EXPR(k->first == 1);
        TC_ASSERT_EXPR(k->second == "one");
        ++k;
        TC_ASSERT_EXPR(k->first == 1);
        TC_ASSERT_EXPR(k->second == "four");
        eq = c.equal_range(3);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 1);
        k = eq.first;
        TC_ASSERT_EXPR(k->first == 3);
        TC_ASSERT_EXPR(k->second == "three");
        eq = c.equal_range(4);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 1);
        k = eq.first;
        TC_ASSERT_EXPR(k->first == 4);
        TC_ASSERT_EXPR(k->second == "four");
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());

        TC_ASSERT_EXPR(c.erase(2) == 0);
        TC_ASSERT_EXPR(c.size() == 4);
        eq = c.equal_range(1);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 2);
        k = eq.first;
        TC_ASSERT_EXPR(k->first == 1);
        TC_ASSERT_EXPR(k->second == "one");
        ++k;
        TC_ASSERT_EXPR(k->first == 1);
        TC_ASSERT_EXPR(k->second == "four");
        eq = c.equal_range(3);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 1);
        k = eq.first;
        TC_ASSERT_EXPR(k->first == 3);
        TC_ASSERT_EXPR(k->second == "three");
        eq = c.equal_range(4);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 1);
        k = eq.first;
        TC_ASSERT_EXPR(k->first == 4);
        TC_ASSERT_EXPR(k->second == "four");
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());

        TC_ASSERT_EXPR(c.erase(4) == 1);
        TC_ASSERT_EXPR(c.size() == 3);
        eq = c.equal_range(1);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 2);
        k = eq.first;
        TC_ASSERT_EXPR(k->first == 1);
        TC_ASSERT_EXPR(k->second == "one");
        ++k;
        TC_ASSERT_EXPR(k->first == 1);
        TC_ASSERT_EXPR(k->second == "four");
        eq = c.equal_range(3);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 1);
        k = eq.first;
        TC_ASSERT_EXPR(k->first == 3);
        TC_ASSERT_EXPR(k->second == "three");
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());

        TC_ASSERT_EXPR(c.erase(4) == 0);
        TC_ASSERT_EXPR(c.size() == 3);
        eq = c.equal_range(1);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 2);
        k = eq.first;
        TC_ASSERT_EXPR(k->first == 1);
        TC_ASSERT_EXPR(k->second == "one");
        ++k;
        TC_ASSERT_EXPR(k->first == 1);
        TC_ASSERT_EXPR(k->second == "four");
        eq = c.equal_range(3);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 1);
        k = eq.first;
        TC_ASSERT_EXPR(k->first == 3);
        TC_ASSERT_EXPR(k->second == "three");
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());

        TC_ASSERT_EXPR(c.erase(1) == 2);
        TC_ASSERT_EXPR(c.size() == 1);
        eq = c.equal_range(3);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 1);
        k = eq.first;
        TC_ASSERT_EXPR(k->first == 3);
        TC_ASSERT_EXPR(k->second == "three");
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());

        TC_ASSERT_EXPR(c.erase(1) == 0);
        TC_ASSERT_EXPR(c.size() == 1);
        eq = c.equal_range(3);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 1);
        k = eq.first;
        TC_ASSERT_EXPR(k->first == 3);
        TC_ASSERT_EXPR(k->second == "three");
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());

        TC_ASSERT_EXPR(c.erase(3) == 1);
        TC_ASSERT_EXPR(c.size() == 0);
        eq = c.equal_range(3);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 0);
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());

        TC_ASSERT_EXPR(c.erase(3) == 0);
        TC_ASSERT_EXPR(c.size() == 0);
        eq = c.equal_range(3);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 0);
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
    }
#if TEST_STD_VER >= 11
    {
    typedef std::unordered_multimap<int, int> C;
    C m, m2;
    for ( int i = 0; i < 10; ++i ) {
        for (int j = 0; j < 2; ++j ) {
            m.insert (std::make_pair(i,j));
            m2.insert(std::make_pair(i,j));
            }
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
