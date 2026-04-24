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

// void reserve(size_type n);

#include <unordered_map>
#include <string>
#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class C>
void test(const C& c)
{
    TC_ASSERT_EXPR(c.size() == 6);
    {
        std::set<std::string> s;
        s.insert("one");
        s.insert("four");
        TC_ASSERT_EXPR(s.find(c.find(1)->second) != s.end());
        s.erase(s.find(c.find(1)->second));
        TC_ASSERT_EXPR(s.find(std::next(c.find(1))->second) != s.end());
    }
    {
        std::set<std::string> s;
        s.insert("two");
        s.insert("four");
        TC_ASSERT_EXPR(s.find(c.find(2)->second) != s.end());
        s.erase(s.find(c.find(2)->second));
        TC_ASSERT_EXPR(s.find(std::next(c.find(2))->second) != s.end());
    }
    TC_ASSERT_EXPR(c.find(3)->second == "three");
    TC_ASSERT_EXPR(c.find(4)->second == "four");
}

void reserve_invariant(std::size_t n) // LWG #2156
{
    for (std::size_t i = 0; i < n; ++i)
    {
        std::unordered_multimap<std::size_t, size_t> c;
        c.reserve(n);
        std::size_t buckets = c.bucket_count();
        for (std::size_t j = 0; j < i; ++j)
        {
            c.insert(std::unordered_multimap<std::size_t, size_t>::value_type(i,i));
            TC_ASSERT_EXPR(buckets == c.bucket_count());
        }
    }
}

int tc_containers_unord_unord_multimap_reserve(void) {
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
        test(c);
        TC_ASSERT_EXPR(c.bucket_count() >= 7);
        c.reserve(3);
        LIBCPP_ASSERT(c.bucket_count() == 7);
        test(c);
        c.max_load_factor(2);
        c.reserve(3);
        LIBCPP_ASSERT(c.bucket_count() == 3);
        test(c);
        c.reserve(31);
        TC_ASSERT_EXPR(c.bucket_count() >= 16);
        test(c);
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
        test(c);
        TC_ASSERT_EXPR(c.bucket_count() >= 7);
        c.reserve(3);
        LIBCPP_ASSERT(c.bucket_count() == 7);
        test(c);
        c.max_load_factor(2);
        c.reserve(3);
        LIBCPP_ASSERT(c.bucket_count() == 3);
        test(c);
        c.reserve(31);
        TC_ASSERT_EXPR(c.bucket_count() >= 16);
        test(c);
    }
#endif
    reserve_invariant(20);

  return 0;
}
