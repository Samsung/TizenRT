//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <map>

// class map

// pair<iterator,iterator>             equal_range(const key_type& k);
// pair<const_iterator,const_iterator> equal_range(const key_type& k) const;

#include <map>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "private_constructor.h"
#include "is_transparent.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_map_map_ops_equal_range(void) {
    {
    typedef std::pair<const int, double> V;
    typedef std::map<int, double> M;
    {
        typedef std::pair<M::iterator, M::iterator> R;
        V ar[] =
        {
            V(5, 5),
            V(7, 6),
            V(9, 7),
            V(11, 8),
            V(13, 9),
            V(15, 10),
            V(17, 11),
            V(19, 12)
        };
        M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.equal_range(5);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 0));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 1));
        r = m.equal_range(7);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 1));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 2));
        r = m.equal_range(9);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 2));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 3));
        r = m.equal_range(11);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 3));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 4));
        r = m.equal_range(13);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 4));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 5));
        r = m.equal_range(15);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 5));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 6));
        r = m.equal_range(17);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 6));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 7));
        r = m.equal_range(19);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 7));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 8));
        r = m.equal_range(4);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 0));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 0));
        r = m.equal_range(6);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 1));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 1));
        r = m.equal_range(8);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 2));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 2));
        r = m.equal_range(10);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 3));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 3));
        r = m.equal_range(12);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 4));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 4));
        r = m.equal_range(14);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 5));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 5));
        r = m.equal_range(16);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 6));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 6));
        r = m.equal_range(18);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 7));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 7));
        r = m.equal_range(20);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 8));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 8));
    }
    {
        typedef std::pair<M::const_iterator, M::const_iterator> R;
        V ar[] =
        {
            V(5, 5),
            V(7, 6),
            V(9, 7),
            V(11, 8),
            V(13, 9),
            V(15, 10),
            V(17, 11),
            V(19, 12)
        };
        const M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.equal_range(5);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 0));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 1));
        r = m.equal_range(7);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 1));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 2));
        r = m.equal_range(9);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 2));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 3));
        r = m.equal_range(11);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 3));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 4));
        r = m.equal_range(13);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 4));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 5));
        r = m.equal_range(15);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 5));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 6));
        r = m.equal_range(17);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 6));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 7));
        r = m.equal_range(19);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 7));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 8));
        r = m.equal_range(4);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 0));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 0));
        r = m.equal_range(6);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 1));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 1));
        r = m.equal_range(8);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 2));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 2));
        r = m.equal_range(10);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 3));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 3));
        r = m.equal_range(12);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 4));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 4));
        r = m.equal_range(14);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 5));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 5));
        r = m.equal_range(16);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 6));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 6));
        r = m.equal_range(18);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 7));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 7));
        r = m.equal_range(20);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 8));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 8));
    }
    }
#if TEST_STD_VER >= 11
    {
    typedef std::pair<const int, double> V;
    typedef std::map<int, double, std::less<int>, min_allocator<V>> M;
    {
        typedef std::pair<M::iterator, M::iterator> R;
        V ar[] =
        {
            V(5, 5),
            V(7, 6),
            V(9, 7),
            V(11, 8),
            V(13, 9),
            V(15, 10),
            V(17, 11),
            V(19, 12)
        };
        M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.equal_range(5);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 0));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 1));
        r = m.equal_range(7);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 1));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 2));
        r = m.equal_range(9);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 2));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 3));
        r = m.equal_range(11);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 3));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 4));
        r = m.equal_range(13);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 4));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 5));
        r = m.equal_range(15);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 5));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 6));
        r = m.equal_range(17);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 6));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 7));
        r = m.equal_range(19);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 7));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 8));
        r = m.equal_range(4);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 0));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 0));
        r = m.equal_range(6);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 1));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 1));
        r = m.equal_range(8);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 2));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 2));
        r = m.equal_range(10);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 3));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 3));
        r = m.equal_range(12);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 4));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 4));
        r = m.equal_range(14);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 5));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 5));
        r = m.equal_range(16);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 6));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 6));
        r = m.equal_range(18);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 7));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 7));
        r = m.equal_range(20);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 8));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 8));
    }
    {
        typedef std::pair<M::const_iterator, M::const_iterator> R;
        V ar[] =
        {
            V(5, 5),
            V(7, 6),
            V(9, 7),
            V(11, 8),
            V(13, 9),
            V(15, 10),
            V(17, 11),
            V(19, 12)
        };
        const M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.equal_range(5);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 0));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 1));
        r = m.equal_range(7);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 1));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 2));
        r = m.equal_range(9);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 2));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 3));
        r = m.equal_range(11);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 3));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 4));
        r = m.equal_range(13);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 4));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 5));
        r = m.equal_range(15);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 5));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 6));
        r = m.equal_range(17);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 6));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 7));
        r = m.equal_range(19);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 7));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 8));
        r = m.equal_range(4);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 0));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 0));
        r = m.equal_range(6);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 1));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 1));
        r = m.equal_range(8);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 2));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 2));
        r = m.equal_range(10);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 3));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 3));
        r = m.equal_range(12);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 4));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 4));
        r = m.equal_range(14);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 5));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 5));
        r = m.equal_range(16);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 6));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 6));
        r = m.equal_range(18);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 7));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 7));
        r = m.equal_range(20);
        TC_ASSERT_EXPR(r.first == std::next(m.begin(), 8));
        TC_ASSERT_EXPR(r.second == std::next(m.begin(), 8));
    }
    }
