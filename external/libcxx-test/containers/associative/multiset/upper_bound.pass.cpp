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

// class multiset

//       iterator upper_bound(const key_type& k);
// const_iterator upper_bound(const key_type& k) const;

#include <set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "private_constructor.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_upper_bound(void) {
    {
    typedef int V;
    typedef std::multiset<int> M;
    {
        typedef M::iterator R;
        V ar[] =
        {
            5,
            5,
            5,
            7,
            7,
            7,
            9,
            9,
            9
        };
        M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.upper_bound(4);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 0));
        r = m.upper_bound(5);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.upper_bound(6);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.upper_bound(7);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.upper_bound(8);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.upper_bound(9);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 9));
        r = m.upper_bound(11);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 9));
    }
    {
        typedef M::const_iterator R;
        V ar[] =
        {
            5,
            5,
            5,
            7,
            7,
            7,
            9,
            9,
            9
        };
        const M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.upper_bound(4);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 0));
        r = m.upper_bound(5);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.upper_bound(6);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.upper_bound(7);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.upper_bound(8);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.upper_bound(9);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 9));
        r = m.upper_bound(11);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 9));
    }
    }
#if TEST_STD_VER >= 11
    {
    typedef int V;
    typedef std::multiset<int, std::less<int>, min_allocator<int>> M;
    {
        typedef M::iterator R;
        V ar[] =
        {
            5,
            5,
            5,
            7,
            7,
            7,
            9,
            9,
            9
        };
        M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.upper_bound(4);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 0));
        r = m.upper_bound(5);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.upper_bound(6);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.upper_bound(7);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.upper_bound(8);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.upper_bound(9);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 9));
        r = m.upper_bound(11);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 9));
    }
    {
        typedef M::const_iterator R;
        V ar[] =
        {
            5,
            5,
            5,
            7,
            7,
            7,
            9,
            9,
            9
        };
        const M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        R r = m.upper_bound(4);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 0));
        r = m.upper_bound(5);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.upper_bound(6);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
        r = m.upper_bound(7);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.upper_bound(8);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
        r = m.upper_bound(9);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 9));
        r = m.upper_bound(11);
        TC_ASSERT_EXPR(r == std::next(m.begin(), 9));
    }
    }
#endif
#if TEST_STD_VER > 11
    {
    typedef int V;
    typedef std::multiset<V, std::less<>> M;

    typedef M::iterator R;
    V ar[] =
    {
        5,
        5,
        5,
        7,
        7,
        7,
        9,
        9,
        9
    };
    M m(ar, ar+sizeof(ar)/sizeof(ar[0]));
    R r = m.upper_bound(4);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 0));
    r = m.upper_bound(5);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
    r = m.upper_bound(6);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
    r = m.upper_bound(7);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
    r = m.upper_bound(8);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
    r = m.upper_bound(9);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 9));
    r = m.upper_bound(11);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 9));
    }

    {
    typedef PrivateConstructor V;
    typedef std::multiset<V, std::less<>> M;

    typedef M::iterator R;
    M m;
    m.insert ( V::make ( 5 ));
    m.insert ( V::make ( 5 ));
    m.insert ( V::make ( 5 ));
    m.insert ( V::make ( 7 ));
    m.insert ( V::make ( 7 ));
    m.insert ( V::make ( 7 ));
    m.insert ( V::make ( 9 ));
    m.insert ( V::make ( 9 ));
    m.insert ( V::make ( 9 ));

    R r = m.upper_bound(4);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 0));
    r = m.upper_bound(5);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
    r = m.upper_bound(6);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 3));
    r = m.upper_bound(7);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
    r = m.upper_bound(8);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 6));
    r = m.upper_bound(9);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 9));
    r = m.upper_bound(11);
    TC_ASSERT_EXPR(r == std::next(m.begin(), 9));
    }
#endif

  return 0;
}
