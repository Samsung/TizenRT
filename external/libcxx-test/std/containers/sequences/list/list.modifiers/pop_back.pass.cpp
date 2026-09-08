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

// static int pop_back();

#include <list>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

int tc_libcxx_containers_list_modifiers_pop_back(void)
{
    {
    int a[] = {1, 2, 3};
    std::list<int> c(a, a+3);
    c.pop_back();
    TC_ASSERT_EXPR(c == std::list<int>(a, a+2));
    c.pop_back();
    TC_ASSERT_EXPR(c == std::list<int>(a, a+1));
    c.pop_back();
    TC_ASSERT_EXPR(c.empty());
    }
    TC_SUCCESS_RESULT();
    return 0;
}
