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

// static int reserve(size_type n);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include "test_allocator.h"
#include "asan_testing.h"

int tc_libcxx_containers_vector_capacity_reserve(void)
{
    {
        std::vector<int> v;
        v.reserve(10);
        TC_ASSERT_EXPR(v.capacity() >= 10);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        std::vector<int> v(100);
        TC_ASSERT_EXPR(v.capacity() == 100);
        v.reserve(50);
        TC_ASSERT_EXPR(v.size() == 100);
        TC_ASSERT_EXPR(v.capacity() == 100);
        v.reserve(150);
        TC_ASSERT_EXPR(v.size() == 100);
        TC_ASSERT_EXPR(v.capacity() == 150);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        // Add 1 for implementations that dynamically allocate a container proxy.
        std::vector<int, limited_allocator<int, 250 + 1> > v(100);
        TC_ASSERT_EXPR(v.capacity() == 100);
        v.reserve(50);
        TC_ASSERT_EXPR(v.size() == 100);
        TC_ASSERT_EXPR(v.capacity() == 100);
        v.reserve(150);
        TC_ASSERT_EXPR(v.size() == 100);
        TC_ASSERT_EXPR(v.capacity() == 150);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
