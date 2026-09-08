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
// vector<bool>

// static int reserve(size_type n);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


int tc_libcxx_containers_vector_bool_reserve(void)
{
    {
        std::vector<bool> v;
        v.reserve(10);
        TC_ASSERT_EXPR(v.capacity() >= 10);
    }
    {
        std::vector<bool> v(100);
        TC_ASSERT_EXPR(v.capacity() >= 100);
        v.reserve(50);
        TC_ASSERT_EXPR(v.size() == 100);
        TC_ASSERT_EXPR(v.capacity() >= 100);
        v.reserve(150);
        TC_ASSERT_EXPR(v.size() == 100);
        TC_ASSERT_EXPR(v.capacity() >= 150);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
