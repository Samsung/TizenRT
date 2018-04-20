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

// template<class U, class V> pair& operator=(pair<U, V>&& p);

#include <utility>
#include <memory>
#include <cassert>
#include "libcxx_tc_common.h"
#include <archetypes.hpp>

struct Base
{
    virtual ~Base() {}
};

struct Derived
    : public Base
{
};

int tc_libcxx_utilities_pairs_pair_assign_rv_pair_U_V(void)
{
    {
        typedef std::pair<std::unique_ptr<Derived>, short> P1;
        typedef std::pair<std::unique_ptr<Base>, long> P2;
        P1 p1(std::unique_ptr<Derived>(), static_cast<short>(4));
        P2 p2;
        p2 = std::move(p1);
        TC_ASSERT_EXPR(p2.first == nullptr);
        TC_ASSERT_EXPR(p2.second == 4);
    }
    {
       using C = TestTypes::TestType;
       using P = std::pair<int, C>;
       using T = std::pair<long, C>;
       T t(42, -42);
       P p(101, 101);
       C::reset_constructors();
       p = std::move(t);
       TC_ASSERT_EXPR(C::constructed == 0);
       TC_ASSERT_EXPR(C::assigned == 1);
       TC_ASSERT_EXPR(C::copy_assigned == 0);
       TC_ASSERT_EXPR(C::move_assigned == 1);
       TC_ASSERT_EXPR(p.first == 42);
       TC_ASSERT_EXPR(p.second.value == -42);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
