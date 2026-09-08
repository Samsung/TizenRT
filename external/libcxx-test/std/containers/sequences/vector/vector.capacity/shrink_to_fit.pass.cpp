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

// static int shrink_to_fit();

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include "test_allocator.h"
#include "asan_testing.h"

int tc_libcxx_containers_vector_capacity_shrink_to_fit(void)
{
    {
        std::vector<int> v(100);
        v.push_back(1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        v.shrink_to_fit();
        TC_ASSERT_EXPR(v.capacity() == 101);
        TC_ASSERT_EXPR(v.size() == 101);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
    {
        std::vector<int, limited_allocator<int, 401> > v(100);
        v.push_back(1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        v.shrink_to_fit();
        TC_ASSERT_EXPR(v.capacity() == 101);
        TC_ASSERT_EXPR(v.size() == 101);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
#ifndef _LIBCPP_NO_EXCEPTIONS
    {
        std::vector<int, limited_allocator<int, 400> > v(100);
        v.push_back(1);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
        v.shrink_to_fit();
        LIBCPP_ASSERT(v.capacity() == 200); // assumes libc++'s 2x growth factor
        TC_ASSERT_EXPR(v.size() == 101);
        TC_ASSERT_EXPR(is_contiguous_container_asan_correct(v));
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
