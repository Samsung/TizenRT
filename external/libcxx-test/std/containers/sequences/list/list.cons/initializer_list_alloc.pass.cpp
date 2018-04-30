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

// list(initializer_list<value_type> il, const Allocator& a = allocator_type());

#include <list>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_allocator.h"

int tc_libcxx_containers_list_cons_initializer_list_alloc(void)
{
    {
    std::list<int, test_allocator<int>> d({3, 4, 5, 6}, test_allocator<int>(3));
    TC_ASSERT_EXPR(d.get_allocator() == test_allocator<int>(3));
    TC_ASSERT_EXPR(d.size() == 4);
    std::list<int, test_allocator<int>>::iterator i = d.begin();
    TC_ASSERT_EXPR(*i++ == 3);
    TC_ASSERT_EXPR(*i++ == 4);
    TC_ASSERT_EXPR(*i++ == 5);
    TC_ASSERT_EXPR(*i++ == 6);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
