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
// UNSUPPORTED: c++03

// <utility>

// template <class T1, class T2> struct pair

// struct piecewise_construct_t { explicit piecewise_construct_t() = default; };
// constexpr piecewise_construct_t piecewise_construct = piecewise_construct_t();

#include <utility>
#include <tuple>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

class A
{
    int i_;
    char c_;
public:
    constexpr A(int i, char c) : i_(i), c_(c) {}
    constexpr int get_i() const {return i_;}
    constexpr char get_c() const {return c_;}
};

class B
{
    double d_;
    unsigned u1_;
    unsigned u2_;
public:
    constexpr explicit B(double d, unsigned u1, unsigned u2) : d_(d), u1_(u1), u2_(u2) {}
    constexpr double get_d() const {return d_;}
    constexpr unsigned get_u1() const {return u1_;}
    constexpr unsigned get_u2() const {return u2_;}
};

TEST_CONSTEXPR_CXX20
bool test()
{
    std::pair<A, B> p(std::piecewise_construct,
                      std::make_tuple(4, 'a'),
                      std::make_tuple(3.5, 6u, 2u));
    TC_ASSERT_EXPR(p.first.get_i() == 4);
    TC_ASSERT_EXPR(p.first.get_c() == 'a');
    TC_ASSERT_EXPR(p.second.get_d() == 3.5);
    TC_ASSERT_EXPR(p.second.get_u1() == 6u);
    TC_ASSERT_EXPR(p.second.get_u2() == 2u);

    return true;
}

int tc_utilities_utility_pairs_pair_piecewise_piecewise_construct(void) {
    test();
#if TEST_STD_VER >= 20
    static_assert(test());
#endif

    return 0;
}
