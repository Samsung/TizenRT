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

// <list>

// list(list&& c);

#include <list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include "MoveOnly.h"
#include "test_allocator.h"

int tc_libcxx_containers_list_cons_move(void)
{
    {
        std::list<MoveOnly, test_allocator<MoveOnly> > l(test_allocator<MoveOnly>(5));
        std::list<MoveOnly, test_allocator<MoveOnly> > lo(test_allocator<MoveOnly>(5));
        for (int i = 1; i <= 3; ++i)
        {
            l.push_back(i);
            lo.push_back(i);
        }
        std::list<MoveOnly, test_allocator<MoveOnly> > l2 = std::move(l);
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == lo.get_allocator());
    }
    {
        std::list<MoveOnly, other_allocator<MoveOnly> > l(other_allocator<MoveOnly>(5));
        std::list<MoveOnly, other_allocator<MoveOnly> > lo(other_allocator<MoveOnly>(5));
        for (int i = 1; i <= 3; ++i)
        {
            l.push_back(i);
            lo.push_back(i);
        }
        std::list<MoveOnly, other_allocator<MoveOnly> > l2 = std::move(l);
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == lo.get_allocator());
    }
    TC_SUCCESS_RESULT();
    return 0;
}
