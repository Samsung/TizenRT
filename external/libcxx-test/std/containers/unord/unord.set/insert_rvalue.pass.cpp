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

// pair<iterator, bool> insert(value_type&& x);

#include <unordered_set>
#include <cassert>

#include "test_macros.h"
#include "MoveOnly.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_set_insert_rvalue(void) {
    {
        typedef std::unordered_set<double> C;
        typedef std::pair<C::iterator, bool> R;
        typedef double P;
        C c;
        R r = c.insert(P(3.5));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r.first == 3.5);
        TC_ASSERT_EXPR(r.second);

        r = c.insert(P(3.5));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r.first == 3.5);
        TC_ASSERT_EXPR(!r.second);

        r = c.insert(P(4.5));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(*r.first == 4.5);
        TC_ASSERT_EXPR(r.second);

        r = c.insert(P(5.5));
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(*r.first == 5.5);
        TC_ASSERT_EXPR(r.second);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_set<MoveOnly> C;
        typedef std::pair<C::iterator, bool> R;
        typedef MoveOnly P;
        C c;
        R r = c.insert(P(3));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r.first == 3);
        TC_ASSERT_EXPR(r.second);

        r = c.insert(P(3));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r.first == 3);
        TC_ASSERT_EXPR(!r.second);

        r = c.insert(P(4));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(*r.first == 4);
        TC_ASSERT_EXPR(r.second);

        r = c.insert(P(5));
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(*r.first == 5);
        TC_ASSERT_EXPR(r.second);
    }
    {
        typedef std::unordered_set<double, std::hash<double>,
                                std::equal_to<double>, min_allocator<double>> C;
        typedef std::pair<C::iterator, bool> R;
        typedef double P;
        C c;
        R r = c.insert(P(3.5));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r.first == 3.5);
        TC_ASSERT_EXPR(r.second);

        r = c.insert(P(3.5));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r.first == 3.5);
        TC_ASSERT_EXPR(!r.second);

        r = c.insert(P(4.5));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(*r.first == 4.5);
        TC_ASSERT_EXPR(r.second);

        r = c.insert(P(5.5));
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(*r.first == 5.5);
        TC_ASSERT_EXPR(r.second);
    }
    {
        typedef std::unordered_set<MoveOnly, std::hash<MoveOnly>,
                            std::equal_to<MoveOnly>, min_allocator<MoveOnly>> C;
        typedef std::pair<C::iterator, bool> R;
        typedef MoveOnly P;
        C c;
        R r = c.insert(P(3));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r.first == 3);
        TC_ASSERT_EXPR(r.second);

        r = c.insert(P(3));
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(*r.first == 3);
        TC_ASSERT_EXPR(!r.second);

        r = c.insert(P(4));
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(*r.first == 4);
        TC_ASSERT_EXPR(r.second);

        r = c.insert(P(5));
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(*r.first == 5);
        TC_ASSERT_EXPR(r.second);
    }
#endif // TEST_STD_VER >= 11

  return 0;
}
