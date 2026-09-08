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

// <vector>

// static int swap(vector& x);

#include <vector>
#include <cassert>
#include "test_macros.h"
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
