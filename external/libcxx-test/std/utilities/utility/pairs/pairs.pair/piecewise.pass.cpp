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

// template <class... Args1, class... Args2>
//     pair(piecewise_construct_t, tuple<Args1...> first_args,
//                                 tuple<Args2...> second_args);

#include <utility>
#include <tuple>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_libcxx_utilities_pairs_pair_piecewise(void)
{
    {
        typedef std::pair<int, int*> P1;
        typedef std::pair<int*, int> P2;
        typedef std::pair<P1, P2> P3;
        P3 p3(std::piecewise_construct, std::tuple<int, int*>(3, nullptr),
                                        std::tuple<int*, int>(nullptr, 4));
        TC_ASSERT_EXPR(p3.first == P1(3, nullptr));
        TC_ASSERT_EXPR(p3.second == P2(nullptr, 4));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
