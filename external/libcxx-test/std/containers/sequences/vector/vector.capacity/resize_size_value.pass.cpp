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

// static int resize(size_type sz, const value_type& x);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include "test_allocator.h"
#include "asan_testing.h"

int tc_libcxx_containers_vector_capacity_resize_size_value(void)
{
    {
        std::vector<int> v(100);
        v.resize(50, 1);
        TC_ASSERT_EXPR(v.size() == 50);
        TC_ASSERT_EXPR(v.capacity() == 100);
        TC_ASSERT_EXPR(v == std::vector<int>(50));
        v.resize(200, 1);
        TC_ASSERT_EXPR(v.size() == 200);
        TC_ASSERT_EXPR(v.capacity() >= 200);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        for (unsigned i = 0; i < 50; ++i)
            TC_ASSERT_EXPR(v[i] == 0);
        for (unsigned i = 50; i < 200; ++i)
            TC_ASSERT_EXPR(v[i] == 1);
    }
    {
        // Add 1 for implementations that dynamically allocate a container proxy.
        std::vector<int, limited_allocator<int, 300 + 1> > v(100);
        v.resize(50, 1);
        TC_ASSERT_EXPR(v.size() == 50);
        TC_ASSERT_EXPR(v.capacity() == 100);
        v.resize(200, 1);
        TC_ASSERT_EXPR(v.size() == 200);
        TC_ASSERT_EXPR(v.capacity() >= 200);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
