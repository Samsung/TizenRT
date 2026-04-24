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

// void resize(size_type sz);

#include <list>
#include <cassert>
#include "test_macros.h"
#include "DefaultOnly.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_list_list_capacity_resize_size(void) {
    {
        std::list<int> l(5, 2);
        l.resize(2);
        TC_ASSERT_EXPR(l.size() == 2);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 2);
        TC_ASSERT_EXPR(l == std::list<int>(2, 2));
    }
    {
        std::list<int> l(5, 2);
        l.resize(10);
        TC_ASSERT_EXPR(l.size() == 10);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 10);
        TC_ASSERT_EXPR(l.front() == 2);
        TC_ASSERT_EXPR(l.back() == 0);
    }
#if TEST_STD_VER >= 11
    {
        std::list<DefaultOnly> l(10);
        l.resize(5);
        TC_ASSERT_EXPR(l.size() == 5);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 5);
    }
    {
        std::list<DefaultOnly> l(10);
        l.resize(20);
        TC_ASSERT_EXPR(l.size() == 20);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 20);
    }
    {
        std::list<int, min_allocator<int>> l(5, 2);
        l.resize(2);
        TC_ASSERT_EXPR(l.size() == 2);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 2);
        TC_ASSERT_EXPR((l == std::list<int, min_allocator<int>>(2, 2)));
    }
    {
        std::list<int, min_allocator<int>> l(5, 2);
        l.resize(10);
        TC_ASSERT_EXPR(l.size() == 10);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 10);
        TC_ASSERT_EXPR(l.front() == 2);
        TC_ASSERT_EXPR(l.back() == 0);
    }
    {
        std::list<DefaultOnly, min_allocator<DefaultOnly>> l(10);
        l.resize(5);
        TC_ASSERT_EXPR(l.size() == 5);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 5);
    }
    {
        std::list<DefaultOnly, min_allocator<DefaultOnly>> l(10);
        l.resize(20);
        TC_ASSERT_EXPR(l.size() == 20);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 20);
    }
#endif

    return 0;
}
