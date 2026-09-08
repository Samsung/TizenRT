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

// UNSUPPORTED: c++98, c++03

// <vector>

// vector(initializer_list<value_type> il);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include "asan_testing.h"

int tc_libcxx_containers_vector_cons_initializer_list(void)
{
    {
    std::vector<int> d = {3, 4, 5, 6};
    TC_ASSERT_EXPR(d.size() == 4);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(d));
    TC_ASSERT_EXPR(d[0] == 3);
    TC_ASSERT_EXPR(d[1] == 4);
    TC_ASSERT_EXPR(d[2] == 5);
    TC_ASSERT_EXPR(d[3] == 6);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
