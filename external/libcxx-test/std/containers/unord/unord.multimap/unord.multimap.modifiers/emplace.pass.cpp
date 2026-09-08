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

// template <class... Args>
//     iterator emplace(Args&&... args);

#include <unordered_map>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "Emplaceable.h"

int tc_libcxx_containers_unord_multimap_modifiers_emplace(void)
{
    {
        typedef std::unordered_multimap<int, Emplaceable> C;
        typedef C::iterator R;
        C c;
        R r = c.emplace(std::piecewise_construct, std::forward_as_tuple(3),
                                                  std::forward_as_tuple());
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(r->first == 3);
        TC_ASSERT_EXPR(r->second == Emplaceable());

        r = c.emplace(std::pair<const int, Emplaceable>(4, Emplaceable(5, 6)));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(r->first == 4);
        TC_ASSERT_EXPR(r->second == Emplaceable(5, 6));

        r = c.emplace(std::piecewise_construct, std::forward_as_tuple(5),
                                                std::forward_as_tuple(6, 7));
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(r->first == 5);
        TC_ASSERT_EXPR(r->second == Emplaceable(6, 7));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
