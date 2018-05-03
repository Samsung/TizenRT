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

// UNSUPPORTED: c++98, c++03

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_map

// unordered_map(initializer_list<value_type> il);

#include <unordered_map>
#include <string>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cfloat>
#include <cstddef>

#include "test_macros.h"
#include "test_compare.h"
#include "test_hash.h"
#include "test_allocator.h"

int tc_libcxx_containers_unord_map_cnstr_init(void)
{
    {
        typedef std::unordered_map<int, std::string,
                                   test_hash<std::hash<int> >,
                                   test_compare<std::equal_to<int> >,
                                   test_allocator<std::pair<const int, std::string> >
                                   > C;
        typedef std::pair<int, std::string> P;
        C c = {
                P(1, "one"),
                P(2, "two"),
                P(3, "three"),
                P(4, "four"),
                P(1, "four"),
                P(2, "four"),
              };
        TC_ASSERT_EXPR(c.bucket_count() >= 5);
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.at(1) == "one");
        TC_ASSERT_EXPR(c.at(2) == "two");
        TC_ASSERT_EXPR(c.at(3) == "three");
        TC_ASSERT_EXPR(c.at(4) == "four");
        TC_ASSERT_EXPR(c.hash_function() == test_hash<std::hash<int> >());
        TC_ASSERT_EXPR(c.key_eq() == test_compare<std::equal_to<int> >());
        TC_ASSERT_EXPR(c.get_allocator() ==
               (test_allocator<std::pair<const int, std::string> >()));
        TC_ASSERT_EXPR(!c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
#if TEST_STD_VER > 11
    {
        typedef std::pair<int, std::string> P;
        typedef test_allocator<std::pair<const int, std::string>> A;
        typedef test_hash<std::hash<int>> HF;
        typedef test_compare<std::equal_to<int>> Comp;
        typedef std::unordered_map<int, std::string, HF, Comp, A> C;

        A a(42);
        C c ( {
                P(1, "one"),
                P(2, "two"),
                P(3, "three"),
                P(4, "four"),
                P(1, "four"),
                P(2, "four"),
              }, 12, a);
        TC_ASSERT_EXPR(c.bucket_count() >= 12);
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.at(1) == "one");
        TC_ASSERT_EXPR(c.at(2) == "two");
        TC_ASSERT_EXPR(c.at(3) == "three");
        TC_ASSERT_EXPR(c.at(4) == "four");
        TC_ASSERT_EXPR(c.hash_function() == test_hash<std::hash<int> >());
        TC_ASSERT_EXPR(c.key_eq() == test_compare<std::equal_to<int> >());
        TC_ASSERT_EXPR(c.get_allocator() == a);
        TC_ASSERT_EXPR(!c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
    {
        typedef std::pair<int, std::string> P;
        typedef test_allocator<std::pair<const int, std::string>> A;
        typedef test_hash<std::hash<int>> HF;
        typedef test_compare<std::equal_to<int>> Comp;
        typedef std::unordered_map<int, std::string, HF, Comp, A> C;

        HF hf(42);
        A a(43);
        C c ( {
                P(1, "one"),
                P(2, "two"),
                P(3, "three"),
                P(4, "four"),
                P(1, "four"),
                P(2, "four"),
              }, 12, hf, a);
        TC_ASSERT_EXPR(c.bucket_count() >= 12);
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.at(1) == "one");
        TC_ASSERT_EXPR(c.at(2) == "two");
        TC_ASSERT_EXPR(c.at(3) == "three");
        TC_ASSERT_EXPR(c.at(4) == "four");
        TC_ASSERT_EXPR(c.hash_function() == hf);
        TC_ASSERT_EXPR(!(c.hash_function() == test_hash<std::hash<int> >()));
        TC_ASSERT_EXPR(c.key_eq() == test_compare<std::equal_to<int> >());
        TC_ASSERT_EXPR(c.get_allocator() == a);
        TC_ASSERT_EXPR(!c.empty());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
#endif // TEST_STD_VER > 11
    TC_SUCCESS_RESULT();
    return 0;
}
