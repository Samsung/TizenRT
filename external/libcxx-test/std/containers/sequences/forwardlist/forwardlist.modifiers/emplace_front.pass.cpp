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

// template <class... Args> reference emplace_front(Args&&... args);
// return type is 'reference' in C++17; 'static int' before

#include <forward_list>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

#include "Emplaceable.h"

int tc_libcxx_containers_forwardlist_modifiers_emplace_front(void)
{
    {
        typedef Emplaceable T;
        typedef std::forward_list<T> C;
        C c;
#if TEST_STD_VER > 14
        T& r1 = c.emplace_front();
        TC_ASSERT_EXPR(c.front() == Emplaceable());
        TC_ASSERT_EXPR(&r1 == &c.front());
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 1);
        T& r2 = c.emplace_front(1, 2.5);
        TC_ASSERT_EXPR(c.front() == Emplaceable(1, 2.5));
        TC_ASSERT_EXPR(&r2 == &c.front());
#else
        c.emplace_front();
        TC_ASSERT_EXPR(c.front() == Emplaceable());
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 1);
        c.emplace_front(1, 2.5);
        TC_ASSERT_EXPR(c.front() == Emplaceable(1, 2.5));
#endif
        TC_ASSERT_EXPR(*next(c.begin()) == Emplaceable());
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 2);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
