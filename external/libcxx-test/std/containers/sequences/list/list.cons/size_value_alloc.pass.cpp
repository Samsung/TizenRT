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

// list(size_type n, const T& value, const Allocator& = Allocator());

#include <list>
#include <cassert>
#include "libcxx_tc_common.h"
#include "DefaultOnly.h"
#include "test_allocator.h"

int tc_libcxx_containers_list_cons_size_value_alloc(void)
{
    {
        std::list<int> l(3, 2);
        TC_ASSERT_EXPR(l.size() == 3);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 3);
        std::list<int>::const_iterator i = l.begin();
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
    }
    {
        std::list<int> l(3, 2, std::allocator<int>());
        TC_ASSERT_EXPR(l.size() == 3);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 3);
        std::list<int>::const_iterator i = l.begin();
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
    }
    {
        // Add 2 for implementations that dynamically allocate a sentinel node and container proxy.
        std::list<int, limited_allocator<int, 3 + 2> > l(3, 2);
        TC_ASSERT_EXPR(l.size() == 3);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 3);
        std::list<int>::const_iterator i = l.begin();
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
        ++i;
        TC_ASSERT_EXPR(*i == 2);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
