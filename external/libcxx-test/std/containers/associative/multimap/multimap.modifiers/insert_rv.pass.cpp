//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <map>

// class multimap

// template <class P>
//   iterator insert(P&& p);

#include <map>
#include <cassert>

#include "MoveOnly.h"
#include "min_allocator.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class Container>
void do_insert_rv_test()
{
    typedef std::multimap<int, MoveOnly> M;
    typedef typename M::iterator R;
    typedef typename M::value_type VT;
    M m;
    R r = m.insert(VT(2, 2));
    TC_ASSERT_EXPR(r == m.begin());
    TC_ASSERT_EXPR(m.size() == 1);
    TC_ASSERT_EXPR(r->first == 2);
    TC_ASSERT_EXPR(r->second == 2);

    r = m.insert(VT(1, 1));
    TC_ASSERT_EXPR(r == m.begin());
    TC_ASSERT_EXPR(m.size() == 2);
    TC_ASSERT_EXPR(r->first == 1);
    TC_ASSERT_EXPR(r->second == 1);

    r = m.insert(VT(3, 3));
    TC_ASSERT_EXPR(r == std::prev(m.end()));
    TC_ASSERT_EXPR(m.size() == 3);
    TC_ASSERT_EXPR(r->first == 3);
    TC_ASSERT_EXPR(r->second == 3);

    r = m.insert(VT(3, 3));
    TC_ASSERT_EXPR(r == std::prev(m.end()));
    TC_ASSERT_EXPR(m.size() == 4);
    TC_ASSERT_EXPR(r->first == 3);
    TC_ASSERT_EXPR(r->second == 3);
}

int tc_containers_associative_multimap_multimap_modifiers_insert_rv(void) {
    do_insert_rv_test<std::multimap<int, MoveOnly>>();
    {
        typedef std::multimap<int, MoveOnly, std::less<int>, min_allocator<std::pair<const int, MoveOnly>>> M;
        do_insert_rv_test<M>();
    }
    {
        typedef std::multimap<int, MoveOnly> M;
        typedef M::iterator R;
        M m;
        R r = m.insert({2, MoveOnly(2)});
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 1);
        TC_ASSERT_EXPR(r->first == 2);
        TC_ASSERT_EXPR(r->second == 2);

        r = m.insert({1, MoveOnly(1)});
        TC_ASSERT_EXPR(r == m.begin());
        TC_ASSERT_EXPR(m.size() == 2);
        TC_ASSERT_EXPR(r->first == 1);
        TC_ASSERT_EXPR(r->second == 1);

        r = m.insert({3, MoveOnly(3)});
        TC_ASSERT_EXPR(r == std::prev(m.end()));
        TC_ASSERT_EXPR(m.size() == 3);
        TC_ASSERT_EXPR(r->first == 3);
        TC_ASSERT_EXPR(r->second == 3);

        r = m.insert({3, MoveOnly(3)});
        TC_ASSERT_EXPR(r == std::prev(m.end()));
        TC_ASSERT_EXPR(m.size() == 4);
        TC_ASSERT_EXPR(r->first == 3);
        TC_ASSERT_EXPR(r->second == 3);
    }

  return 0;
}
