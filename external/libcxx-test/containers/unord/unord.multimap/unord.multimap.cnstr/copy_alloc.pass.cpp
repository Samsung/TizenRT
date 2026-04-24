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

// unordered_multimap(const unordered_multimap& u, const allocator_type& a);

#include <unordered_map>
#include <string>
#include <set>
#include <cassert>
#include <cfloat>
#include <cmath>
#include <cstddef>

#include "test_macros.h"
#include "../../../check_consecutive.h"
#include "../../../test_compare.h"
#include "../../../test_hash.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_multimap_unord_multimap_cnstr_copy_alloc(void) {
    {
        typedef std::unordered_multimap<int, std::string,
                                   test_hash<int>,
                                   test_equal_to<int>,
                                   test_allocator<std::pair<const int, std::string> >
                                   > C;
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
        C c0(a, a + sizeof(a)/sizeof(a[0]),
            7,
            test_hash<int>(8),
            test_equal_to<int>(9),
            test_allocator<std::pair<const int, std::string> >(10)
           );
        C c(c0, test_allocator<std::pair<const int, std::string> >(5));
        LIBCPP_ASSERT(c.bucket_count() == 7);
        TC_ASSERT_EXPR(c.size() == 6);
        std::multiset<std::string> s;
        s.insert("one");
        s.insert("four");
        CheckConsecutiveKeys<C::const_iterator>(c.find(1), c.end(), 1, s);
        s.insert("two");
        s.insert("four");
        CheckConsecutiveKeys<C::const_iterator>(c.find(2), c.end(), 2, s);
        s.insert("three");
        CheckConsecutiveKeys<C::const_iterator>(c.find(3), c.end(), 3, s);
        s.insert("four");
        CheckConsecutiveKeys<C::const_iterator>(c.find(4), c.end(), 4, s);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<int>(8));
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<int>(9));
        TC_ASSERT_EXPR(c.get_allocator() ==
               (test_allocator<std::pair<const int, std::string> >(5)));
        TC_ASSERT_EXPR(!c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(std::fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_multimap<int, std::string,
                                   test_hash<int>,
                                   test_equal_to<int>,
                                   min_allocator<std::pair<const int, std::string> >
                                   > C;
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
        C c0(a, a + sizeof(a)/sizeof(a[0]),
            7,
            test_hash<int>(8),
            test_equal_to<int>(9),
            min_allocator<std::pair<const int, std::string> >()
           );
        C c(c0, min_allocator<std::pair<const int, std::string> >());
        LIBCPP_ASSERT(c.bucket_count() == 7);
        TC_ASSERT_EXPR(c.size() == 6);
        std::multiset<std::string> s;
        s.insert("one");
        s.insert("four");
        CheckConsecutiveKeys<C::const_iterator>(c.find(1), c.end(), 1, s);
        s.insert("two");
        s.insert("four");
        CheckConsecutiveKeys<C::const_iterator>(c.find(2), c.end(), 2, s);
        s.insert("three");
        CheckConsecutiveKeys<C::const_iterator>(c.find(3), c.end(), 3, s);
        s.insert("four");
        CheckConsecutiveKeys<C::const_iterator>(c.find(4), c.end(), 4, s);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<int>(8));
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<int>(9));
        TC_ASSERT_EXPR(c.get_allocator() ==
               (min_allocator<std::pair<const int, std::string> >()));
        TC_ASSERT_EXPR(!c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(std::fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
    {
        typedef explicit_allocator<std::pair<const int, std::string>> A;
        typedef std::unordered_multimap<int, std::string,
                                   test_hash<int>,
                                   test_equal_to<int>,
                                   A
                                   > C;
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
        C c0(a, a + sizeof(a)/sizeof(a[0]),
            7,
            test_hash<int>(8),
            test_equal_to<int>(9),
            A{}
           );
        C c(c0, A{});
        LIBCPP_ASSERT(c.bucket_count() == 7);
        TC_ASSERT_EXPR(c.size() == 6);
        std::multiset<std::string> s;
        s.insert("one");
        s.insert("four");
        CheckConsecutiveKeys<C::const_iterator>(c.find(1), c.end(), 1, s);
        s.insert("two");
        s.insert("four");
        CheckConsecutiveKeys<C::const_iterator>(c.find(2), c.end(), 2, s);
        s.insert("three");
        CheckConsecutiveKeys<C::const_iterator>(c.find(3), c.end(), 3, s);
        s.insert("four");
        CheckConsecutiveKeys<C::const_iterator>(c.find(4), c.end(), 4, s);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<int>(8));
        TC_ASSERT_EXPR(c.key_eq() == test_equal_to<int>(9));
        TC_ASSERT_EXPR(c.get_allocator() == A{});
        TC_ASSERT_EXPR(!c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(std::fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
#endif

  return 0;
}
