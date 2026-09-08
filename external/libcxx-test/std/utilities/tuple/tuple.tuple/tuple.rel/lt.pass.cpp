//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <tuple>

// template <class... Types> class tuple;

// template<class... TTypes, class... UTypes>
//   bool
//   operator<(const tuple<TTypes...>& t, const tuple<UTypes...>& u);
//
// template<class... TTypes, class... UTypes>
//   bool
//   operator>(const tuple<TTypes...>& t, const tuple<UTypes...>& u);
//
// template<class... TTypes, class... UTypes>
//   bool
//   operator<=(const tuple<TTypes...>& t, const tuple<UTypes...>& u);
//
// template<class... TTypes, class... UTypes>
//   bool
//   operator>=(const tuple<TTypes...>& t, const tuple<UTypes...>& u);

// UNSUPPORTED: c++03

#include <tuple>
#include <string>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_tuple_tuple_tuple_tuple_rel_lt(void) {
    {
        typedef std::tuple<> T1;
        typedef std::tuple<> T2;
        const T1 t1;
        const T2 t2;
        TC_ASSERT_EXPR(!(t1 <  t2));
        TC_ASSERT_EXPR( (t1 <= t2));
        TC_ASSERT_EXPR(!(t1 >  t2));
        TC_ASSERT_EXPR( (t1 >= t2));
    }
    {
        typedef std::tuple<long> T1;
        typedef std::tuple<double> T2;
        const T1 t1(1);
        const T2 t2(1);
        TC_ASSERT_EXPR(!(t1 <  t2));
        TC_ASSERT_EXPR( (t1 <= t2));
        TC_ASSERT_EXPR(!(t1 >  t2));
        TC_ASSERT_EXPR( (t1 >= t2));
    }
    {
        typedef std::tuple<long> T1;
        typedef std::tuple<double> T2;
        const T1 t1(1);
        const T2 t2(0.9);
        TC_ASSERT_EXPR(!(t1 <  t2));
        TC_ASSERT_EXPR(!(t1 <= t2));
        TC_ASSERT_EXPR( (t1 >  t2));
        TC_ASSERT_EXPR( (t1 >= t2));
    }
    {
        typedef std::tuple<long> T1;
        typedef std::tuple<double> T2;
        const T1 t1(1);
        const T2 t2(1.1);
        TC_ASSERT_EXPR( (t1 <  t2));
        TC_ASSERT_EXPR( (t1 <= t2));
        TC_ASSERT_EXPR(!(t1 >  t2));
        TC_ASSERT_EXPR(!(t1 >= t2));
    }
    {
        typedef std::tuple<long, int> T1;
        typedef std::tuple<double, long> T2;
        const T1 t1(1, 2);
        const T2 t2(1, 2);
        TC_ASSERT_EXPR(!(t1 <  t2));
        TC_ASSERT_EXPR( (t1 <= t2));
        TC_ASSERT_EXPR(!(t1 >  t2));
        TC_ASSERT_EXPR( (t1 >= t2));
    }
    {
        typedef std::tuple<long, int> T1;
        typedef std::tuple<double, long> T2;
        const T1 t1(1, 2);
        const T2 t2(0.9, 2);
        TC_ASSERT_EXPR(!(t1 <  t2));
        TC_ASSERT_EXPR(!(t1 <= t2));
        TC_ASSERT_EXPR( (t1 >  t2));
        TC_ASSERT_EXPR( (t1 >= t2));
    }
    {
        typedef std::tuple<long, int> T1;
        typedef std::tuple<double, long> T2;
        const T1 t1(1, 2);
        const T2 t2(1.1, 2);
        TC_ASSERT_EXPR( (t1 <  t2));
        TC_ASSERT_EXPR( (t1 <= t2));
        TC_ASSERT_EXPR(!(t1 >  t2));
        TC_ASSERT_EXPR(!(t1 >= t2));
    }
    {
        typedef std::tuple<long, int> T1;
        typedef std::tuple<double, long> T2;
        const T1 t1(1, 2);
        const T2 t2(1, 1);
        TC_ASSERT_EXPR(!(t1 <  t2));
        TC_ASSERT_EXPR(!(t1 <= t2));
        TC_ASSERT_EXPR( (t1 >  t2));
        TC_ASSERT_EXPR( (t1 >= t2));
    }
    {
        typedef std::tuple<long, int> T1;
        typedef std::tuple<double, long> T2;
        const T1 t1(1, 2);
        const T2 t2(1, 3);
        TC_ASSERT_EXPR( (t1 <  t2));
        TC_ASSERT_EXPR( (t1 <= t2));
        TC_ASSERT_EXPR(!(t1 >  t2));
        TC_ASSERT_EXPR(!(t1 >= t2));
    }
    {
        typedef std::tuple<long, int, double> T1;
        typedef std::tuple<double, long, int> T2;
        const T1 t1(1, 2, 3);
        const T2 t2(1, 2, 3);
        TC_ASSERT_EXPR(!(t1 <  t2));
        TC_ASSERT_EXPR( (t1 <= t2));
        TC_ASSERT_EXPR(!(t1 >  t2));
        TC_ASSERT_EXPR( (t1 >= t2));
    }
    {
        typedef std::tuple<long, int, double> T1;
        typedef std::tuple<double, long, int> T2;
        const T1 t1(1, 2, 3);
        const T2 t2(0.9, 2, 3);
        TC_ASSERT_EXPR(!(t1 <  t2));
        TC_ASSERT_EXPR(!(t1 <= t2));
        TC_ASSERT_EXPR( (t1 >  t2));
        TC_ASSERT_EXPR( (t1 >= t2));
    }
    {
        typedef std::tuple<long, int, double> T1;
        typedef std::tuple<double, long, int> T2;
        const T1 t1(1, 2, 3);
        const T2 t2(1.1, 2, 3);
        TC_ASSERT_EXPR( (t1 <  t2));
        TC_ASSERT_EXPR( (t1 <= t2));
        TC_ASSERT_EXPR(!(t1 >  t2));
        TC_ASSERT_EXPR(!(t1 >= t2));
    }
    {
        typedef std::tuple<long, int, double> T1;
        typedef std::tuple<double, long, int> T2;
        const T1 t1(1, 2, 3);
        const T2 t2(1, 1, 3);
        TC_ASSERT_EXPR(!(t1 <  t2));
        TC_ASSERT_EXPR(!(t1 <= t2));
        TC_ASSERT_EXPR( (t1 >  t2));
        TC_ASSERT_EXPR( (t1 >= t2));
    }
    {
        typedef std::tuple<long, int, double> T1;
        typedef std::tuple<double, long, int> T2;
        const T1 t1(1, 2, 3);
        const T2 t2(1, 3, 3);
        TC_ASSERT_EXPR( (t1 <  t2));
        TC_ASSERT_EXPR( (t1 <= t2));
        TC_ASSERT_EXPR(!(t1 >  t2));
        TC_ASSERT_EXPR(!(t1 >= t2));
    }
    {
        typedef std::tuple<long, int, double> T1;
        typedef std::tuple<double, long, int> T2;
        const T1 t1(1, 2, 3);
        const T2 t2(1, 2, 2);
        TC_ASSERT_EXPR(!(t1 <  t2));
        TC_ASSERT_EXPR(!(t1 <= t2));
        TC_ASSERT_EXPR( (t1 >  t2));
        TC_ASSERT_EXPR( (t1 >= t2));
    }
    {
        typedef std::tuple<long, int, double> T1;
        typedef std::tuple<double, long, int> T2;
        const T1 t1(1, 2, 3);
        const T2 t2(1, 2, 4);
        TC_ASSERT_EXPR( (t1 <  t2));
        TC_ASSERT_EXPR( (t1 <= t2));
        TC_ASSERT_EXPR(!(t1 >  t2));
        TC_ASSERT_EXPR(!(t1 >= t2));
    }
#if TEST_STD_VER > 11
    {
        typedef std::tuple<long, int, double> T1;
        typedef std::tuple<double, long, int> T2;
        constexpr T1 t1(1, 2, 3);
        constexpr T2 t2(1, 2, 4);
        static_assert( (t1 <  t2), "");
        static_assert( (t1 <= t2), "");
        static_assert(!(t1 >  t2), "");
        static_assert(!(t1 >= t2), "");
    }
#endif

  return 0;
}
