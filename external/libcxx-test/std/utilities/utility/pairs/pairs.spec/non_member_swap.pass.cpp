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

// template <class T1, class T2> void swap(pair<T1, T2>& x, pair<T1, T2>& y);

#include <utility>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_libcxx_utilities_pairs_spec_non_member_swap(void)
{
    {
        typedef std::pair<int, short> P1;
        P1 p1(3, static_cast<short>(4));
        P1 p2(5, static_cast<short>(6));
        swap(p1, p2);
        TC_ASSERT_EXPR(p1.first == 5);
        TC_ASSERT_EXPR(p1.second == 6);
        TC_ASSERT_EXPR(p2.first == 3);
        TC_ASSERT_EXPR(p2.second == 4);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
