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

// static int rehash(size_type n);

#include <unordered_map>
#include <string>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cfloat>
#include <cmath>
#include <cstddef>

#include "test_macros.h"

template <class C>
static int rehash_postcondition(const C& c, size_t n)
{
    TC_ASSERT_EXPR(c.bucket_count() >= c.size() / c.max_load_factor() && c.bucket_count() >= n);
    return 0;
}

template <class C>
static int test(const C& c)
{
    TC_ASSERT_EXPR(c.size() == 6);
    typedef std::pair<typename C::const_iterator, typename C::const_iterator> Eq;
    Eq eq = c.equal_range(1);
    TC_ASSERT_EXPR(std::distance(eq.first, eq.second) == 2);
    typename C::const_iterator i = eq.first;
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
    return 0;
}

int tc_libcxx_containers_unord_multimap_rehash(void)
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
        test(c);
        TC_ASSERT_EXPR(c.bucket_count() >= 7);
        c.rehash(3);
        rehash_postcondition(c, 3);
        LIBCPP_ASSERT(c.bucket_count() == 7);
        test(c);
        c.max_load_factor(2);
        c.rehash(3);
        rehash_postcondition(c, 3);
        LIBCPP_ASSERT(c.bucket_count() == 3);
        test(c);
        c.rehash(31);
        rehash_postcondition(c, 31);
        LIBCPP_ASSERT(c.bucket_count() == 31);
        test(c);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
