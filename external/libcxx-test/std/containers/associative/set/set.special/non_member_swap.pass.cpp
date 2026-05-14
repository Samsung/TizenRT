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

// void swap(set& m);

#include <set>
#include <cassert>
#include "test_macros.h"
#include "test_allocator.h"
#include "../../../test_compare.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_set_special_non_member_swap(void) {
    typedef int V;
    {
    typedef std::set<int> M;
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
            5,
            6,
            7,
            8,
            9,
            10,
            11,
            12
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
            1,
            2,
            3,
            4
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
            1,
            2,
            3,
            4
        };
        V ar2[] =
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
        typedef std::set<int, C, A> M;
        V ar1[] =
        {
            1,
            2,
            3,
            4
        };
        V ar2[] =
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
        typedef std::set<int, C, A> M;
        V ar1[] =
        {
            1,
            2,
            3,
            4
        };
        V ar2[] =
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

  return 0;
}
