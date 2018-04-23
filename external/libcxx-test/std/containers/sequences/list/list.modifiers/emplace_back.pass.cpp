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

// <list>

// template <class... Args> reference emplace_back(Args&&... args);
// return type is 'reference' in C++17; 'static int' before

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

int tc_libcxx_containers_list_modifiers_emplace_back(void)
{
    {
    std::list<A> c;
#if TEST_STD_VER > 14
    A& r1 = c.emplace_back(2, 3.5);
    TC_ASSERT_EXPR(c.size() == 1);
    TC_ASSERT_EXPR(&r1 == &c.back());
    TC_ASSERT_EXPR(c.front().geti() == 2);
    TC_ASSERT_EXPR(c.front().getd() == 3.5);
    A& r2 = c.emplace_back(3, 4.5);
    TC_ASSERT_EXPR(c.size() == 2);
    TC_ASSERT_EXPR(&r2 == &c.back());
#else
    c.emplace_back(2, 3.5);
    TC_ASSERT_EXPR(c.size() == 1);
    TC_ASSERT_EXPR(c.front().geti() == 2);
    TC_ASSERT_EXPR(c.front().getd() == 3.5);
    c.emplace_back(3, 4.5);
    TC_ASSERT_EXPR(c.size() == 2);
#endif
    TC_ASSERT_EXPR(c.front().geti() == 2);
    TC_ASSERT_EXPR(c.front().getd() == 3.5);
    TC_ASSERT_EXPR(c.back().geti() == 3);
    TC_ASSERT_EXPR(c.back().getd() == 4.5);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
