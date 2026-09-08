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

// static int shrink_to_fit();

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


int tc_libcxx_containers_vector_bool_shrink_to_fit(void)
{
    {
        std::vector<bool> v(100);
        v.push_back(1);
        v.shrink_to_fit();
        TC_ASSERT_EXPR(v.capacity() >= 101);
        TC_ASSERT_EXPR(v.size() >= 101);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
