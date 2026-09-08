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

// list(const list& c);

#include <list>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "DefaultOnly.h"
#include "test_allocator.h"

int tc_libcxx_containers_list_cons_copy(void)
{
    {
        std::list<int> l(3, 2);
        std::list<int> l2 = l;
        TC_ASSERT_EXPR(l2 == l);
    }
    {
        std::list<int, test_allocator<int> > l(3, 2, test_allocator<int>(5));
        std::list<int, test_allocator<int> > l2 = l;
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == l.get_allocator());
    }
#if TEST_STD_VER >= 11
    {
        std::list<int, other_allocator<int> > l(3, 2, other_allocator<int>(5));
        std::list<int, other_allocator<int> > l2 = l;
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == other_allocator<int>(-2));
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
