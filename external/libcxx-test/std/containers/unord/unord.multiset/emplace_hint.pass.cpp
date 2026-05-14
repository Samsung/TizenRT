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

// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_multiset

// template <class... Args>
//     iterator emplace_hint(const_iterator p, Args&&... args);


#include <unordered_set>
#include <cassert>

#include "test_macros.h"
#include "../../Emplaceable.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_multiset_emplace_hint(void) {
    {
        typedef std::unordered_multiset<Emplaceable> C;
        typedef C::iterator R;
        C c;
        C::const_iterator e = c.end();
        R r = c.emplace_hint(e);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r == Emplaceable());

        r = c.emplace_hint(c.end(), Emplaceable(5, 6));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(*r == Emplaceable(5, 6));

        r = c.emplace_hint(r, 5, 6);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(*r == Emplaceable(5, 6));
    }
    {
        typedef std::unordered_multiset<Emplaceable, std::hash<Emplaceable>,
                      std::equal_to<Emplaceable>, min_allocator<Emplaceable>> C;
        typedef C::iterator R;
        C c;
        C::const_iterator e = c.end();
        R r = c.emplace_hint(e);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r == Emplaceable());

        r = c.emplace_hint(c.end(), Emplaceable(5, 6));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(*r == Emplaceable(5, 6));

        r = c.emplace_hint(r, 5, 6);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(*r == Emplaceable(5, 6));
    }

  return 0;
}
