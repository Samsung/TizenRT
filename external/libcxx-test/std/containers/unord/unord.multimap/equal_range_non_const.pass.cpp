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

// pair<iterator, iterator> equal_range(const key_type& k);

#include <unordered_map>
#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


int tc_libcxx_containers_unord_multimap_equal_range_non_const(void)
{
    {
        typedef std::unordered_multimap<int, std::string> C;
        typedef C::iterator I;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(10, "ten"),
            P(20, "twenty"),
            P(30, "thirty"),
            P(40, "forty"),
            P(50, "fifty"),
            P(50, "fiftyA"),
            P(50, "fiftyB"),
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
        r = c.equal_range(50);
        TC_ASSERT_EXPR(r.first->first == 50);
        TC_ASSERT_EXPR(r.first->second == "fifty");
        ++r.first;
        TC_ASSERT_EXPR(r.first->first == 50);
        TC_ASSERT_EXPR(r.first->second == "fiftyA");
        ++r.first;
        TC_ASSERT_EXPR(r.first->first == 50);
        TC_ASSERT_EXPR(r.first->second == "fiftyB");
    }
    TC_SUCCESS_RESULT();
    return 0;
}
