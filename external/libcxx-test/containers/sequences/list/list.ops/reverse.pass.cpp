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

// void reverse();

#include <list>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_list_list_ops_reverse(void) {
    {
    int a1[] = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int a2[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    std::list<int> c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
    c1.reverse();
    TC_ASSERT_EXPR(c1 == std::list<int>(a2, a2+sizeof(a2)/sizeof(a2[0])));
    }
#if TEST_STD_VER >= 11
    {
    int a1[] = {11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
    int a2[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    std::list<int, min_allocator<int>> c1(a1, a1+sizeof(a1)/sizeof(a1[0]));
    c1.reverse();
    TC_ASSERT_EXPR((c1 == std::list<int, min_allocator<int>>(a2, a2+sizeof(a2)/sizeof(a2[0]))));
    }
#endif

  return 0;
}
