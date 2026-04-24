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
// UNSUPPORTED: c++03

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_multimap

// template <class... Args>
//     iterator emplace_hint(const_iterator p, Args&&... args);


#include <unordered_map>
#include <cassert>
#include <iterator>

#include "../../../Emplaceable.h"
#include "min_allocator.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_multimap_unord_multimap_modifiers_emplace_hint(void) {
    {
        typedef std::unordered_multimap<int, Emplaceable> C;
        typedef C::iterator R;
        C c;
        C::const_iterator e = c.end();
        R r = c.emplace_hint(e, std::piecewise_construct, std::forward_as_tuple(3),
                                                          std::forward_as_tuple());
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(r->first == 3);
        TC_ASSERT_EXPR(r->second == Emplaceable());

        r = c.emplace_hint(c.end(), std::pair<const int, Emplaceable>(3, Emplaceable(5, 6)));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(r->first == 3);
        TC_ASSERT_EXPR(r->second == Emplaceable(5, 6));
        LIBCPP_ASSERT(r == std::next(c.begin()));

        r = c.emplace_hint(r, std::piecewise_construct, std::forward_as_tuple(3),
                                                        std::forward_as_tuple(6, 7));
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(r->first == 3);
        TC_ASSERT_EXPR(r->second == Emplaceable(6, 7));
        LIBCPP_ASSERT(r == std::next(c.begin()));
        r = c.begin();
        TC_ASSERT_EXPR(r->first == 3);
        LIBCPP_ASSERT(r->second == Emplaceable());
        r = std::next(r, 2);
        TC_ASSERT_EXPR(r->first == 3);
        LIBCPP_ASSERT(r->second == Emplaceable(5, 6));
    }
    {
        typedef std::unordered_multimap<int, Emplaceable, std::hash<int>, std::equal_to<int>,
                            min_allocator<std::pair<const int, Emplaceable>>> C;
        typedef C::iterator R;
        C c;
        C::const_iterator e = c.end();
        R r = c.emplace_hint(e, std::piecewise_construct, std::forward_as_tuple(3),
                                                          std::forward_as_tuple());
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(r->first == 3);
        TC_ASSERT_EXPR(r->second == Emplaceable());

        r = c.emplace_hint(c.end(), std::pair<const int, Emplaceable>(3, Emplaceable(5, 6)));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(r->first == 3);
        TC_ASSERT_EXPR(r->second == Emplaceable(5, 6));
        LIBCPP_ASSERT(r == std::next(c.begin()));

        r = c.emplace_hint(r, std::piecewise_construct, std::forward_as_tuple(3),
                                                        std::forward_as_tuple(6, 7));
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(r->first == 3);
        TC_ASSERT_EXPR(r->second == Emplaceable(6, 7));
        LIBCPP_ASSERT(r == std::next(c.begin()));
        r = c.begin();
        TC_ASSERT_EXPR(r->first == 3);
        LIBCPP_ASSERT(r->second == Emplaceable());
        r = std::next(r, 2);
        TC_ASSERT_EXPR(r->first == 3);
        LIBCPP_ASSERT(r->second == Emplaceable(5, 6));
    }

  return 0;
}
