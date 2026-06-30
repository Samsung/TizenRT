//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <tuple>

// template <class... Types> class tuple;

// template <class U1, class U2> tuple(const pair<U1, U2>& u);

// UNSUPPORTED: c++03

#include <tuple>
#include <utility>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_const_pair(void) {
    {
        typedef std::pair<long, char> T0;
        typedef std::tuple<long long, short> T1;
        T0 t0(2, 'a');
        T1 t1 = t0;
        TC_ASSERT_EXPR(std::get<0>(t1) == 2);
        TC_ASSERT_EXPR(std::get<1>(t1) == short('a'));
    }
#if TEST_STD_VER > 11
    {
        typedef std::pair<long, char> P0;
        typedef std::tuple<long long, short> T1;
        constexpr P0 p0(2, 'a');
        constexpr T1 t1 = p0;
        static_assert(std::get<0>(t1) == std::get<0>(p0), "");
        static_assert(std::get<1>(t1) == std::get<1>(p0), "");
        static_assert(std::get<0>(t1) == 2, "");
        static_assert(std::get<1>(t1) == short('a'), "");
    }
#endif

  return 0;
}
