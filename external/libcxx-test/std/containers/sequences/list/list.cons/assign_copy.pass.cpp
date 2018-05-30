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

// <list>

// list& operator=(const list& c);

#include <list>
#include <cassert>
#include "libcxx_tc_common.h"
#include "test_allocator.h"

int tc_libcxx_containers_list_cons_assign_copy(void)
{
    {
        std::list<int, test_allocator<int> > l(3, 2, test_allocator<int>(5));
        std::list<int, test_allocator<int> > l2(l, test_allocator<int>(3));
        l2 = l;
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == test_allocator<int>(3));
    }
    {
        std::list<int, other_allocator<int> > l(3, 2, other_allocator<int>(5));
        std::list<int, other_allocator<int> > l2(l, other_allocator<int>(3));
        l2 = l;
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == other_allocator<int>(5));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
