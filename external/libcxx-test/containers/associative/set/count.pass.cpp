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
// <set>

// class set

// size_type count(const key_type& k) const;

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "private_constructor.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_count(void) {
    {
        typedef int V;
        typedef std::set<int> M;
        typedef M::size_type R;
        V ar[] =
        {
            5,
            6,
            7,
            8,
            9,
            10,
            11,
            12
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
#if TEST_STD_VER >= 11
    {
        typedef int V;
        typedef std::set<int, std::less<int>, min_allocator<int>> M;
        typedef M::size_type R;
        V ar[] =
        {
            5,
            6,
            7,
            8,
            9,
            10,
            11,
            12
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
#endif
#if TEST_STD_VER > 11
    {
        typedef int V;
        typedef std::set<int, std::less<>> M;
        typedef M::size_type R;
        V ar[] =
        {
            5,
            6,
            7,
            8,
            9,
            10,
            11,
            12
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
    {
    typedef PrivateConstructor V;
    typedef std::set<V, std::less<>> M;
        typedef M::size_type R;

    M m;
    m.insert ( V::make ( 5 ));
    m.insert ( V::make ( 6 ));
    m.insert ( V::make ( 7 ));
    m.insert ( V::make ( 8 ));
    m.insert ( V::make ( 9 ));
    m.insert ( V::make ( 10 ));
    m.insert ( V::make ( 11 ));
    m.insert ( V::make ( 12 ));

    const M& mc = m;

    R r = mc.count(5);
    TC_ASSERT_EXPR(r == 1);
    r = mc.count(6);
    TC_ASSERT_EXPR(r == 1);
    r = mc.count(7);
    TC_ASSERT_EXPR(r == 1);
    r = mc.count(8);
    TC_ASSERT_EXPR(r == 1);
    r = mc.count(9);
    TC_ASSERT_EXPR(r == 1);
    r = mc.count(10);
    TC_ASSERT_EXPR(r == 1);
    r = mc.count(11);
    TC_ASSERT_EXPR(r == 1);
    r = mc.count(12);
    TC_ASSERT_EXPR(r == 1);
    r = mc.count(4);
    TC_ASSERT_EXPR(r == 0);
    }
#endif


  return 0;
}
