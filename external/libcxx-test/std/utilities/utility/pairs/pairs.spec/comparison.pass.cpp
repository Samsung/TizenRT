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

// template <class T1, class T2> bool operator==(const pair<T1,T2>&, const pair<T1,T2>&);
// template <class T1, class T2> bool operator!=(const pair<T1,T2>&, const pair<T1,T2>&);
// template <class T1, class T2> bool operator< (const pair<T1,T2>&, const pair<T1,T2>&);
// template <class T1, class T2> bool operator> (const pair<T1,T2>&, const pair<T1,T2>&);
// template <class T1, class T2> bool operator>=(const pair<T1,T2>&, const pair<T1,T2>&);
// template <class T1, class T2> bool operator<=(const pair<T1,T2>&, const pair<T1,T2>&);

#include <utility>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

int tc_libcxx_utilities_pairs_spec_comparison(void)
{
    {
        typedef std::pair<int, short> P;
        P p1(3, static_cast<short>(4));
        P p2(3, static_cast<short>(4));
        TC_ASSERT_EXPR( (p1 == p2));
        TC_ASSERT_EXPR(!(p1 != p2));
        TC_ASSERT_EXPR(!(p1 <  p2));
        TC_ASSERT_EXPR( (p1 <= p2));
        TC_ASSERT_EXPR(!(p1 >  p2));
        TC_ASSERT_EXPR( (p1 >= p2));
    }
    {
        typedef std::pair<int, short> P;
        P p1(2, static_cast<short>(4));
        P p2(3, static_cast<short>(4));
        TC_ASSERT_EXPR(!(p1 == p2));
        TC_ASSERT_EXPR( (p1 != p2));
        TC_ASSERT_EXPR( (p1 <  p2));
        TC_ASSERT_EXPR( (p1 <= p2));
        TC_ASSERT_EXPR(!(p1 >  p2));
        TC_ASSERT_EXPR(!(p1 >= p2));
    }
    {
        typedef std::pair<int, short> P;
        P p1(3, static_cast<short>(2));
        P p2(3, static_cast<short>(4));
        TC_ASSERT_EXPR(!(p1 == p2));
        TC_ASSERT_EXPR( (p1 != p2));
        TC_ASSERT_EXPR( (p1 <  p2));
        TC_ASSERT_EXPR( (p1 <= p2));
        TC_ASSERT_EXPR(!(p1 >  p2));
        TC_ASSERT_EXPR(!(p1 >= p2));
    }
    {
        typedef std::pair<int, short> P;
        P p1(3, static_cast<short>(4));
        P p2(2, static_cast<short>(4));
        TC_ASSERT_EXPR(!(p1 == p2));
        TC_ASSERT_EXPR( (p1 != p2));
        TC_ASSERT_EXPR(!(p1 <  p2));
        TC_ASSERT_EXPR(!(p1 <= p2));
        TC_ASSERT_EXPR( (p1 >  p2));
        TC_ASSERT_EXPR( (p1 >= p2));
    }
    {
        typedef std::pair<int, short> P;
        P p1(3, static_cast<short>(4));
        P p2(3, static_cast<short>(2));
        TC_ASSERT_EXPR(!(p1 == p2));
        TC_ASSERT_EXPR( (p1 != p2));
        TC_ASSERT_EXPR(!(p1 <  p2));
        TC_ASSERT_EXPR(!(p1 <= p2));
        TC_ASSERT_EXPR( (p1 >  p2));
        TC_ASSERT_EXPR( (p1 >= p2));
    }

#if TEST_STD_VER > 11
    {
        typedef std::pair<int, short> P;
        constexpr P p1(3, static_cast<short>(4));
        constexpr P p2(3, static_cast<short>(2));
        static_assert(!(p1 == p2), "");
        static_assert( (p1 != p2), "");
        static_assert(!(p1 <  p2), "");
        static_assert(!(p1 <= p2), "");
        static_assert( (p1 >  p2), "");
        static_assert( (p1 >= p2), "");
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
