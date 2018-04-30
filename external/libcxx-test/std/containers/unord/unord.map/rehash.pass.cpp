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

// static int rehash(size_type n);

#include <unordered_map>
#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

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
    TC_ASSERT_EXPR(c.size() == 4);
    TC_ASSERT_EXPR(c.at(1) == "one");
    TC_ASSERT_EXPR(c.at(2) == "two");
    TC_ASSERT_EXPR(c.at(3) == "three");
    TC_ASSERT_EXPR(c.at(4) == "four");
    return 0;
}

int tc_libcxx_containers_unord_map_rehash(void)
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
        test(c);
        TC_ASSERT_EXPR(c.bucket_count() >= 5);
        c.rehash(3);
        rehash_postcondition(c, 3);
        LIBCPP_ASSERT(c.bucket_count() == 5);
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
