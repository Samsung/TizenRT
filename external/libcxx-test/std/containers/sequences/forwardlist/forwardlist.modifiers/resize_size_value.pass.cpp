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

// <forward_list>

// static int resize(size_type n, const value_type& v);

#include <forward_list>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "DefaultOnly.h"

#if TEST_STD_VER >= 11
#include "container_test_types.h"
#endif

int tc_libcxx_containers_forwardlist_modifiers_resize_size_value(void)
{
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t[] = {0, 1, 2, 3, 4};
        C c(std::begin(t), std::end(t));

        c.resize(3, 10);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 3);
        TC_ASSERT_EXPR(*next(c.begin(), 0) == 0);
        TC_ASSERT_EXPR(*next(c.begin(), 1) == 1);
        TC_ASSERT_EXPR(*next(c.begin(), 2) == 2);

        c.resize(6, 10);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 6);
        TC_ASSERT_EXPR(*next(c.begin(), 0) == 0);
        TC_ASSERT_EXPR(*next(c.begin(), 1) == 1);
        TC_ASSERT_EXPR(*next(c.begin(), 2) == 2);
        TC_ASSERT_EXPR(*next(c.begin(), 3) == 10);
        TC_ASSERT_EXPR(*next(c.begin(), 4) == 10);
        TC_ASSERT_EXPR(*next(c.begin(), 5) == 10);

        c.resize(6, 12);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 6);
        TC_ASSERT_EXPR(*next(c.begin(), 0) == 0);
        TC_ASSERT_EXPR(*next(c.begin(), 1) == 1);
        TC_ASSERT_EXPR(*next(c.begin(), 2) == 2);
        TC_ASSERT_EXPR(*next(c.begin(), 3) == 10);
        TC_ASSERT_EXPR(*next(c.begin(), 4) == 10);
        TC_ASSERT_EXPR(*next(c.begin(), 5) == 10);
    }
#if TEST_STD_VER >= 11
    {
        // Test that the allocator's construct method is being used to
        // construct the new elements and that it's called exactly N times.
        typedef std::forward_list<int, ContainerTestAllocator<int, int>> Container;
        ConstructController* cc = getConstructController();
        cc->reset();
        {
            Container c;
            cc->expect<int const&>(6);
            c.resize(6, 42);
            TC_ASSERT_EXPR(!cc->unchecked());
        }
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
