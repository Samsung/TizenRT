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

// void resize(size_type sz, const value_type& x);

#include <list>
#include <cassert>
#include "test_macros.h"
#include "DefaultOnly.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_list_list_capacity_resize_size_value(void) {
    {
        std::list<double> l(5, 2);
        l.resize(2, 3.5);
        TC_ASSERT_EXPR(l.size() == 2);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 2);
        TC_ASSERT_EXPR(l == std::list<double>(2, 2));
    }
    {
        std::list<double> l(5, 2);
        l.resize(10, 3.5);
        TC_ASSERT_EXPR(l.size() == 10);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 10);
        TC_ASSERT_EXPR(l.front() == 2);
        TC_ASSERT_EXPR(l.back() == 3.5);
    }
#if TEST_STD_VER >= 11
    {
        std::list<double, min_allocator<double>> l(5, 2);
        l.resize(2, 3.5);
        TC_ASSERT_EXPR(l.size() == 2);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 2);
        TC_ASSERT_EXPR((l == std::list<double, min_allocator<double>>(2, 2)));
    }
    {
        std::list<double, min_allocator<double>> l(5, 2);
        l.resize(10, 3.5);
        TC_ASSERT_EXPR(l.size() == 10);
        TC_ASSERT_EXPR(std::distance(l.begin(), l.end()) == 10);
        TC_ASSERT_EXPR(l.front() == 2);
        TC_ASSERT_EXPR(l.back() == 3.5);
    }
#endif

  return 0;
}
