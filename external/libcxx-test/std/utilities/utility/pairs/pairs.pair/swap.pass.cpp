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

// void swap(pair& p);

#include <utility>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

struct S {
    int i;
    S() : i(0) {}
    S(int j) : i(j) {}
    S * operator& () { assert(false); return this; }
    S const * operator& () const { assert(false); return this; }
    bool operator==(int x) const { return i == x; }
    };

int tc_libcxx_utilities_pairs_pair_swap(void)
{
    {
        typedef std::pair<int, short> P1;
        P1 p1(3, static_cast<short>(4));
        P1 p2(5, static_cast<short>(6));
        p1.swap(p2);
        TC_ASSERT_EXPR(p1.first == 5);
        TC_ASSERT_EXPR(p1.second == 6);
        TC_ASSERT_EXPR(p2.first == 3);
        TC_ASSERT_EXPR(p2.second == 4);
    }
    {
        typedef std::pair<int, S> P1;
        P1 p1(3, S(4));
        P1 p2(5, S(6));
        p1.swap(p2);
        TC_ASSERT_EXPR(p1.first == 5);
        TC_ASSERT_EXPR(p1.second == 6);
        TC_ASSERT_EXPR(p2.first == 3);
        TC_ASSERT_EXPR(p2.second == 4);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
