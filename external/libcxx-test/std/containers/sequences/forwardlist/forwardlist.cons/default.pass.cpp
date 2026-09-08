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

// <forward_list>

// forward_list();

#include <forward_list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_containers_forwardlist_cons_default(void)
{
    {
        typedef int T;
        typedef std::forward_list<T> C;
        C c;
        TC_ASSERT_EXPR(c.empty());
    }
#if TEST_STD_VER >= 11
    {
        typedef int T;
        typedef std::forward_list<T> C;
        C c = {};
        TC_ASSERT_EXPR(c.empty());
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
