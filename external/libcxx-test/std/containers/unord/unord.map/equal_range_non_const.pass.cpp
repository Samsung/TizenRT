//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_map

// pair<iterator, iterator> equal_range(const key_type& k);

#include <unordered_map>
#include <string>
#include <cassert>
#include <iterator>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_map_equal_range_non_const(void) {
    {
        typedef std::unordered_map<int, std::string> C;
        typedef C::iterator I;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(10, "ten"),
            P(20, "twenty"),
            P(30, "thirty"),
            P(40, "forty"),
            P(50, "fifty"),
            P(60, "sixty"),
            P(70, "seventy"),
            P(80, "eighty"),
        };
        C c(std::begin(a), std::end(a));
        std::pair<I, I> r = c.equal_range(30);
        TC_ASSERT_EXPR(std::distance(r.first, r.second) == 1);
        TC_ASSERT_EXPR(r.first->first == 30);
        TC_ASSERT_EXPR(r.first->second == "thirty");
        r = c.equal_range(5);
        TC_ASSERT_EXPR(std::distance(r.first, r.second) == 0);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_map<int, std::string, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, std::string>>> C;
        typedef C::iterator I;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(10, "ten"),
            P(20, "twenty"),
            P(30, "thirty"),
            P(40, "forty"),
            P(50, "fifty"),
            P(60, "sixty"),
            P(70, "seventy"),
            P(80, "eighty"),
        };
        C c(std::begin(a), std::end(a));
        std::pair<I, I> r = c.equal_range(30);
        TC_ASSERT_EXPR(std::distance(r.first, r.second) == 1);
        TC_ASSERT_EXPR(r.first->first == 30);
        TC_ASSERT_EXPR(r.first->second == "thirty");
        r = c.equal_range(5);
        TC_ASSERT_EXPR(std::distance(r.first, r.second) == 0);
    }
#endif

  return 0;
}
