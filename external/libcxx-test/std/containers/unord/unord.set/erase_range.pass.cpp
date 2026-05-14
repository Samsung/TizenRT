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

// iterator erase(const_iterator first, const_iterator last)

#include <unordered_set>
#include <algorithm>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_set_erase_range(void) {
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
        C::const_iterator i = c.find(2);
        C::const_iterator j = std::next(i);
        C::iterator k = c.erase(i, i);
        TC_ASSERT_EXPR(k == i);
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.count(1) == 1);
        TC_ASSERT_EXPR(c.count(2) == 1);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);

        k = c.erase(i, j);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.count(1) == 1);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);

        k = c.erase(c.cbegin(), c.cend());
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(k == c.end());
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_set<int, std::hash<int>, std::equal_to<int>, min_allocator<int>> C;
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
        C::const_iterator i = c.find(2);
        C::const_iterator j = std::next(i);
        C::iterator k = c.erase(i, i);
        TC_ASSERT_EXPR(k == i);
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(c.count(1) == 1);
        TC_ASSERT_EXPR(c.count(2) == 1);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);

        k = c.erase(i, j);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.count(1) == 1);
        TC_ASSERT_EXPR(c.count(3) == 1);
        TC_ASSERT_EXPR(c.count(4) == 1);

        k = c.erase(c.cbegin(), c.cend());
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(k == c.end());
    }
#endif

  return 0;
}
