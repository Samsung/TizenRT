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
// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_multimap

// iterator erase(const_iterator first, const_iterator last)

#include <unordered_map>
#include <string>
#include <set>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "../../../check_consecutive.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_multimap_unord_multimap_modifiers_erase_range(void) {
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
        C::const_iterator i = c.find(2);
        C::const_iterator j = std::next(i, 2);
        C::iterator k = c.erase(i, i);
        TC_ASSERT_EXPR(k == i);
        TC_ASSERT_EXPR(c.size() == 6);
        typedef std::pair<C::iterator, C::iterator> Eq;
        Eq eq = c.equal_range(1);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 2);
        std::multiset<std::string> s;
        s.insert("one");
        s.insert("four");
        CheckConsecutiveKeys<C::const_iterator>(c.find(1), c.end(), 1, s);
        eq = c.equal_range(2);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 2);
        s.insert("two");
        s.insert("four");
        CheckConsecutiveKeys<C::const_iterator>(c.find(2), c.end(), 2, s);
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

        k = c.erase(i, j);
        TC_ASSERT_EXPR(c.size() == 4);
        eq = c.equal_range(1);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 2);
        s.insert("one");
        s.insert("four");
        CheckConsecutiveKeys<C::const_iterator>(c.find(1), c.end(), 1, s);
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

        k = c.erase(c.cbegin(), c.cend());
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(k == c.end());
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_multimap<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
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
        C::const_iterator i = c.find(2);
        C::const_iterator j = std::next(i, 2);
        C::iterator k = c.erase(i, i);
        TC_ASSERT_EXPR(k == i);
        TC_ASSERT_EXPR(c.size() == 6);
        typedef std::pair<C::iterator, C::iterator> Eq;
        Eq eq = c.equal_range(1);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 2);
        std::multiset<std::string> s;
        s.insert("one");
        s.insert("four");
        CheckConsecutiveKeys<C::const_iterator>(c.find(1), c.end(), 1, s);
        eq = c.equal_range(2);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 2);
        s.insert("two");
        s.insert("four");
        CheckConsecutiveKeys<C::const_iterator>(c.find(2), c.end(), 2, s);
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

        k = c.erase(i, j);
        TC_ASSERT_EXPR(c.size() == 4);
        eq = c.equal_range(1);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 2);
        s.insert("one");
        s.insert("four");
        CheckConsecutiveKeys<C::const_iterator>(c.find(1), c.end(), 1, s);
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

        k = c.erase(c.cbegin(), c.cend());
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(k == c.end());
    }
#endif

  return 0;
}
