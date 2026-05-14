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
// <unordered_set>

// template <class Value, class Hash = hash<Value>, class Pred = equal_to<Value>,
//           class Alloc = allocator<Value>>
// class unordered_multiset

// float max_load_factor() const;
// void max_load_factor(float mlf);

#include <unordered_set>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_unord_unord_multiset_max_load_factor(void) {
    {
        typedef std::unordered_multiset<int> C;
        const C c;
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
    {
        typedef std::unordered_multiset<int> C;
        C c;
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
        c.max_load_factor(2.5);
        TC_ASSERT_EXPR(c.max_load_factor() == 2.5);
    }
#if TEST_STD_VER >= 11
    {
        typedef std::unordered_multiset<int, std::hash<int>,
                                      std::equal_to<int>, min_allocator<int>> C;
        const C c;
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
    }
    {
        typedef std::unordered_multiset<int, std::hash<int>,
                                      std::equal_to<int>, min_allocator<int>> C;
        C c;
        TC_ASSERT_EXPR(c.max_load_factor() == 1);
        c.max_load_factor(2.5);
        TC_ASSERT_EXPR(c.max_load_factor() == 2.5);
    }
#endif

    return 0;
}
