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
// <list>

// void      unique(); // before C++20
// size_type unique(); // C++20 and later

#include <list>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_list_list_ops_unique(void) {
    {
    int a1[] = {2, 1, 1, 4, 4, 4, 4, 3, 3};
    int a2[] = {2, 1, 4, 3};
    typedef std::list<int> L;
    L c(a1, a1+sizeof(a1)/sizeof(a1[0]));
#if TEST_STD_VER > 17
    ASSERT_SAME_TYPE(L::size_type, decltype(c.unique()));
    TC_ASSERT_EXPR(c.unique() == 5);
#else
    ASSERT_SAME_TYPE(void,         decltype(c.unique()));
    c.unique();
#endif
    TC_ASSERT_EXPR(c == std::list<int>(a2, a2+4));
    }
#if TEST_STD_VER >= 11
    {
    int a1[] = {2, 1, 1, 4, 4, 4, 4, 3, 3};
    int a2[] = {2, 1, 4, 3};
    std::list<int, min_allocator<int>> c(a1, a1+sizeof(a1)/sizeof(a1[0]));
#if TEST_STD_VER > 17
    TC_ASSERT_EXPR(c.unique() == 5);
#else
    c.unique();
#endif
    TC_ASSERT_EXPR((c == std::list<int, min_allocator<int>>(a2, a2+4)));
    }
#endif

  return 0;
}
