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

// class multimap

// size_type count(const key_type& k) const;

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "private_constructor.hpp"
#include "is_transparent.h"

int tc_libcxx_containers_multimap_ops_count(void)
{
    typedef std::pair<const int, double> V;
    {
    typedef std::multimap<int, double> M;
    {
        typedef M::size_type R;
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
        R r = m.count(4);
        TC_ASSERT_EXPR(r == 0);
        r = m.count(5);
        TC_ASSERT_EXPR(r == 3);
        r = m.count(6);
        TC_ASSERT_EXPR(r == 0);
        r = m.count(7);
        TC_ASSERT_EXPR(r == 3);
        r = m.count(8);
        TC_ASSERT_EXPR(r == 0);
        r = m.count(9);
        TC_ASSERT_EXPR(r == 3);
        r = m.count(10);
        TC_ASSERT_EXPR(r == 0);
    }
    }

#if TEST_STD_VER > 11
    {
    typedef std::multimap<int, double, std::less<>> M;
    typedef M::size_type R;
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
    R r = m.count(4);
    TC_ASSERT_EXPR(r == 0);
    r = m.count(5);
    TC_ASSERT_EXPR(r == 3);
    r = m.count(6);
    TC_ASSERT_EXPR(r == 0);
    r = m.count(7);
    TC_ASSERT_EXPR(r == 3);
    r = m.count(8);
    TC_ASSERT_EXPR(r == 0);
    r = m.count(9);
    TC_ASSERT_EXPR(r == 3);
    r = m.count(10);
    TC_ASSERT_EXPR(r == 0);

    r = m.count(C2Int(4));
    TC_ASSERT_EXPR(r == 0);
    r = m.count(C2Int(5));
    TC_ASSERT_EXPR(r == 3);
    r = m.count(C2Int(6));
    TC_ASSERT_EXPR(r == 0);
    r = m.count(C2Int(7));
    TC_ASSERT_EXPR(r == 3);
    r = m.count(C2Int(8));
    TC_ASSERT_EXPR(r == 0);
    r = m.count(C2Int(9));
    TC_ASSERT_EXPR(r == 3);
    r = m.count(C2Int(10));
    TC_ASSERT_EXPR(r == 0);
    }

    {
    typedef PrivateConstructor PC;
    typedef std::multimap<PC, double, std::less<>> M;
    typedef M::size_type R;

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

    R r = m.count(4);
    TC_ASSERT_EXPR(r == 0);
    r = m.count(5);
    TC_ASSERT_EXPR(r == 3);
    r = m.count(6);
    TC_ASSERT_EXPR(r == 0);
    r = m.count(7);
    TC_ASSERT_EXPR(r == 3);
    r = m.count(8);
    TC_ASSERT_EXPR(r == 0);
    r = m.count(9);
    TC_ASSERT_EXPR(r == 3);
    r = m.count(10);
    TC_ASSERT_EXPR(r == 0);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
