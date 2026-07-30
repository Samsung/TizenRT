//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <set>

// class set

//       iterator upper_bound(const key_type& k);
// const_iterator upper_bound(const key_type& k) const;

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "private_constructor.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_upper_bound(void) {
    {
    typedef int V;
    typedef std::set<int> M;
    {
        typedef M::iterator R;
        V ar[] =
        {
            5,
            7,
            9,
            11,
            13,
            15,
            17,
            19
        };
        M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.upper_bound(5);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 1));
        r = m.upper_bound(7);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 2));
        r = m.upper_bound(9);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.upper_bound(11);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 4));
        r = m.upper_bound(13);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 5));
        r = m.upper_bound(15);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.upper_bound(17);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 7));
        r = m.upper_bound(19);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 8));
        r = m.upper_bound(4);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 0));
        r = m.upper_bound(6);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 1));
        r = m.upper_bound(8);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 2));
        r = m.upper_bound(10);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.upper_bound(12);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 4));
        r = m.upper_bound(14);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 5));
        r = m.upper_bound(16);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.upper_bound(18);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 7));
        r = m.upper_bound(20);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 8));
    }
    {
        typedef M::const_iterator R;
        V ar[] =
        {
            5,
            7,
            9,
            11,
            13,
            15,
            17,
            19
        };
        const M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.upper_bound(5);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 1));
        r = m.upper_bound(7);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 2));
        r = m.upper_bound(9);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.upper_bound(11);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 4));
        r = m.upper_bound(13);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 5));
        r = m.upper_bound(15);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.upper_bound(17);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 7));
        r = m.upper_bound(19);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 8));
        r = m.upper_bound(4);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 0));
        r = m.upper_bound(6);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 1));
        r = m.upper_bound(8);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 2));
        r = m.upper_bound(10);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.upper_bound(12);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 4));
        r = m.upper_bound(14);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 5));
        r = m.upper_bound(16);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.upper_bound(18);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 7));
        r = m.upper_bound(20);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 8));
    }
    }
#if TEST_STD_VER >= 11
    {
    typedef int V;
    typedef std::set<int, std::less<int>, min_allocator<int>> M;
    {
        typedef M::iterator R;
        V ar[] =
        {
            5,
            7,
            9,
            11,
            13,
            15,
            17,
            19
        };
        M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.upper_bound(5);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 1));
        r = m.upper_bound(7);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 2));
        r = m.upper_bound(9);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.upper_bound(11);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 4));
        r = m.upper_bound(13);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 5));
        r = m.upper_bound(15);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.upper_bound(17);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 7));
        r = m.upper_bound(19);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 8));
        r = m.upper_bound(4);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 0));
        r = m.upper_bound(6);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 1));
        r = m.upper_bound(8);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 2));
        r = m.upper_bound(10);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.upper_bound(12);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 4));
        r = m.upper_bound(14);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 5));
        r = m.upper_bound(16);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.upper_bound(18);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 7));
        r = m.upper_bound(20);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 8));
    }
    {
        typedef M::const_iterator R;
        V ar[] =
        {
            5,
            7,
            9,
            11,
            13,
            15,
            17,
            19
        };
        const M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.upper_bound(5);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 1));
        r = m.upper_bound(7);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 2));
        r = m.upper_bound(9);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.upper_bound(11);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 4));
        r = m.upper_bound(13);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 5));
        r = m.upper_bound(15);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.upper_bound(17);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 7));
        r = m.upper_bound(19);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 8));
        r = m.upper_bound(4);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 0));
        r = m.upper_bound(6);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 1));
        r = m.upper_bound(8);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 2));
        r = m.upper_bound(10);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.upper_bound(12);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 4));
        r = m.upper_bound(14);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 5));
        r = m.upper_bound(16);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.upper_bound(18);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 7));
        r = m.upper_bound(20);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 8));
    }
    }
#endif
#if TEST_STD_VER > 11
    {
    typedef int V;
    typedef std::set<V, std::less<>> M;
    typedef M::iterator R;

    V ar[] =
    {
        5,
        7,
        9,
        11,
        13,
        15,
        17,
        19
    };
    M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
    R r = m.upper_bound(5);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 1));
    r = m.upper_bound(7);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 2));
    r = m.upper_bound(9);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
    r = m.upper_bound(11);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 4));
    r = m.upper_bound(13);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 5));
    r = m.upper_bound(15);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
    r = m.upper_bound(17);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 7));
    r = m.upper_bound(19);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 8));
    r = m.upper_bound(4);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 0));
    r = m.upper_bound(6);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 1));
    r = m.upper_bound(8);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 2));
    r = m.upper_bound(10);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
    r = m.upper_bound(12);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 4));
    r = m.upper_bound(14);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 5));
    r = m.upper_bound(16);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
    r = m.upper_bound(18);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 7));
    r = m.upper_bound(20);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 8));
    }

    {
    typedef PrivateConstructor V;
    typedef std::set<V, std::less<>> M;
    typedef M::iterator R;

    M m;
    m.insert ( V::make ( 5 ));
    m.insert ( V::make ( 7 ));
    m.insert ( V::make ( 9 ));
    m.insert ( V::make ( 11 ));
    m.insert ( V::make ( 13 ));
    m.insert ( V::make ( 15 ));
    m.insert ( V::make ( 17 ));
    m.insert ( V::make ( 19 ));

    R r = m.upper_bound(5);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 1));
    r = m.upper_bound(7);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 2));
    r = m.upper_bound(9);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
    r = m.upper_bound(11);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 4));
    r = m.upper_bound(13);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 5));
    r = m.upper_bound(15);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
    r = m.upper_bound(17);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 7));
    r = m.upper_bound(19);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 8));
    r = m.upper_bound(4);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 0));
    r = m.upper_bound(6);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 1));
    r = m.upper_bound(8);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 2));
    r = m.upper_bound(10);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
    r = m.upper_bound(12);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 4));
    r = m.upper_bound(14);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 5));
    r = m.upper_bound(16);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
    r = m.upper_bound(18);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 7));
    r = m.upper_bound(20);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 8));
    }
#endif

  return 0;
}
