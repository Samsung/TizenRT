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
// <map>

// class multimap

// template <class Key, class T, class Compare, class Allocator>
//   void
//   swap(multimap<Key, T, Compare, Allocator>& x, multimap<Key, T, Compare, Allocator>& y);

#include <map>
#include <cassert>
#include "test_macros.h"
#include "test_allocator.h"
#include "../../../test_compare.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_multimap_multimap_special_non_member_swap(void) {
    typedef std::pair<const int, double> V;
    {
    typedef std::multimap<int, double> M;
    {
        M m1;
        M m2;
        M m1_save = m1;
        M m2_save = m2;
        swap(m1, m2);
        TC_ASSERT_EXPR(m1 == m2_save);
        TC_ASSERT_EXPR(m2 == m1_save);
    }
    {
        V ar2[] =
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
        M m1;
        M m2(ar2, ar2+sizeof(ar2)/sizeof(ar2[0]));
        M m1_save = m1;
        M m2_save = m2;
        swap(m1, m2);
        TC_ASSERT_EXPR(m1 == m2_save);
        TC_ASSERT_EXPR(m2 == m1_save);
    }
    {
        V ar1[] =
        {
            V(1, 1),
            V(2, 2),
            V(3, 3),
            V(4, 4)
        };
        M m1(ar1, ar1+sizeof(ar1)/sizeof(ar1[0]));
        M m2;
        M m1_save = m1;
        M m2_save = m2;
        swap(m1, m2);
        TC_ASSERT_EXPR(m1 == m2_save);
        TC_ASSERT_EXPR(m2 == m1_save);
    }
    {
        V ar1[] =
        {
            V(1, 1),
            V(2, 2),
            V(3, 3),
            V(4, 4)
        };
        V ar2[] =
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
        M m1(ar1, ar1+sizeof(ar1)/sizeof(ar1[0]));
        M m2(ar2, ar2+sizeof(ar2)/sizeof(ar2[0]));
        M m1_save = m1;
        M m2_save = m2;
        swap(m1, m2);
        TC_ASSERT_EXPR(m1 == m2_save);
        TC_ASSERT_EXPR(m2 == m1_save);
    }
    }
    {
        typedef test_allocator<V> A;
        typedef test_less<int> C;
        typedef std::multimap<int, double, C, A> M;
        V ar1[] =
        {
            V(1, 1),
            V(2, 2),
            V(3, 3),
            V(4, 4)
        };
        V ar2[] =
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
        M m1(ar1, ar1+sizeof(ar1)/sizeof(ar1[0]), C(1), A(1, 1));
        M m2(ar2, ar2+sizeof(ar2)/sizeof(ar2[0]), C(2), A(1, 2));
        M m1_save = m1;
        M m2_save = m2;
        swap(m1, m2);
        TC_ASSERT_EXPR(m1 == m2_save);
        TC_ASSERT_EXPR(m2 == m1_save);
        TC_ASSERT_EXPR(m1.key_comp() == C(2));
        TC_ASSERT_EXPR(m1.get_allocator().get_id() == 1);
        TC_ASSERT_EXPR(m2.key_comp() == C(1));
        TC_ASSERT_EXPR(m2.get_allocator().get_id() == 2);
    }
    {
        typedef other_allocator<V> A;
        typedef test_less<int> C;
        typedef std::multimap<int, double, C, A> M;
        V ar1[] =
        {
            V(1, 1),
            V(2, 2),
            V(3, 3),
            V(4, 4)
        };
        V ar2[] =
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
        M m1(ar1, ar1+sizeof(ar1)/sizeof(ar1[0]), C(1), A(1));
        M m2(ar2, ar2+sizeof(ar2)/sizeof(ar2[0]), C(2), A(2));
        M m1_save = m1;
        M m2_save = m2;
        swap(m1, m2);
        TC_ASSERT_EXPR(m1 == m2_save);
        TC_ASSERT_EXPR(m2 == m1_save);
        TC_ASSERT_EXPR(m1.key_comp() == C(2));
        TC_ASSERT_EXPR(m1.get_allocator() == A(2));
        TC_ASSERT_EXPR(m2.key_comp() == C(1));
        TC_ASSERT_EXPR(m2.get_allocator() == A(1));
    }
#if TEST_STD_VER >= 11
    {
    typedef std::multimap<int, double, std::less<int>, min_allocator<std::pair<const int, double>>> M;
    {
        M m1;
        M m2;
        M m1_save = m1;
        M m2_save = m2;
        swap(m1, m2);
        TC_ASSERT_EXPR(m1 == m2_save);
        TC_ASSERT_EXPR(m2 == m1_save);
    }
    {
        V ar2[] =
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
        M m1;
        M m2(ar2, ar2+sizeof(ar2)/sizeof(ar2[0]));
        M m1_save = m1;
        M m2_save = m2;
        swap(m1, m2);
        TC_ASSERT_EXPR(m1 == m2_save);
        TC_ASSERT_EXPR(m2 == m1_save);
    }
    {
        V ar1[] =
        {
            V(1, 1),
            V(2, 2),
            V(3, 3),
            V(4, 4)
        };
        M m1(ar1, ar1+sizeof(ar1)/sizeof(ar1[0]));
        M m2;
        M m1_save = m1;
        M m2_save = m2;
        swap(m1, m2);
        TC_ASSERT_EXPR(m1 == m2_save);
        TC_ASSERT_EXPR(m2 == m1_save);
    }
    {
        V ar1[] =
        {
            V(1, 1),
            V(2, 2),
            V(3, 3),
            V(4, 4)
        };
        V ar2[] =
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
        M m1(ar1, ar1+sizeof(ar1)/sizeof(ar1[0]));
        M m2(ar2, ar2+sizeof(ar2)/sizeof(ar2[0]));
        M m1_save = m1;
        M m2_save = m2;
        swap(m1, m2);
        TC_ASSERT_EXPR(m1 == m2_save);
        TC_ASSERT_EXPR(m2 == m1_save);
    }
    }
    {
        typedef min_allocator<V> A;
        typedef test_less<int> C;
        typedef std::multimap<int, double, C, A> M;
        V ar1[] =
        {
            V(1, 1),
            V(2, 2),
            V(3, 3),
            V(4, 4)
        };
        V ar2[] =
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
        M m1(ar1, ar1+sizeof(ar1)/sizeof(ar1[0]), C(1), A());
        M m2(ar2, ar2+sizeof(ar2)/sizeof(ar2[0]), C(2), A());
        M m1_save = m1;
        M m2_save = m2;
        swap(m1, m2);
        TC_ASSERT_EXPR(m1 == m2_save);
        TC_ASSERT_EXPR(m2 == m1_save);
        TC_ASSERT_EXPR(m1.key_comp() == C(2));
        TC_ASSERT_EXPR(m1.get_allocator() == A());
        TC_ASSERT_EXPR(m2.key_comp() == C(1));
        TC_ASSERT_EXPR(m2.get_allocator() == A());
    }
#endif

  return 0;
}
