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
// class unordered_multimap

// size_type bucket_size(size_type n) const

#include <unordered_map>
#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

int tc_libcxx_containers_unord_multimap_bucket_size(void)
{
    {
        typedef std::unordered_multimap<int, std::string> C;
        typedef std::pair<int, std::string> P;
        P a[] =
        {
            P(1, "one"),
            P(2, "two"),
            P(3, "three"),
            P(4, "four"),
            P(1, "four"),
            P(2, "four"),
        };
        const C c(std::begin(a), std::end(a));
        TC_ASSERT_EXPR(c.bucket_count() >= 7);
        LIBCPP_ASSERT(c.bucket_size(0) == 0);
        LIBCPP_ASSERT(c.bucket_size(1) == 2);
        LIBCPP_ASSERT(c.bucket_size(2) == 2);
        LIBCPP_ASSERT(c.bucket_size(3) == 1);
        LIBCPP_ASSERT(c.bucket_size(4) == 1);
        LIBCPP_ASSERT(c.bucket_size(5) == 0);
        LIBCPP_ASSERT(c.bucket_size(6) == 0);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
