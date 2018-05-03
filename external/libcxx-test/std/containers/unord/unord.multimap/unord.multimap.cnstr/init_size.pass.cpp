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
// class unordered_multimap

// unordered_multimap(initializer_list<value_type> il, size_type n);

#include <unordered_map>
#include <string>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cfloat>
#include <cmath>
#include <cstddef>

#include "test_macros.h"
#include "test_compare.h"
#include "test_hash.h"
#include "test_allocator.h"

int tc_libcxx_containers_unord_multimap_cnstr_init_size(void)
{
    {
        typedef std::unordered_multimap<int, std::string,
                                   test_hash<std::hash<int> >,
                                   test_compare<std::equal_to<int> >,
                                   test_allocator<std::pair<const int, std::string> >
                                   > C;
        typedef std::pair<int, std::string> P;
        C c({
                P(1, "one"),
                P(2, "two"),
                P(3, "three"),
                P(4, "four"),
                P(1, "four"),
                P(2, "four"),
            },
            7
           );
        LIBCPP_ASSERT(c.bucket_count() == 7);
        TC_ASSERT_EXPR(c.size() == 6);
        typedef std::pair<C::const_iterator, C::const_iterator> Eq;
        Eq eq = c.equal_range(1);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 2);
        C::const_iterator i = eq.first;
        TC_ASSERT_EXPR(i->first == 1);
        TC_ASSERT_EXPR(i->second == "one");
        ++i;
        TC_ASSERT_EXPR(i->first == 1);
        TC_ASSERT_EXPR(i->second == "four");
        eq = c.equal_range(2);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 2);
        i = eq.first;
        TC_ASSERT_EXPR(i->first == 2);
        TC_ASSERT_EXPR(i->second == "two");
        ++i;
        TC_ASSERT_EXPR(i->first == 2);
        TC_ASSERT_EXPR(i->second == "four");

        eq = c.equal_range(3);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 1);
        i = eq.first;
        TC_ASSERT_EXPR(i->first == 3);
        TC_ASSERT_EXPR(i->second == "three");
        eq = c.equal_range(4);
        TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 1);
        i = eq.first;
        TC_ASSERT_EXPR(i->first == 4);
        TC_ASSERT_EXPR(i->second == "four");
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.begin(), c.end())) == c.size());
        TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c.cbegin(), c.cend())) == c.size());
        TC_ASSERT_EXPR(std::fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
        TC_ASSERT_EXPR(c.hash_function() == test_hash<std::hash<int> >());
        TC_ASSERT_EXPR(c.key_eq() == test_compare<std::equal_to<int> >());
        TC_ASSERT_EXPR((c.get_allocator() == test_allocator<std::pair<const int, std::string> >()));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
