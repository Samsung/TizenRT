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

// <unordered_map>

// template <class Key, class T, class Hash = hash<Key>, class Pred = equal_to<Key>,
//           class Alloc = allocator<pair<const Key, T>>>
// class unordered_map

// float load_factor() const

#include <unordered_map>
#include <string>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cmath>
#include <cfloat>

#include "test_macros.h"

int tc_libcxx_containers_unord_map_load_factor(void)
{
    {
        typedef std::unordered_map<int, std::string> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(10, "ten"),
            P(20, "twenty"),
            P(30, "thirty"),
            P(40, "forty"),
            P(50, "fifty"),
            P(60, "sixty"),
            P(70, "seventy"),
            P(80, "eighty"),
        };
        const C c(std::begin(a), std::end(a));
        TC_ASSERT_EXPR(std::fabs(c.load_factor() - (float)c.size()/c.bucket_count()) < FLT_EPSILON);
    }
    {
        typedef std::unordered_map<int, std::string> C;
        const C c;
        TC_ASSERT_EXPR(c.load_factor() == 0);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
