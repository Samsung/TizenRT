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

// <utility>

// template <class T1, class T2> struct pair

// struct piecewise_construct_t { };
// constexpr piecewise_construct_t piecewise_construct = piecewise_construct_t();

#include <utility>
#include <tuple>
#include <cassert>
#include "libcxx_tc_common.h"

class A
{
    int i_;
    char c_;
public:
    A(int i, char c) : i_(i), c_(c) {}
    int get_i() const {return i_;}
    char get_c() const {return c_;}
};

class B
{
    double d_;
    unsigned u1_;
    unsigned u2_;
public:
    B(double d, unsigned u1, unsigned u2) : d_(d), u1_(u1), u2_(u2) {}
    double get_d() const {return d_;}
    unsigned get_u1() const {return u1_;}
    unsigned get_u2() const {return u2_;}
};

int tc_libcxx_utilities_pair_piecewise_piecewise_construct(void)
{
    std::pair<A, B> p(std::piecewise_construct,
                      std::make_tuple(4, 'a'),
                      std::make_tuple(3.5, 6u, 2u));
    TC_ASSERT_EXPR(p.first.get_i() == 4);
    TC_ASSERT_EXPR(p.first.get_c() == 'a');
    TC_ASSERT_EXPR(p.second.get_d() == 3.5);
    TC_ASSERT_EXPR(p.second.get_u1() == 6u);
    TC_ASSERT_EXPR(p.second.get_u2() == 2u);
    TC_SUCCESS_RESULT();
    return 0;
}
