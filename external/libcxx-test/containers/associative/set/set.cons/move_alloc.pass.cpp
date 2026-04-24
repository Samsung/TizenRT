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
// UNSUPPORTED: c++03

// <set>

// class set

// set(set&& s, const allocator_type& a);

#include <set>
#include <cassert>
#include <iterator>

#include "test_macros.h"
#include "MoveOnly.h"
#include "../../../test_compare.h"
#include "test_allocator.h"
#include "Counter.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_set_set_cons_move_alloc(void) {
    {
        typedef MoveOnly V;
        typedef test_less<MoveOnly> C;
        typedef test_allocator<V> A;
        typedef std::set<MoveOnly, C, A> M;
        typedef std::move_iterator<V*> I;
        V a1[] =
        {
            V(1),
            V(1),
            V(1),
            V(2),
            V(2),
            V(2),
            V(3),
            V(3),
            V(3)
        };
        M m1(I(a1), I(a1+sizeof(a1)/sizeof(a1[0])), C(5), A(7));
        V a2[] =
        {
            V(1),
            V(1),
            V(1),
            V(2),
            V(2),
            V(2),
            V(3),
            V(3),
            V(3)
        };
        M m2(I(a2), I(a2+sizeof(a2)/sizeof(a2[0])), C(5), A(7));
        M m3(std::move(m1), A(7));
        TC_ASSERT_EXPR(m3 == m2);
        TC_ASSERT_EXPR(m3.get_allocator() == A(7));
        TC_ASSERT_EXPR(m3.key_comp() == C(5));
        LIBCPP_ASSERT(m1.empty());
    }
    {
        typedef MoveOnly V;
        typedef test_less<MoveOnly> C;
        typedef test_allocator<V> A;
        typedef std::set<MoveOnly, C, A> M;
        typedef std::move_iterator<V*> I;
        V a1[] =
        {
            V(1),
            V(1),
            V(1),
            V(2),
            V(2),
            V(2),
            V(3),
            V(3),
            V(3)
        };
        M m1(I(a1), I(a1+sizeof(a1)/sizeof(a1[0])), C(5), A(7));
        V a2[] =
        {
            V(1),
            V(1),
            V(1),
            V(2),
            V(2),
            V(2),
            V(3),
            V(3),
            V(3)
        };
        M m2(I(a2), I(a2+sizeof(a2)/sizeof(a2[0])), C(5), A(7));
        M m3(std::move(m1), A(5));
        TC_ASSERT_EXPR(m3 == m2);
        TC_ASSERT_EXPR(m3.get_allocator() == A(5));
        TC_ASSERT_EXPR(m3.key_comp() == C(5));
        LIBCPP_ASSERT(m1.empty());
    }
    {
        typedef MoveOnly V;
        typedef test_less<MoveOnly> C;
        typedef other_allocator<V> A;
        typedef std::set<MoveOnly, C, A> M;
        typedef std::move_iterator<V*> I;
        V a1[] =
        {
            V(1),
            V(1),
            V(1),
            V(2),
            V(2),
            V(2),
            V(3),
            V(3),
            V(3)
        };
        M m1(I(a1), I(a1+sizeof(a1)/sizeof(a1[0])), C(5), A(7));
        V a2[] =
        {
            V(1),
            V(1),
            V(1),
            V(2),
            V(2),
            V(2),
            V(3),
            V(3),
            V(3)
        };
        M m2(I(a2), I(a2+sizeof(a2)/sizeof(a2[0])), C(5), A(7));
        M m3(std::move(m1), A(5));
        TC_ASSERT_EXPR(m3 == m2);
        TC_ASSERT_EXPR(m3.get_allocator() == A(5));
        TC_ASSERT_EXPR(m3.key_comp() == C(5));
        LIBCPP_ASSERT(m1.empty());
    }
    {
        typedef Counter<int> V;
        typedef std::less<V> C;
        typedef test_allocator<V> A;
        typedef std::set<V, C, A> M;
        typedef V* I;
        Counter_base::gConstructed = 0;
        {
            V a1[] =
            {
            V(1),
            V(1),
            V(1),
            V(2),
            V(2),
            V(2),
            V(3),
            V(3),
            V(3)
            };
            const std::size_t num = sizeof(a1)/sizeof(a1[0]);
            TC_ASSERT_EXPR(Counter_base::gConstructed == num);

            M m1(I(a1), I(a1+num), C(), A());
            TC_ASSERT_EXPR(Counter_base::gConstructed == 3+num);

            M m2(m1);
            TC_ASSERT_EXPR(m2 == m1);
            TC_ASSERT_EXPR(Counter_base::gConstructed == 6+num);

            M m3(std::move(m1), A());
            TC_ASSERT_EXPR(m3 == m2);
            LIBCPP_ASSERT(m1.empty());
            TC_ASSERT_EXPR(Counter_base::gConstructed >= (int)(6+num));
            TC_ASSERT_EXPR(Counter_base::gConstructed <= (int)(m1.size()+6+num));

            {
            M m4(std::move(m2), A(5));
            TC_ASSERT_EXPR(Counter_base::gConstructed >= (int)(6+num));
            TC_ASSERT_EXPR(Counter_base::gConstructed <= (int)(m1.size()+m2.size()+6+num));
            TC_ASSERT_EXPR(m4 == m3);
            LIBCPP_ASSERT(m2.empty());
            }
            TC_ASSERT_EXPR(Counter_base::gConstructed >= (int)(3+num));
            TC_ASSERT_EXPR(Counter_base::gConstructed <= (int)(m1.size()+m2.size()+3+num));
        }
        TC_ASSERT_EXPR(Counter_base::gConstructed == 0);
    }


  return 0;
}
