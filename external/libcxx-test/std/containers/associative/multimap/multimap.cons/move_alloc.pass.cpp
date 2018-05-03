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

// UNSUPPORTED: c++98, c++03

// <map>

// class multimap

// multimap(multimap&& m, const allocator_type& a);

#include <map>
#include <cassert>
#include "libcxx_tc_common.h"

#include "MoveOnly.h"
#include "test_compare.h"
#include "test_allocator.h"
#include "Counter.h"

int tc_libcxx_containers_multimap_cons_move_alloc(void)
{
    {
        typedef std::pair<MoveOnly, MoveOnly> V;
        typedef std::pair<const MoveOnly, MoveOnly> VC;
        typedef test_compare<std::less<MoveOnly> > C;
        typedef test_allocator<VC> A;
        typedef std::multimap<MoveOnly, MoveOnly, C, A> M;
        typedef std::move_iterator<V*> I;
        V a1[] =
        {
            V(1, 1),
            V(1, 2),
            V(1, 3),
            V(2, 1),
            V(2, 2),
            V(2, 3),
            V(3, 1),
            V(3, 2),
            V(3, 3)
        };
        M m1(I(a1), I(a1+sizeof(a1)/sizeof(a1[0])), C(5), A(7));
        V a2[] =
        {
            V(1, 1),
            V(1, 2),
            V(1, 3),
            V(2, 1),
            V(2, 2),
            V(2, 3),
            V(3, 1),
            V(3, 2),
            V(3, 3)
        };
        M m2(I(a2), I(a2+sizeof(a2)/sizeof(a2[0])), C(5), A(7));
        M m3(std::move(m1), A(7));
        TC_ASSERT_EXPR(m3 == m2);
        TC_ASSERT_EXPR(m3.get_allocator() == A(7));
        TC_ASSERT_EXPR(m3.key_comp() == C(5));
        TC_ASSERT_EXPR(m1.empty());
    }
    {
        typedef std::pair<MoveOnly, MoveOnly> V;
        typedef std::pair<const MoveOnly, MoveOnly> VC;
        typedef test_compare<std::less<MoveOnly> > C;
        typedef test_allocator<VC> A;
        typedef std::multimap<MoveOnly, MoveOnly, C, A> M;
        typedef std::move_iterator<V*> I;
        V a1[] =
        {
            V(1, 1),
            V(1, 2),
            V(1, 3),
            V(2, 1),
            V(2, 2),
            V(2, 3),
            V(3, 1),
            V(3, 2),
            V(3, 3)
        };
        M m1(I(a1), I(a1+sizeof(a1)/sizeof(a1[0])), C(5), A(7));
        V a2[] =
        {
            V(1, 1),
            V(1, 2),
            V(1, 3),
            V(2, 1),
            V(2, 2),
            V(2, 3),
            V(3, 1),
            V(3, 2),
            V(3, 3)
        };
        M m2(I(a2), I(a2+sizeof(a2)/sizeof(a2[0])), C(5), A(7));
        M m3(std::move(m1), A(5));
        TC_ASSERT_EXPR(m3 == m2);
        TC_ASSERT_EXPR(m3.get_allocator() == A(5));
        TC_ASSERT_EXPR(m3.key_comp() == C(5));
        TC_ASSERT_EXPR(m1.empty());
    }
    {
        typedef std::pair<MoveOnly, MoveOnly> V;
        typedef std::pair<const MoveOnly, MoveOnly> VC;
        typedef test_compare<std::less<MoveOnly> > C;
        typedef other_allocator<VC> A;
        typedef std::multimap<MoveOnly, MoveOnly, C, A> M;
        typedef std::move_iterator<V*> I;
        V a1[] =
        {
            V(1, 1),
            V(1, 2),
            V(1, 3),
            V(2, 1),
            V(2, 2),
            V(2, 3),
            V(3, 1),
            V(3, 2),
            V(3, 3)
        };
        M m1(I(a1), I(a1+sizeof(a1)/sizeof(a1[0])), C(5), A(7));
        V a2[] =
        {
            V(1, 1),
            V(1, 2),
            V(1, 3),
            V(2, 1),
            V(2, 2),
            V(2, 3),
            V(3, 1),
            V(3, 2),
            V(3, 3)
        };
        M m2(I(a2), I(a2+sizeof(a2)/sizeof(a2[0])), C(5), A(7));
        M m3(std::move(m1), A(5));
        TC_ASSERT_EXPR(m3 == m2);
        TC_ASSERT_EXPR(m3.get_allocator() == A(5));
        TC_ASSERT_EXPR(m3.key_comp() == C(5));
        TC_ASSERT_EXPR(m1.empty());
    }
    {
        typedef Counter<int> T;
        typedef std::pair<int, T> V;
        typedef std::pair<const int, T> VC;
        typedef test_allocator<VC> A;
        typedef std::less<int> C;
        typedef std::multimap<const int, T, C, A> M;
        typedef V* I;
        Counter_base::gConstructed = 0;
        {
            V a1[] =
            {
                V(1, 1),
                V(1, 2),
                V(1, 3),
                V(2, 1),
                V(2, 2),
                V(2, 3),
                V(3, 1),
                V(3, 2),
                V(3, 3)
            };
            const size_t num = sizeof(a1)/sizeof(a1[0]);
            TC_ASSERT_EXPR(Counter_base::gConstructed == num);

            M m1(I(a1), I(a1+num), C(), A());
            TC_ASSERT_EXPR(Counter_base::gConstructed == 2*num);

            M m2(m1);
            TC_ASSERT_EXPR(m2 == m1);
            TC_ASSERT_EXPR(Counter_base::gConstructed == 3*num);

            M m3(std::move(m1), A());
            TC_ASSERT_EXPR(m3 == m2);
            TC_ASSERT_EXPR(m1.empty());
            TC_ASSERT_EXPR(Counter_base::gConstructed == 3*num);

            {
            M m4(std::move(m2), A(5));
            TC_ASSERT_EXPR(Counter_base::gConstructed == 3*num);
            TC_ASSERT_EXPR(m4 == m3);
            TC_ASSERT_EXPR(m2.empty());
            }
            TC_ASSERT_EXPR(Counter_base::gConstructed == 2*num);
        }
        TC_ASSERT_EXPR(Counter_base::gConstructed == 0);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
