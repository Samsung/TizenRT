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

// <list>

// static int resize(size_type sz, const value_type& x);

#include <list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include "DefaultOnly.h"

int tc_libcxx_containers_list_capacity_resize_size_value(void)
{
    {
        std::list<double> l(5, 2);
        l.resize(2, 3.5);
        TC_ASSERT_EXPR(l.size() == 2);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 2);
        TC_ASSERT_EXPR(l == std::list<double>(2, 2));
    }
    {
        std::list<double> l(5, 2);
        l.resize(10, 3.5);
        TC_ASSERT_EXPR(l.size() == 10);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 10);
        TC_ASSERT_EXPR(l.front() == 2);
        TC_ASSERT_EXPR(l.back() == 3.5);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
