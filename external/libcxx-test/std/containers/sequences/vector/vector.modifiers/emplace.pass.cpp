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

// template <class... Args> iterator emplace(const_iterator pos, Args&&... args);

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
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

int tc_libcxx_containers_vector_modifiers_emplace(void)
{
    {
        std::vector<A> c;
        std::vector<A>::iterator i = c.emplace(c.cbegin(), 2, 3.5);
        TC_ASSERT_EXPR(i == c.begin());
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(c.front().geti() == 2);
        TC_ASSERT_EXPR(c.front().getd() == 3.5);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        i = c.emplace(c.cend(), 3, 4.5);
        TC_ASSERT_EXPR(i == c.end()-1);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(c.front().geti() == 2);
        TC_ASSERT_EXPR(c.front().getd() == 3.5);
        TC_ASSERT_EXPR(c.back().geti() == 3);
        TC_ASSERT_EXPR(c.back().getd() == 4.5);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        i = c.emplace(c.cbegin()+1, 4, 6.5);
        TC_ASSERT_EXPR(i == c.begin()+1);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.front().geti() == 2);
        TC_ASSERT_EXPR(c.front().getd() == 3.5);
        TC_ASSERT_EXPR(c[1].geti() == 4);
        TC_ASSERT_EXPR(c[1].getd() == 6.5);
        TC_ASSERT_EXPR(c.back().geti() == 3);
        TC_ASSERT_EXPR(c.back().getd() == 4.5);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
    }
    {
        std::vector<A, limited_allocator<A, 7> > c;
        std::vector<A, limited_allocator<A, 7> >::iterator i = c.emplace(c.cbegin(), 2, 3.5);
        TC_ASSERT_EXPR(i == c.begin());
        TC_ASSERT_EXPR(c.size() == 1);
        TC_ASSERT_EXPR(c.front().geti() == 2);
        TC_ASSERT_EXPR(c.front().getd() == 3.5);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        i = c.emplace(c.cend(), 3, 4.5);
        TC_ASSERT_EXPR(i == c.end()-1);
        TC_ASSERT_EXPR(c.size() == 2);
        TC_ASSERT_EXPR(c.front().geti() == 2);
        TC_ASSERT_EXPR(c.front().getd() == 3.5);
        TC_ASSERT_EXPR(c.back().geti() == 3);
        TC_ASSERT_EXPR(c.back().getd() == 4.5);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
        i = c.emplace(c.cbegin()+1, 4, 6.5);
        TC_ASSERT_EXPR(i == c.begin()+1);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.front().geti() == 2);
        TC_ASSERT_EXPR(c.front().getd() == 3.5);
        TC_ASSERT_EXPR(c[1].geti() == 4);
        TC_ASSERT_EXPR(c[1].getd() == 6.5);
        TC_ASSERT_EXPR(c.back().geti() == 3);
        TC_ASSERT_EXPR(c.back().getd() == 4.5);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(c));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
