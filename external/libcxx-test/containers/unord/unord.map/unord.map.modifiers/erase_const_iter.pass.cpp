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
// class unordered_map

// iterator erase(const_iterator p)

#include <unordered_map>
#include <string>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

struct TemplateConstructor
{
    template<typename T>
    TemplateConstructor (const T&) {}
};

bool operator==(const TemplateConstructor&, const TemplateConstructor&) { return false; }
struct Hash { std::size_t operator() (const TemplateConstructor &) const { return 0; } };

int tc_containers_unord_unord_map_unord_map_modifiers_erase_const_iter(void) {
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
        C::const_iterator i = c.find(2);
        c.erase(i);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.at(1) == "one");
        TC_ASSERT_EXPR(c.at(3) == "three");
        TC_ASSERT_EXPR(c.at(4) == "four");
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>,
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
        c.erase(i);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.at(1) == "one");
        TC_ASSERT_EXPR(c.at(3) == "three");
        TC_ASSERT_EXPR(c.at(4) == "four");
    }
#endif
#if TEST_STD_VER >= 14
    {
    //  This is LWG #2059
        typedef TemplateConstructor T;
        typedef std::unordered_map<T, int, Hash> C;
        typedef C::iterator I;

        C m;
        T a{0};
        I it = m.find(a);
        if (it != m.end())
            m.erase(it);
    }
#endif

  return 0;
}
