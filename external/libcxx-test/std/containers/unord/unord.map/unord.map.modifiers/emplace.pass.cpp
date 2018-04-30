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

// template <class... Args>
//     pair<iterator, bool> emplace(Args&&... args);

#include <unordered_map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "Emplaceable.h"

int tc_libcxx_containers_unord_map_modifiers_emplace(void)
{
    {
        typedef std::unordered_map<int, Emplaceable> C;
        typedef std::pair<C::iterator, bool> R;
        C c;
        R r = c.emplace(std::piecewise_construct, std::forward_as_tuple(3),
                                                  std::forward_as_tuple());
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(r.first->first == 3);
        TC_ASSERT_EXPR(r.first->second == Emplaceable());

        r = c.emplace(std::pair<const int, Emplaceable>(4, Emplaceable(5, 6)));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(r.first->first == 4);
        TC_ASSERT_EXPR(r.first->second == Emplaceable(5, 6));

        r = c.emplace(std::piecewise_construct, std::forward_as_tuple(5),
                                               std::forward_as_tuple(6, 7));
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(r.first->first == 5);
        TC_ASSERT_EXPR(r.first->second == Emplaceable(6, 7));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
