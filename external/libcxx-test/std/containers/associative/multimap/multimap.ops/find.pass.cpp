//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <map>

// class multimap

//       iterator find(const key_type& k);
// const_iterator find(const key_type& k) const;

#include <map>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "private_constructor.h"
#include "is_transparent.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multimap_multimap_ops_find(void) {
    typedef std::pair<const int, double> V;
    {
    typedef std::multimap<int, double> M;
    {
        typedef M::iterator R;
        V ar[] =
        {
            V(5, 1),
            V(5, 2),
            V(5, 3),
            V(7, 1),
            V(7, 2),
            V(7, 3),
            V(9, 1),
            V(9, 2),
            V(9, 3)
        };
        M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.find(5);
        TC_ASSERT_EXPR(r == m.begin());
        r = m.find(6);
        TC_ASSERT_EXPR(r == m.end());
        r = m.find(7);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.find(8);
        TC_ASSERT_EXPR(r == m.end());
        r = m.find(9);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.find(10);
        TC_ASSERT_EXPR(r == m.end());
    }
    {
        typedef M::const_iterator R;
        V ar[] =
        {
            V(5, 1),
            V(5, 2),
            V(5, 3),
            V(7, 1),
            V(7, 2),
            V(7, 3),
            V(9, 1),
            V(9, 2),
            V(9, 3)
        };
        const M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.find(5);
        TC_ASSERT_EXPR(r == m.begin());
        r = m.find(6);
        TC_ASSERT_EXPR(r == m.end());
        r = m.find(7);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.find(8);
        TC_ASSERT_EXPR(r == m.end());
        r = m.find(9);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.find(10);
        TC_ASSERT_EXPR(r == m.end());
    }
    }
#if TEST_STD_VER >= 11
    {
    typedef std::multimap<int, double, std::less<int>, min_allocator<std::pair<const int, double>>> M;
    {
        typedef M::iterator R;
        V ar[] =
        {
            V(5, 1),
            V(5, 2),
            V(5, 3),
            V(7, 1),
            V(7, 2),
            V(7, 3),
            V(9, 1),
            V(9, 2),
            V(9, 3)
        };
        M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.find(5);
        TC_ASSERT_EXPR(r == m.begin());
        r = m.find(6);
        TC_ASSERT_EXPR(r == m.end());
        r = m.find(7);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.find(8);
        TC_ASSERT_EXPR(r == m.end());
        r = m.find(9);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.find(10);
        TC_ASSERT_EXPR(r == m.end());
    }
    {
        typedef M::const_iterator R;
        V ar[] =
        {
            V(5, 1),
            V(5, 2),
            V(5, 3),
            V(7, 1),
            V(7, 2),
            V(7, 3),
            V(9, 1),
            V(9, 2),
            V(9, 3)
        };
        const M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.find(5);
        TC_ASSERT_EXPR(r == m.begin());
        r = m.find(6);
        TC_ASSERT_EXPR(r == m.end());
        r = m.find(7);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.find(8);
        TC_ASSERT_EXPR(r == m.end());
        r = m.find(9);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.find(10);
        TC_ASSERT_EXPR(r == m.end());
    }
    }
#endif
#if TEST_STD_VER > 11
    {
    typedef std::multimap<int, double, std::less<>> M;
    typedef M::iterator R;

        V ar[] =
        {
            V(5, 1),
            V(5, 2),
            V(5, 3),
            V(7, 1),
            V(7, 2),
            V(7, 3),
            V(9, 1),
            V(9, 2),
            V(9, 3)
        };
        M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.find(5);
        TC_ASSERT_EXPR(r == m.begin());
        r = m.find(6);
        TC_ASSERT_EXPR(r == m.end());
        r = m.find(7);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.find(8);
        TC_ASSERT_EXPR(r == m.end());
        r = m.find(9);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.find(10);
        TC_ASSERT_EXPR(r == m.end());

        r = m.find(C2Int(5));
        TC_ASSERT_EXPR(r == m.begin());
        r = m.find(C2Int(6));
        TC_ASSERT_EXPR(r == m.end());
        r = m.find(C2Int(7));
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.find(C2Int(8));
        TC_ASSERT_EXPR(r == m.end());
        r = m.find(C2Int(9));
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.find(C2Int(10));
        TC_ASSERT_EXPR(r == m.end());
    }

    {
    typedef PrivateConstructor PC;
    typedef std::multimap<PC, double, std::less<>> M;
    typedef M::iterator R;

    M m;
    m.insert ( std::make_pair<PC, double> ( PC::make(5), 1 ));
    m.insert ( std::make_pair<PC, double> ( PC::make(5), 2 ));
    m.insert ( std::make_pair<PC, double> ( PC::make(5), 3 ));
    m.insert ( std::make_pair<PC, double> ( PC::make(7), 1 ));
    m.insert ( std::make_pair<PC, double> ( PC::make(7), 2 ));
    m.insert ( std::make_pair<PC, double> ( PC::make(7), 3 ));
    m.insert ( std::make_pair<PC, double> ( PC::make(9), 1 ));
    m.insert ( std::make_pair<PC, double> ( PC::make(9), 2 ));
    m.insert ( std::make_pair<PC, double> ( PC::make(9), 3 ));

    R r = m.find(5);
    TC_ASSERT_EXPR(r == m.begin());
    r = m.find(6);
    TC_ASSERT_EXPR(r == m.end());
    r = m.find(7);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
    r = m.find(8);
    TC_ASSERT_EXPR(r == m.end());
    r = m.find(9);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
    r = m.find(10);
    TC_ASSERT_EXPR(r == m.end());
    }
#endif

  return 0;
}
