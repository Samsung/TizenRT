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

// <vector>

// static int swap(vector& x);

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"

#include "asan_testing.h"

int tc_libcxx_containers_vector_capacity_swap(void)
{
    {
        std::vector<int> v1(100);
        std::vector<int> v2(200);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v1));
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v2));
        v1.swap(v2);
        TC_ASSERT_EXPR(v1.size() == 200);
        TC_ASSERT_EXPR(v1.capacity() == 200);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v1));
        TC_ASSERT_EXPR(v2.size() == 100);
        TC_ASSERT_EXPR(v2.capacity() == 100);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v2));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
