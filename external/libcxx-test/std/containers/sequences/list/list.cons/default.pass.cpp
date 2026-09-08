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

// explicit list(const Alloc& = Alloc());

#include <list>
#include <cassert>
#include "libcxx_tc_common.h"
#include "DefaultOnly.h"
#include "test_macros.h"

int tc_libcxx_containers_list_cons_default(void)
{
    {
        std::list<int> l;
        TC_ASSERT_EXPR(l.size() == 0);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 0);
    }
    {
        std::list<DefaultOnly> l;
        TC_ASSERT_EXPR(l.size() == 0);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 0);
    }
    {
        std::list<int> l((std::allocator<int>()));
        TC_ASSERT_EXPR(l.size() == 0);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 0);
    }
#if TEST_STD_VER >= 11
    {
        std::list<int> l = {};
        TC_ASSERT_EXPR(l.size() == 0);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 0);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
