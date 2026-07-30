//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_set

// void reserve(size_type n);

#include <unordered_set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class C>
void test(const C& c)
{
    TC_ASSERT_EXPR(c.size() == 4);
    TC_ASSERT_EXPR(c.count(1) == 1);
    TC_ASSERT_EXPR(c.count(2) == 1);
    TC_ASSERT_EXPR(c.count(3) == 1);
    TC_ASSERT_EXPR(c.count(4) == 1);
}

void reserve_invariant(std::size_t n) // LWG #2156
{
    for (std::size_t i = 0; i < n; ++i)
    {
        std::unordered_set<std::size_t> c;
        c.reserve(n);
        std::size_t buckets = c.bucket_count();
        for (std::size_t j = 0; j < i; ++j)
        {
            c.insert(i);
            TC_ASSERT_EXPR(buckets == c.bucket_count());
        }
    }
}

int tc_containers_unord_unord_set_reserve(void) {
    {
        typedef std::unordered_set<int> C;
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
        TC_ASSERT_EXPR(c.bucket_count() >= 5);
        c.reserve(3);
        LIBCPP_ASSERT(c.bucket_count() == 5);
        test(c);
        c.max_load_factor(2);
        c.reserve(3);
        TC_ASSERT_EXPR(c.bucket_count() >= 2);
        test(c);
        c.reserve(31);
        TC_ASSERT_EXPR(c.bucket_count() >= 16);
        test(c);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_set<int, std::hash<int>,
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
        TC_ASSERT_EXPR(c.bucket_count() >= 5);
        c.reserve(3);
        LIBCPP_ASSERT(c.bucket_count() == 5);
        test(c);
        c.max_load_factor(2);
        c.reserve(3);
        TC_ASSERT_EXPR(c.bucket_count() >= 2);
        test(c);
        c.reserve(31);
        TC_ASSERT_EXPR(c.bucket_count() >= 16);
        test(c);
    }
#endif
    reserve_invariant(20);

  return 0;
}
