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

// <vector>

// template <class... Args> reference emplace_back(Args&&... args);
// return type is 'reference' in C++17; 'static int' before

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"
#include "test_macros.h"
#include "test_allocator.h"
#include "test_allocator.h"
#include "asan_testing.h"

class A
{
    int i_;
    double d_;

    A(const A&);
    A& operator=(const A&);
public:
    A(int i, double d)
        : i_(i), d_(d) {}

    A(A&& a)
        : i_(a.i_),
          d_(a.d_)
    {
        a.i_ = 0;
        a.d_ = 0;
    }

    A& operator=(A&& a)
    {
        i_ = a.i_;
        d_ = a.d_;
        a.i_ = 0;
        a.d_ = 0;
        return *this;
    }

    int geti() const {return i_;}
    double getd() const {return d_;}
};

int tc_libcxx_containers_vector_modifiers_emplace_back(void)
{
    {
        std::vector<A> c;
#if TEST_STD_VER > 14
        A& r1 = c.emplace_back(2, 3.5);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(&r1 == &c.back());
        TC_ASSERT_EXPR(c.front().geti() == 2);
        TC_ASSERT_EXPR(c.front().getd() == 3.5);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        A& r2 = c.emplace_back(3, 4.5);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(&r2 == &c.back());
#else
        c.emplace_back(2, 3.5);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(c.front().geti() == 2);
        TC_ASSERT_EXPR(c.front().getd() == 3.5);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        c.emplace_back(3, 4.5);
        TC_ASSERT_EXPR(c.size() == 2);
#endif
        TC_ASSERT_EXPR(c.front().geti() == 2);
        TC_ASSERT_EXPR(c.front().getd() == 3.5);
        TC_ASSERT_EXPR(c.back().geti() == 3);
        TC_ASSERT_EXPR(c.back().getd() == 4.5);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
    }
    {
        std::vector<A, limited_allocator<A, 4> > c;
#if TEST_STD_VER > 14
        A& r1 = c.emplace_back(2, 3.5);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(&r1 == &c.back());
        TC_ASSERT_EXPR(c.front().geti() == 2);
        TC_ASSERT_EXPR(c.front().getd() == 3.5);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        A& r2 = c.emplace_back(3, 4.5);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(&r2 == &c.back());
#else
        c.emplace_back(2, 3.5);
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(c.front().geti() == 2);
        TC_ASSERT_EXPR(c.front().getd() == 3.5);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        c.emplace_back(3, 4.5);
        TC_ASSERT_EXPR(c.size() == 2);
#endif
        TC_ASSERT_EXPR(c.front().geti() == 2);
        TC_ASSERT_EXPR(c.front().getd() == 3.5);
        TC_ASSERT_EXPR(c.back().geti() == 3);
        TC_ASSERT_EXPR(c.back().getd() == 4.5);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
    }
    {
        std::vector<Tag_X, TaggingAllocator<Tag_X>> c;
        c.emplace_back();
        TC_ASSERT_EXPR(c.size() == 1);
        c.emplace_back(1, 2, 3);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
