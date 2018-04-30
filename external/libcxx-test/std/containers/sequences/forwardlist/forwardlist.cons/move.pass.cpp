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

// <forward_list>

// forward_list(forward_list&& x);

#include <forward_list>
#include <cassert>
#include "libcxx_tc_common.h"
#include <iterator>

#include "test_allocator.h"
#include "MoveOnly.h"

int tc_libcxx_containers_forwardlist_cons_move(void)
{
    {
        typedef MoveOnly T;
        typedef test_allocator<T> A;
        typedef std::forward_list<T, A> C;
        T t[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        typedef std::move_iterator<T*> I;
        C c0(I(std::begin(t)), I(std::end(t)), A(10));
        C c = std::move(c0);
        int n = 0;
        for (C::const_iterator i = c.begin(), e = c.end(); i != e; ++i, ++n)
            TC_ASSERT_EXPR(*i == n);
        TC_ASSERT_EXPR(n == std::end(t) - std::begin(t));
        TC_ASSERT_EXPR(c0.empty());
        TC_ASSERT_EXPR(c.get_allocator() == A(10));
    }
    {
        typedef MoveOnly T;
        typedef other_allocator<T> A;
        typedef std::forward_list<T, A> C;
        T t[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        typedef std::move_iterator<T*> I;
        C c0(I(std::begin(t)), I(std::end(t)), A(10));
        C c = std::move(c0);
        int n = 0;
        for (C::const_iterator i = c.begin(), e = c.end(); i != e; ++i, ++n)
            TC_ASSERT_EXPR(*i == n);
        TC_ASSERT_EXPR(n == std::end(t) - std::begin(t));
        TC_ASSERT_EXPR(c0.empty());
        TC_ASSERT_EXPR(c.get_allocator() == A(10));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
