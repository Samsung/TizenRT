//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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

#include "test_macros.h"

template <class C>
static int test(const C& c)
{
    TC_ASSERT_EXPR(c.size() == 6);
    TC_ASSERT_EXPR(c.find(1)->second == "one");
    TC_ASSERT_EXPR(next(c.find(1))->second == "four");
    TC_ASSERT_EXPR(c.find(2)->second == "two");
    TC_ASSERT_EXPR(next(c.find(2))->second == "four");
    TC_ASSERT_EXPR(c.find(3)->second == "three");
    TC_ASSERT_EXPR(c.find(4)->second == "four");
    return 0;
}

static int reserve_invariant(size_t n) // LWG #2156
{
    for (size_t i = 0; i < n; ++i)
    {
        std::unordered_multimap<size_t, size_t> c;
        c.reserve(n);
        size_t buckets = c.bucket_count();
        for (size_t j = 0; j < i; ++j)
        {
            c.insert(std::unordered_multimap<size_t, size_t>::value_type(i,i));
            TC_ASSERT_EXPR(buckets == c.bucket_count());
        }
    }
    return 0;
}

int tc_libcxx_containers_unord_multimap_reserve(void)
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
    reserve_invariant(20);
    TC_SUCCESS_RESULT();
    return 0;
}
