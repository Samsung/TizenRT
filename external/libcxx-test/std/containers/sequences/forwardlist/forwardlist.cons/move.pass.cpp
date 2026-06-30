//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
#include "test_macros.h"
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
        for (C::const_iterator i = c.begin(), e = c.end(); i != e; ++i)
        {
            TC_ASSERT_EXPR(*i == n);
            ++n;
        }
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
        for (C::const_iterator i = c.begin(), e = c.end(); i != e; ++i)
        {
            TC_ASSERT_EXPR(*i == n);
            ++n;
        }
        TC_ASSERT_EXPR(n == std::end(t) - std::begin(t));
        TC_ASSERT_EXPR(c0.empty());
        TC_ASSERT_EXPR(c.get_allocator() == A(10));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
