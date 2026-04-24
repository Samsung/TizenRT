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
