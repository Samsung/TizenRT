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

// <utility>

// template <class T1, class T2> struct pair

// template<size_t I, class T1, class T2>
//     typename tuple_element<I, std::pair<T1, T2> >::type&
//     get(pair<T1, T2>&);

#include <utility>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

#if TEST_STD_VER > 11
struct S {
   std::pair<int, int> a;
   int k;
   constexpr S() : a{1,2}, k(std::get<0>(a)) {}
   };

constexpr std::pair<int, int> getP () { return { 3, 4 }; }
#endif

int tc_libcxx_utilities_pair_astuple_get_non_const(void)
{
    {
        typedef std::pair<int, short> P;
        P p(3, static_cast<short>(4));
        TC_ASSERT_EXPR(std::get<0>(p) == 3);
        TC_ASSERT_EXPR(std::get<1>(p) == 4);
        std::get<0>(p) = 5;
        std::get<1>(p) = 6;
        TC_ASSERT_EXPR(std::get<0>(p) == 5);
        TC_ASSERT_EXPR(std::get<1>(p) == 6);
    }

#if TEST_STD_VER > 11
    {
        static_assert(S().k == 1, "");
        static_assert(std::get<1>(getP()) == 4, "");
    }
#endif

    TC_SUCCESS_RESULT();
    return 0;
}
