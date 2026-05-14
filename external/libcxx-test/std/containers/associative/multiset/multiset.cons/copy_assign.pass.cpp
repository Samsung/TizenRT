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

// multiset& operator=(const multiset& s);

#include <set>
#include <cassert>

#include "test_macros.h"
#include "../../../test_compare.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multiset_multiset_cons_copy_assign(void) {
    {
        typedef int V;
        V ar[] =
        {
            1,
            1,
            1,
            2,
            2,
            2,
            3,
            3,
            3
        };
        typedef test_less<int> C;
        typedef test_allocator<V> A;
        std::multiset<int, C, A> mo(ar, ar+sizeof(ar)/sizeof(ar[0]), C(5), A(2));
        std::multiset<int, C, A> m(ar, ar+sizeof(ar)/sizeof(ar[0])/2, C(3), A(7));
        m = mo;
        TC_ASSERT_EXPR(m.get_allocator() == A(7));
        TC_ASSERT_EXPR(m.key_comp() == C(5));
        TC_ASSERT_EXPR(m.size() == 9);
        TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 9);
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 5) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 6) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 7) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 8) == 3);

        TC_ASSERT_EXPR(mo.get_allocator() == A(2));
        TC_ASSERT_EXPR(mo.key_comp() == C(5));
        TC_ASSERT_EXPR(mo.size() == 9);
        TC_ASSERT_EXPR(std::distance(mo.begin(), mo.end()) == 9);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 0) == 1);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 1) == 1);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 2) == 1);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 3) == 2);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 4) == 2);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 5) == 2);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 6) == 3);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 7) == 3);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 8) == 3);
    }
    {
        typedef int V;
        const V ar[] =
        {
            1,
            1,
            1,
            2,
            2,
            2,
            3,
            3,
            3
        };
        std::multiset<int> m(ar, ar+sizeof(ar)/sizeof(ar[0]));
        std::multiset<int> *p = &m;
        m = *p;
        TC_ASSERT_EXPR(m.size() == 9);
        TC_ASSERT_EXPR(std::equal(m.begin(), m.end(), ar));
    }
    {
        typedef int V;
        V ar[] =
        {
            1,
            1,
            1,
            2,
            2,
            2,
            3,
            3,
            3
        };
        typedef test_less<int> C;
        typedef other_allocator<V> A;
        std::multiset<int, C, A> mo(ar, ar+sizeof(ar)/sizeof(ar[0]), C(5), A(2));
        std::multiset<int, C, A> m(ar, ar+sizeof(ar)/sizeof(ar[0])/2, C(3), A(7));
        m = mo;
        TC_ASSERT_EXPR(m.get_allocator() == A(2));
        TC_ASSERT_EXPR(m.key_comp() == C(5));
        TC_ASSERT_EXPR(m.size() == 9);
        TC_ASSERT_EXPR(std::distance(m.begin(), m.end()) == 9);
        TC_ASSERT_EXPR(*std::next(m.begin(), 0) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 1) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 2) == 1);
        TC_ASSERT_EXPR(*std::next(m.begin(), 3) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 4) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 5) == 2);
        TC_ASSERT_EXPR(*std::next(m.begin(), 6) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 7) == 3);
        TC_ASSERT_EXPR(*std::next(m.begin(), 8) == 3);

        TC_ASSERT_EXPR(mo.get_allocator() == A(2));
        TC_ASSERT_EXPR(mo.key_comp() == C(5));
        TC_ASSERT_EXPR(mo.size() == 9);
        TC_ASSERT_EXPR(std::distance(mo.begin(), mo.end()) == 9);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 0) == 1);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 1) == 1);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 2) == 1);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 3) == 2);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 4) == 2);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 5) == 2);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 6) == 3);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 7) == 3);
        TC_ASSERT_EXPR(*std::next(mo.begin(), 8) == 3);
    }

  return 0;
}
