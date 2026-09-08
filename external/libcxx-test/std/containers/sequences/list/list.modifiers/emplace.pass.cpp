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

// template <class... Args> static int emplace(const_iterator p, Args&&... args);


#include <list>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

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

int tc_libcxx_containers_list_modifiers_emplace(void)
{
    {
    std::list<A> c;
    c.emplace(c.cbegin(), 2, 3.5);
    TC_ASSERT_EXPR(c.size() == 1);
    TC_ASSERT_EXPR(c.front().geti() == 2);
    TC_ASSERT_EXPR(c.front().getd() == 3.5);
    c.emplace(c.cend(), 3, 4.5);
    TC_ASSERT_EXPR(c.size() == 2);
    TC_ASSERT_EXPR(c.front().geti() == 2);
    TC_ASSERT_EXPR(c.front().getd() == 3.5);
    TC_ASSERT_EXPR(c.back().geti() == 3);
    TC_ASSERT_EXPR(c.back().getd() == 4.5);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
