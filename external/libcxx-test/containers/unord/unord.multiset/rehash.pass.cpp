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
// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_multiset

// void rehash(size_type n);

#include <unordered_set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class C>
void rehash_postcondition(const C& c, std::size_t n)
{
    TC_ASSERT_EXPR(c.bucket_count() >= c.size() / c.max_load_factor() && c.bucket_count() >= n);
}

template <class C>
void test(const C& c)
{
    TC_ASSERT_EXPR(c.size() == 6);
    TC_ASSERT_EXPR(c.count(1) == 2);
    TC_ASSERT_EXPR(c.count(2) == 2);
    TC_ASSERT_EXPR(c.count(3) == 1);
    TC_ASSERT_EXPR(c.count(4) == 1);
}

int tc_containers_unord_unord_multiset_rehash(void) {
    {
        typedef std::unordered_multiset<int> C;
        typedef int P;
        P a[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
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
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_multiset<int, std::hash<int>,
                                      std::equal_to<int>, min_allocator<int>> C;
        typedef int P;
        P a[] =
        {
            P(1),
            P(2),
            P(3),
            P(4),
            P(1),
            P(2)
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
#endif

  return 0;
}
