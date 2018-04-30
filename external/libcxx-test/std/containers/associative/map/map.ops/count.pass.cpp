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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <map>

// class map

// size_type count(const key_type& k) const;

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "private_constructor.hpp"
#include "is_transparent.h"

int tc_libcxx_containers_map_ops_count(void)
{
    {
    typedef std::pair<const int, double> V;
    typedef std::map<int, double> M;
    {
        typedef M::size_type R;
        V ar[] =
        {
            V(5, 5),
            V(6, 6),
            V(7, 7),
            V(8, 8),
            V(9, 9),
            V(10, 10),
            V(11, 11),
            V(12, 12)
        };
        const M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.count(5);
        TC_ASSERT_EXPR(r == 1);
        r = m.count(6);
        TC_ASSERT_EXPR(r == 1);
        r = m.count(7);
        TC_ASSERT_EXPR(r == 1);
        r = m.count(8);
        TC_ASSERT_EXPR(r == 1);
        r = m.count(9);
        TC_ASSERT_EXPR(r == 1);
        r = m.count(10);
        TC_ASSERT_EXPR(r == 1);
        r = m.count(11);
        TC_ASSERT_EXPR(r == 1);
        r = m.count(12);
        TC_ASSERT_EXPR(r == 1);
        r = m.count(4);
        TC_ASSERT_EXPR(r == 0);
    }
    }
#if TEST_STD_VER > 11
    {
    typedef std::pair<const int, double> V;
    typedef std::map<int, double, std::less <>> M;
    typedef M::size_type R;

    V ar[] =
    {
        V(5, 5),
        V(6, 6),
        V(7, 7),
        V(8, 8),
        V(9, 9),
        V(10, 10),
        V(11, 11),
        V(12, 12)
    };
    const M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
    R r = m.count(5);
    TC_ASSERT_EXPR(r == 1);
    r = m.count(6);
    TC_ASSERT_EXPR(r == 1);
    r = m.count(7);
    TC_ASSERT_EXPR(r == 1);
    r = m.count(8);
    TC_ASSERT_EXPR(r == 1);
    r = m.count(9);
    TC_ASSERT_EXPR(r == 1);
    r = m.count(10);
    TC_ASSERT_EXPR(r == 1);
    r = m.count(11);
    TC_ASSERT_EXPR(r == 1);
    r = m.count(12);
    TC_ASSERT_EXPR(r == 1);
    r = m.count(4);
    TC_ASSERT_EXPR(r == 0);

    r = m.count(C2Int(5));
    TC_ASSERT_EXPR(r == 1);
    r = m.count(C2Int(6));
    TC_ASSERT_EXPR(r == 1);
    r = m.count(C2Int(7));
    TC_ASSERT_EXPR(r == 1);
    r = m.count(C2Int(8));
    TC_ASSERT_EXPR(r == 1);
    r = m.count(C2Int(9));
    TC_ASSERT_EXPR(r == 1);
    r = m.count(C2Int(10));
    TC_ASSERT_EXPR(r == 1);
    r = m.count(C2Int(11));
    TC_ASSERT_EXPR(r == 1);
    r = m.count(C2Int(12));
    TC_ASSERT_EXPR(r == 1);
    r = m.count(C2Int(4));
    TC_ASSERT_EXPR(r == 0);
    }

    {
    typedef PrivateConstructor PC;
    typedef std::map<PC, double, std::less<>> M;
    typedef M::size_type R;

    M m;
    m [ PC::make(5)  ] = 5;
    m [ PC::make(6)  ] = 6;
    m [ PC::make(7)  ] = 7;
    m [ PC::make(8)  ] = 8;
    m [ PC::make(9)  ] = 9;
    m [ PC::make(10) ] = 10;
    m [ PC::make(11) ] = 11;
    m [ PC::make(12) ] = 12;

    R r = m.count(5);
    TC_ASSERT_EXPR(r == 1);
    r = m.count(6);
    TC_ASSERT_EXPR(r == 1);
    r = m.count(7);
    TC_ASSERT_EXPR(r == 1);
    r = m.count(8);
    TC_ASSERT_EXPR(r == 1);
    r = m.count(9);
    TC_ASSERT_EXPR(r == 1);
    r = m.count(10);
    TC_ASSERT_EXPR(r == 1);
    r = m.count(11);
    TC_ASSERT_EXPR(r == 1);
    r = m.count(12);
    TC_ASSERT_EXPR(r == 1);
    r = m.count(4);
    TC_ASSERT_EXPR(r == 0);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
