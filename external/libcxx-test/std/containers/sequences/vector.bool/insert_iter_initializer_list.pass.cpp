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


int tc_libcxx_containers_vector_bool_insert_iter_initializer_list(void)
{
    {
    std::vector<bool> d(10, true);
    std::vector<bool>::iterator i = d.insert(d.cbegin() + 2, {false, true, true, false});
    TC_ASSERT_EXPR(d.size() == 14);
    TC_ASSERT_EXPR(i == d.begin() + 2);
    TC_ASSERT_EXPR(d[0] == true);
    TC_ASSERT_EXPR(d[1] == true);
    TC_ASSERT_EXPR(d[2] == false);
    TC_ASSERT_EXPR(d[3] == true);
    TC_ASSERT_EXPR(d[4] == true);
    TC_ASSERT_EXPR(d[5] == false);
    TC_ASSERT_EXPR(d[6] == true);
    TC_ASSERT_EXPR(d[7] == true);
    TC_ASSERT_EXPR(d[8] == true);
    TC_ASSERT_EXPR(d[9] == true);
    TC_ASSERT_EXPR(d[10] == true);
    TC_ASSERT_EXPR(d[11] == true);
    TC_ASSERT_EXPR(d[12] == true);
    TC_ASSERT_EXPR(d[13] == true);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
