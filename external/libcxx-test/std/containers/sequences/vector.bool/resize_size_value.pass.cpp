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

// static int resize(size_type sz, const value_type& x);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


int tc_libcxx_containers_vector_bool_resize_size_value(void)
{
    {
        std::vector<bool> v(100);
        v.resize(50, 1);
        TC_ASSERT_EXPR(v.size() == 50);
        TC_ASSERT_EXPR(v.capacity() >= 100);
        TC_ASSERT_EXPR(v == std::vector<bool>(50));
        v.resize(200, 1);
        TC_ASSERT_EXPR(v.size() == 200);
        TC_ASSERT_EXPR(v.capacity() >= 200);
        for (unsigned i = 0; i < 50; ++i)
            TC_ASSERT_EXPR(v[i] == 0);
        for (unsigned i = 50; i < 200; ++i)
            TC_ASSERT_EXPR(v[i] == 1);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
