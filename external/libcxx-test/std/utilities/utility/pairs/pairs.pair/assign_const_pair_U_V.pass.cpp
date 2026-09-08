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

// <utility>

// template <class T1, class T2> struct pair

// template<class U, class V> pair& operator=(const pair<U, V>& p);

#include <utility>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#if TEST_STD_VER >= 11
#include "archetypes.hpp"
#endif

int tc_libcxx_utilities_pairs_pair_assign_const_pair_U_V(void)
{
    {
        typedef std::pair<int, short> P1;
        typedef std::pair<double, long> P2;
        P1 p1(3, static_cast<short>(4));
        P2 p2;
        p2 = p1;
        TC_ASSERT_EXPR(p2.first == 3);
        TC_ASSERT_EXPR(p2.second == 4);
    }
#if TEST_STD_VER >= 11
    {
       using C = TestTypes::TestType;
       using P = std::pair<int, C>;
       using T = std::pair<long, C>;
       const T t(42, -42);
       P p(101, 101);
       C::reset_constructors();
       p = t;
       TC_ASSERT_EXPR(C::constructed == 0);
       TC_ASSERT_EXPR(C::assigned == 1);
       TC_ASSERT_EXPR(C::copy_assigned == 1);
       TC_ASSERT_EXPR(C::move_assigned == 0);
       TC_ASSERT_EXPR(p.first == 42);
       TC_ASSERT_EXPR(p.second.value == -42);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
