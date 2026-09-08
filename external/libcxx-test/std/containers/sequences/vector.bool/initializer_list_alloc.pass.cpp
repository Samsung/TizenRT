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

// vector(initializer_list<value_type> il, const Allocator& a = allocator_type());

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "test_allocator.h"

int tc_libcxx_containers_vector_bool_initializer_list_alloc(void)
{
    {
    std::vector<bool, test_allocator<bool>> d({true, false, false, true}, test_allocator<bool>(3));
    TC_ASSERT_EXPR(d.get_allocator() == test_allocator<bool>(3));
    TC_ASSERT_EXPR(d.size() == 4);
    TC_ASSERT_EXPR(d[0] == true);
    TC_ASSERT_EXPR(d[1] == false);
    TC_ASSERT_EXPR(d[2] == false);
    TC_ASSERT_EXPR(d[3] == true);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
