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

// static int pop_front();

#include <list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


int tc_libcxx_containers_list_modifiers_pop_front(void)
{
    {
    int a[] = {1, 2, 3};
    std::list<int> c(a, a+3);
    c.pop_front();
    TC_ASSERT_EXPR(c == std::list<int>(a+1, a+3));
    c.pop_front();
    TC_ASSERT_EXPR(c == std::list<int>(a+2, a+3));
    c.pop_front();
    TC_ASSERT_EXPR(c.empty());
    }
    TC_SUCCESS_RESULT();
    return 0;
}
