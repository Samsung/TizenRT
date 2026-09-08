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

// template <class... Args>
//     iterator emplace_after(const_iterator p, Args&&... args);

#include <forward_list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "Emplaceable.h"

int tc_libcxx_containers_forwardlist_modifiers_emplace_after(void)
{
    {
        typedef Emplaceable T;
        typedef std::forward_list<T> C;
        typedef C::iterator I;
        C c;
        I i = c.emplace_after(c.cbefore_begin());
        TC_ASSERT_EXPR(i == c.begin());
        TC_ASSERT_EXPR(c.front() == Emplaceable());
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 1);

        i = c.emplace_after(c.cbegin(), 1, 2.5);
        TC_ASSERT_EXPR(i == next(c.begin()));
        TC_ASSERT_EXPR(c.front() == Emplaceable());
        TC_ASSERT_EXPR(*next(c.begin()) == Emplaceable(1, 2.5));
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 2);

        i = c.emplace_after(next(c.cbegin()), 2, 3.5);
        TC_ASSERT_EXPR(i == next(c.begin(), 2));
        TC_ASSERT_EXPR(c.front() == Emplaceable());
        TC_ASSERT_EXPR(*next(c.begin()) == Emplaceable(1, 2.5));
        TC_ASSERT_EXPR(*next(c.begin(), 2) == Emplaceable(2, 3.5));
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 3);

        i = c.emplace_after(c.cbegin(), 3, 4.5);
        TC_ASSERT_EXPR(i == next(c.begin()));
        TC_ASSERT_EXPR(c.front() == Emplaceable());
        TC_ASSERT_EXPR(*next(c.begin(), 1) == Emplaceable(3, 4.5));
        TC_ASSERT_EXPR(*next(c.begin(), 2) == Emplaceable(1, 2.5));
        TC_ASSERT_EXPR(*next(c.begin(), 3) == Emplaceable(2, 3.5));
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 4);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