#endif
#if TEST_STD_VER > 11
    {
    typedef std::pair<const int, double> V;
    typedef std::map<int, double, std::less<>> M;
    typedef std::pair<M::iterator, M::iterator> R;

    V ar[] =
    {
        V(5, 5),
        V(7, 6),
        V(9, 7),
        V(11, 8),
        V(13, 9),
        V(15, 10),
        V(17, 11),
        V(19, 12)
    };
    M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
    R r = m.equal_range(5);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 0));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 1));
    r = m.equal_range(7);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 1));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 2));
    r = m.equal_range(9);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 2));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 3));
    r = m.equal_range(11);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 3));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 4));
    r = m.equal_range(13);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 4));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 5));
    r = m.equal_range(15);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 5));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 6));
    r = m.equal_range(17);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 6));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 7));
    r = m.equal_range(19);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 7));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 8));
    r = m.equal_range(4);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 0));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 0));
    r = m.equal_range(6);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 1));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 1));
    r = m.equal_range(8);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 2));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 2));
    r = m.equal_range(10);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 3));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 3));
    r = m.equal_range(12);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 4));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 4));
    r = m.equal_range(14);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 5));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 5));
    r = m.equal_range(16);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 6));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 6));
    r = m.equal_range(18);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 7));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 7));
    r = m.equal_range(20);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 8));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 8));

    r = m.equal_range(C2Int(5));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 0));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 1));
    r = m.equal_range(C2Int(7));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 1));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 2));
    r = m.equal_range(C2Int(9));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 2));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 3));
    r = m.equal_range(C2Int(11));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 3));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 4));
    r = m.equal_range(C2Int(13));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 4));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 5));
    r = m.equal_range(C2Int(15));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 5));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 6));
    r = m.equal_range(C2Int(17));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 6));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 7));
    r = m.equal_range(C2Int(19));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 7));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 8));
    r = m.equal_range(C2Int(4));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 0));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 0));
    r = m.equal_range(C2Int(6));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 1));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 1));
    r = m.equal_range(C2Int(8));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 2));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 2));
    r = m.equal_range(C2Int(10));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 3));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 3));
    r = m.equal_range(C2Int(12));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 4));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 4));
    r = m.equal_range(C2Int(14));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 5));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 5));
    r = m.equal_range(C2Int(16));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 6));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 6));
    r = m.equal_range(C2Int(18));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 7));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 7));
    r = m.equal_range(C2Int(20));
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 8));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 8));
    }
    {
    typedef PrivateConstructor PC;
    typedef std::map<PC, double, std::less<>> M;
    typedef std::pair<M::iterator, M::iterator> R;

    M m;
    m [ PC::make(5)  ] = 5;
    m [ PC::make(7)  ] = 6;
    m [ PC::make(9)  ] = 7;
    m [ PC::make(11) ] = 8;
    m [ PC::make(13) ] = 9;
    m [ PC::make(15) ] = 10;
    m [ PC::make(17) ] = 11;
    m [ PC::make(19) ] = 12;

    R r = m.equal_range(5);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 0));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 1));
    r = m.equal_range(7);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 1));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 2));
    r = m.equal_range(9);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 2));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 3));
    r = m.equal_range(11);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 3));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 4));
    r = m.equal_range(13);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 4));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 5));
    r = m.equal_range(15);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 5));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 6));
    r = m.equal_range(17);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 6));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 7));
    r = m.equal_range(19);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 7));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 8));
    r = m.equal_range(4);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 0));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 0));
    r = m.equal_range(6);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 1));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 1));
    r = m.equal_range(8);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 2));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 2));
    r = m.equal_range(10);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 3));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 3));
    r = m.equal_range(12);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 4));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 4));
    r = m.equal_range(14);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 5));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 5));
    r = m.equal_range(16);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 6));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 6));
    r = m.equal_range(18);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 7));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 7));
    r = m.equal_range(20);
    TC_ASSERT_EXPR(r.first == std::next(m.begin(), 8));
    TC_ASSERT_EXPR(r.second == std::next(m.begin(), 8));
    }
#endif

  return 0;
}
