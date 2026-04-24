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
// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_set

// iterator insert(const_iterator p, value_type&& x);

#include <unordered_set>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_set_insert_hint_rvalue(void) {
    {
        typedef std::unordered_set<double> C;
        typedef C::iterator R;
        typedef double P;
        C c;
        typename C::const_iterator e = c.end();
        R r = c.insert(e, P(3.5));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r == 3.5);

        r = c.insert(r, P(3.5));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r == 3.5);

        r = c.insert(c.end(), P(4.5));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(*r == 4.5);

        r = c.insert(c.end(), P(5.5));
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(*r == 5.5);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_set<MoveOnly> C;
        typedef C::iterator R;
        typedef MoveOnly P;
        C c;
        typename C::const_iterator e = c.end();
        R r = c.insert(e, P(3));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r == 3);

        r = c.insert(r, P(3));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r == 3);

        r = c.insert(c.end(), P(4));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(*r == 4);

        r = c.insert(c.end(), P(5));
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(*r == 5);
    }
    {
        typedef std::unordered_set<double, std::hash<double>,
                                std::equal_to<double>, min_allocator<double>> C;
        typedef C::iterator R;
        typedef double P;
        C c;
        typename C::const_iterator e = c.end();
        R r = c.insert(e, P(3.5));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r == 3.5);

        r = c.insert(r, P(3.5));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r == 3.5);

        r = c.insert(c.end(), P(4.5));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(*r == 4.5);

        r = c.insert(c.end(), P(5.5));
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(*r == 5.5);
    }
    {
        typedef std::unordered_set<MoveOnly, std::hash<MoveOnly>,
                            std::equal_to<MoveOnly>, min_allocator<MoveOnly>> C;
        typedef C::iterator R;
        typedef MoveOnly P;
        C c;
        typename C::const_iterator e = c.end();
        R r = c.insert(e, P(3));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r == 3);

        r = c.insert(r, P(3));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r == 3);

        r = c.insert(c.end(), P(4));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(*r == 4);

        r = c.insert(c.end(), P(5));
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(*r == 5);
    }
#endif // TEST_STD_VER >= 11

  return 0;
}
