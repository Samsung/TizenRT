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

// class multimap

// template <class P>
//     iterator insert(const_iterator position, P&& p);

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
    typedef typename M::iterator R;
    M m;
    R r = m.insert(m.cend(), P(2, 2));
    TC_ASSERT_EXPR(r == m.begin());
    TC_ASSERT_EXPR(m.size() == 1);
    TC_ASSERT_EXPR(r->first == 2);
    TC_ASSERT_EXPR(r->second == 2);

    r = m.insert(m.cend(), P(1, 1));
    TC_ASSERT_EXPR(r == m.begin());
    TC_ASSERT_EXPR(m.size() == 2);
    TC_ASSERT_EXPR(r->first == 1);
    TC_ASSERT_EXPR(r->second == 1);

    r = m.insert(m.cend(), P(3, 3));
    TC_ASSERT_EXPR(r == prev(m.end()));
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(r->first == 3);
    TC_ASSERT_EXPR(r->second == 3);

    r = m.insert(m.cend(), P(3, 2));
    TC_ASSERT_EXPR(r == prev(m.end()));
    TC_ASSERT_EXPR(m.size() == 4);
    TC_ASSERT_EXPR(r->first == 3);
    TC_ASSERT_EXPR(r->second == 2);
    return 0;
}

int tc_libcxx_containers_multimap_modifiers_insert_iter_rv(void)
{
    do_insert_rv_test<std::multimap<int, MoveOnly>, std::pair<int, MoveOnly> >();
    do_insert_rv_test<std::multimap<int, MoveOnly>, std::pair<const int, MoveOnly> >();

    {
        typedef std::multimap<int, MoveOnly> M;
        typedef M::iterator R;
        M m;
        R r = m.insert(m.cend(), {2, MoveOnly(2)});
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(r->first == 2);
        TC_ASSERT_EXPR(r->second == 2);

        r = m.insert(m.cend(), {1, MoveOnly(1)});
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(r->first == 1);
        TC_ASSERT_EXPR(r->second == 1);

        r = m.insert(m.cend(), {3, MoveOnly(3)});
        TC_ASSERT_EXPR(r == prev(m.end()));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(r->first == 3);
        TC_ASSERT_EXPR(r->second == 3);

        r = m.insert(m.cend(), {3, MoveOnly(2)});
        TC_ASSERT_EXPR(r == prev(m.end()));
        TC_ASSERT_EXPR(m.size() == 4);
        TC_ASSERT_EXPR(r->first == 3);
        TC_ASSERT_EXPR(r->second == 2);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
