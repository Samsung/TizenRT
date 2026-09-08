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

// UNSUPPORTED: c++98, c++03

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_multimap

// template <class P,
//           class = typename enable_if<is_convertible<P, value_type>::value>::type>
//     iterator insert(P&& x);

#include <unordered_map>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "MoveOnly.h"

int tc_libcxx_containers_unord_multimap_modifiers_insert_rvalue(void)
{
    {
        typedef std::unordered_multimap<double, int> C;
        typedef C::iterator R;
        typedef std::pair<double, short> P;
        C c;
        R r = c.insert(P(3.5, static_cast<short>(3)));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(r->first == 3.5);
        TC_ASSERT_EXPR(r->second == 3);

        r = c.insert(P(3.5, static_cast<short>(4)));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(r->first == 3.5);
        TC_ASSERT_EXPR(r->second == 4);

        r = c.insert(P(4.5, static_cast<short>(4)));
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(r->first == 4.5);
        TC_ASSERT_EXPR(r->second == 4);

        r = c.insert(P(5.5, static_cast<short>(4)));
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(r->first == 5.5);
        TC_ASSERT_EXPR(r->second == 4);
    }
    {
        typedef std::unordered_multimap<MoveOnly, MoveOnly> C;
        typedef C::iterator R;
        typedef std::pair<MoveOnly, MoveOnly> P;
        C c;
        R r = c.insert(P(3, 3));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(r->first == 3);
        TC_ASSERT_EXPR(r->second == 3);

        r = c.insert(P(3, 4));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(r->first == 3);
        TC_ASSERT_EXPR(r->second == 4);

        r = c.insert(P(4, 4));
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(r->first == 4);
        TC_ASSERT_EXPR(r->second == 4);

        r = c.insert(P(5, 4));
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(r->first == 5);
        TC_ASSERT_EXPR(r->second == 4);
    }
    {
        typedef std::unordered_multimap<double, MoveOnly> C;
        typedef C::iterator R;
        C c;
        R r = c.insert({3.5, 3});
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(r->first == 3.5);
        TC_ASSERT_EXPR(r->second == 3);

        r = c.insert({3.5, 4});
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(r->first == 3.5);
        TC_ASSERT_EXPR(r->second == 4);

        r = c.insert({4.5, 4});
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(r->first == 4.5);
        TC_ASSERT_EXPR(r->second == 4);

        r = c.insert({5.5, 4});
        TC_ASSERT_EXPR(c.size() == 4);
        TC_ASSERT_EXPR(r->first == 5.5);
        TC_ASSERT_EXPR(r->second == 4);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
