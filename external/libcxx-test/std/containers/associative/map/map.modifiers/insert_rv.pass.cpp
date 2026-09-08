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

// <map>

// class map

// pair<iterator, bool> insert( value_type&& v);  // C++17 and later
// template <class P>
//   pair<iterator, bool> insert(P&& p);

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "MoveOnly.h"
#include "test_macros.h"

template <class Container, class Pair>
static int do_insert_rv_test()
{
    typedef Container M;
    typedef Pair P;
    typedef std::pair<typename M::iterator, bool> R;
    M m;
    R r = m.insert(P(2, 2));
    TC_ASSERT_EXPR(r.second);
    TC_ASSERT_EXPR(r.first == m.begin());
    TC_ASSERT_EXPR(m.size() == 1);
    TC_ASSERT_EXPR(r.first->first == 2);
    TC_ASSERT_EXPR(r.first->second == 2);

    r = m.insert(P(1, 1));
    TC_ASSERT_EXPR(r.second);
    TC_ASSERT_EXPR(r.first == m.begin());
    TC_ASSERT_EXPR(m.size() == 2);
    TC_ASSERT_EXPR(r.first->first == 1);
    TC_ASSERT_EXPR(r.first->second == 1);

    r = m.insert(P(3, 3));
    TC_ASSERT_EXPR(r.second);
    TC_ASSERT_EXPR(r.first == prev(m.end()));
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(r.first->first == 3);
    TC_ASSERT_EXPR(r.first->second == 3);

    r = m.insert(P(3, 3));
    TC_ASSERT_EXPR(!r.second);
    TC_ASSERT_EXPR(r.first == prev(m.end()));
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(r.first->first == 3);
    TC_ASSERT_EXPR(r.first->second == 3);
    return 0;
}

int tc_libcxx_containers_map_modifiers_insert_rv(void)
{
    do_insert_rv_test<std::map<int, MoveOnly>, std::pair<int, MoveOnly>>();
    do_insert_rv_test<std::map<int, MoveOnly>, std::pair<const int, MoveOnly>>();

    {
        typedef std::map<int, MoveOnly> M;
        typedef std::pair<M::iterator, bool> R;
        M m;
        R r = m.insert({2, MoveOnly(2)});
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(r.first->first == 2);
        TC_ASSERT_EXPR(r.first->second == 2);

        r = m.insert({1, MoveOnly(1)});
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == m.begin());
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(r.first->first == 1);
        TC_ASSERT_EXPR(r.first->second == 1);

        r = m.insert({3, MoveOnly(3)});
        TC_ASSERT_EXPR(r.second);
        TC_ASSERT_EXPR(r.first == prev(m.end()));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(r.first->first == 3);
        TC_ASSERT_EXPR(r.first->second == 3);

        r = m.insert({3, MoveOnly(3)});
        TC_ASSERT_EXPR(!r.second);
        TC_ASSERT_EXPR(r.first == prev(m.end()));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(r.first->first == 3);
        TC_ASSERT_EXPR(r.first->second == 3);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
