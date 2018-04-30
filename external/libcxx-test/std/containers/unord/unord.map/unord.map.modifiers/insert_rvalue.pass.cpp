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
// class unordered_map

// template <class P,
//           class = typename enable_if<is_convertible<P, value_type>::value>::type>
//     pair<iterator, bool> insert(P&& x);

#include <unordered_map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "MoveOnly.h"

int tc_libcxx_containers_unord_map_modifiers_insert_rvalue(void)
{
    {
        typedef std::unordered_map<double, int> C;
        typedef std::pair<C::iterator, bool> R;
        typedef std::pair<double, short> P;
        C c;
        R r = c.insert(P(3.5, static_cast<short>(3)));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(r.first->first == 3.5);
        TC_ASSERT_EXPR(r.first->second == 3);

        r = c.insert(P(3.5, static_cast<short>(4)));
        TC_ASSERT_EXPR(!r.second);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(r.first->first == 3.5);
        TC_ASSERT_EXPR(r.first->second == 3);

        r = c.insert(P(4.5, static_cast<short>(4)));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(r.first->first == 4.5);
        TC_ASSERT_EXPR(r.first->second == 4);

        r = c.insert(P(5.5, static_cast<short>(4)));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(r.first->first == 5.5);
        TC_ASSERT_EXPR(r.first->second == 4);
    }
    {
        typedef std::unordered_map<MoveOnly, MoveOnly> C;
        typedef std::pair<C::iterator, bool> R;
        typedef std::pair<MoveOnly, MoveOnly> P;
        C c;
        R r = c.insert(P(3, 3));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(r.first->first == 3);
        TC_ASSERT_EXPR(r.first->second == 3);

        r = c.insert(P(3, 4));
        TC_ASSERT_EXPR(!r.second);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(r.first->first == 3);
        TC_ASSERT_EXPR(r.first->second == 3);

        r = c.insert(P(4, 4));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(r.first->first == 4);
        TC_ASSERT_EXPR(r.first->second == 4);

        r = c.insert(P(5, 4));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(r.first->first == 5);
        TC_ASSERT_EXPR(r.first->second == 4);
    }
    {
        typedef std::unordered_map<double, MoveOnly> C;
        typedef std::pair<C::iterator, bool> R;
        C c;
        R r = c.insert({3.5, 3});
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(r.first->first == 3.5);
        TC_ASSERT_EXPR(r.first->second == 3);

        r = c.insert({3.5, 4});
        TC_ASSERT_EXPR(!r.second);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(r.first->first == 3.5);
        TC_ASSERT_EXPR(r.first->second == 3);

        r = c.insert({4.5, 4});
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(r.first->first == 4.5);
        TC_ASSERT_EXPR(r.first->second == 4);

        r = c.insert({5.5, 4});
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(r.first->first == 5.5);
        TC_ASSERT_EXPR(r.first->second == 4);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
