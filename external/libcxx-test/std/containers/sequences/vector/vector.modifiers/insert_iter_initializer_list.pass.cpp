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

// iterator insert(const_iterator p, initializer_list<value_type> il);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "asan_testing.h"

int tc_libcxx_containers_vector_modifiers_insert_iter_initializer_list(void)
{
    {
    std::vector<int> d(10, 1);
    std::vector<int>::iterator i = d.insert(d.cbegin() + 2, {3, 4, 5, 6});
    TC_ASSERT_EXPR(d.size() == 14);
    TC_ASSERT_EXPR(is_contiguous_container_asan_correct(d));
    TC_ASSERT_EXPR(i == d.begin() + 2);
    TC_ASSERT_EXPR(d[0] == 1);
    TC_ASSERT_EXPR(d[1] == 1);
    TC_ASSERT_EXPR(d[2] == 3);
    TC_ASSERT_EXPR(d[3] == 4);
    TC_ASSERT_EXPR(d[4] == 5);
    TC_ASSERT_EXPR(d[5] == 6);
    TC_ASSERT_EXPR(d[6] == 1);
    TC_ASSERT_EXPR(d[7] == 1);
    TC_ASSERT_EXPR(d[8] == 1);
    TC_ASSERT_EXPR(d[9] == 1);
    TC_ASSERT_EXPR(d[10] == 1);
    TC_ASSERT_EXPR(d[11] == 1);
    TC_ASSERT_EXPR(d[12] == 1);
    TC_ASSERT_EXPR(d[13] == 1);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
