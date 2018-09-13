/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
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

// list(list&& c, const allocator_type& a);

#include <list>
#include <cassert>
#include "libcxx_tc_common.h"
#include "MoveOnly.h"
#include "test_allocator.h"

int tc_libcxx_containers_list_cons_move_alloc(void)
{
    {
        std::list<MoveOnly, test_allocator<MoveOnly> > l(test_allocator<MoveOnly>(5));
        std::list<MoveOnly, test_allocator<MoveOnly> > lo(test_allocator<MoveOnly>(5));
        for (int i = 1; i <= 3; ++i)
        {
            l.push_back(i);
            lo.push_back(i);
        }
        std::list<MoveOnly, test_allocator<MoveOnly> > l2(std::move(l), test_allocator<MoveOnly>(6));
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(!l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == test_allocator<MoveOnly>(6));
    }
    {
        std::list<MoveOnly, test_allocator<MoveOnly> > l(test_allocator<MoveOnly>(5));
        std::list<MoveOnly, test_allocator<MoveOnly> > lo(test_allocator<MoveOnly>(5));
        for (int i = 1; i <= 3; ++i)
        {
            l.push_back(i);
            lo.push_back(i);
        }
        std::list<MoveOnly, test_allocator<MoveOnly> > l2(std::move(l), test_allocator<MoveOnly>(5));
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == test_allocator<MoveOnly>(5));
    }
    {
        std::list<MoveOnly, other_allocator<MoveOnly> > l(other_allocator<MoveOnly>(5));
        std::list<MoveOnly, other_allocator<MoveOnly> > lo(other_allocator<MoveOnly>(5));
        for (int i = 1; i <= 3; ++i)
        {
            l.push_back(i);
            lo.push_back(i);
        }
        std::list<MoveOnly, other_allocator<MoveOnly> > l2(std::move(l), other_allocator<MoveOnly>(4));
        TC_ASSERT_EXPR(l2 == lo);
        TC_ASSERT_EXPR(!l.empty());
        TC_ASSERT_EXPR(l2.get_allocator() == other_allocator<MoveOnly>(4));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
