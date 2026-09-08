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

// static int push_front(const value_type& x);

#include <list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


int tc_libcxx_containers_list_modifiers_push_front(void)
{
    {
    std::list<int> c;
    for (int i = 0; i < 5; ++i)
        c.push_front(i);
    int a[] = {4, 3, 2, 1, 0};
    TC_ASSERT_EXPR(c == std::list<int>(a, a+5));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
