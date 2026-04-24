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
// <vector>

// void pop_back();

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"


TEST_CONSTEXPR_CXX20 bool tests()
{
    {
        std::vector<int> c;
        c.push_back(1);
        TC_ASSERT_EXPR(c.size() == 1);
        c.pop_back();
        TC_ASSERT_EXPR(c.size() == 0);

    }
#if TEST_STD_VER >= 11
    {
        std::vector<int, min_allocator<int>> c;
        c.push_back(1);
        TC_ASSERT_EXPR(c.size() == 1);
        c.pop_back();
        TC_ASSERT_EXPR(c.size() == 0);
    }
#endif

    { // LWG 526
        int arr[] = {0, 1, 2, 3, 4};
        int sz = 5;
        std::vector<int> c(arr, arr+sz);
        while (c.size() < c.capacity())
            c.push_back(sz++);
        c.push_back(c.front());
        TC_ASSERT_EXPR(c.back() == 0);
        for (int i = 0; i < sz; ++i)
            TC_ASSERT_EXPR(c[i] == i);
    }

    return true;
}

int tc_containers_sequences_vector_vector_modifiers_pop_back(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
