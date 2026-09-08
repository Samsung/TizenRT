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

// <list>

// template <class... Args> reference emplace_front(Args&&... args);
// return type is 'reference' in C++17; 'static int' before

#include <list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

class A
{
    int i_;
    double d_;

    A(const A&);
    A& operator=(const A&);
public:
    A(int i, double d)
        : i_(i), d_(d) {}

    int geti() const {return i_;}
    double getd() const {return d_;}
};

int tc_libcxx_containers_list_modifiers_emplace_front(void)
{
    {
    std::list<A> c;
#if TEST_STD_VER > 14
    A& r1 = c.emplace_front(2, 3.5);
    TC_ASSERT_EXPR(c.size() == 1);
    TC_ASSERT_EXPR(&r1 == &c.front());
    TC_ASSERT_EXPR(c.front().geti() == 2);
    TC_ASSERT_EXPR(c.front().getd() == 3.5);
    A& r2 = c.emplace_front(3, 4.5);
    TC_ASSERT_EXPR(c.size() == 2);
    TC_ASSERT_EXPR(&r2 == &c.front());
#else
    c.emplace_front(2, 3.5);
    TC_ASSERT_EXPR(c.size() == 1);
    TC_ASSERT_EXPR(c.front().geti() == 2);
    TC_ASSERT_EXPR(c.front().getd() == 3.5);
    c.emplace_front(3, 4.5);
    TC_ASSERT_EXPR(c.size() == 2);
#endif
    TC_ASSERT_EXPR(c.front().geti() == 3);
    TC_ASSERT_EXPR(c.front().getd() == 4.5);
    TC_ASSERT_EXPR(c.back().geti() == 2);
    TC_ASSERT_EXPR(c.back().getd() == 3.5);
    }

    TC_SUCCESS_RESULT();
    return 0;
}
