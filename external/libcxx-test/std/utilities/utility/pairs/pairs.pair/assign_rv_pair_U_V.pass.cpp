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

// <utility>

// template <class T1, class T2> struct pair

// template<class U, class V> pair& operator=(pair<U, V>&& p);

#include <utility>
#include <memory>
#include <cassert>
#include "test_macros.h"
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
