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
// <map>

// class map

// map();

#include <map>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_map_map_cons_default(void) {
    {
    std::map<int, double> m;
    TC_ASSERT_EXPR(m.empty());
    TC_ASSERT_EXPR(m.begin() == m.end());
    }
#if TEST_STD_VER >= 11
    {
    std::map<int, double, std::less<int>, min_allocator<std::pair<const int, double>>> m;
    TC_ASSERT_EXPR(m.empty());
    TC_ASSERT_EXPR(m.begin() == m.end());
    }
    {
    typedef explicit_allocator<std::pair<const int, double>> A;
        {
        std::map<int, double, std::less<int>, A> m;
        TC_ASSERT_EXPR(m.empty());
        TC_ASSERT_EXPR(m.begin() == m.end());
        }
        {
        A a;
        std::map<int, double, std::less<int>, A> m(a);
        TC_ASSERT_EXPR(m.empty());
        TC_ASSERT_EXPR(m.begin() == m.end());
        }
    }
    {
    std::map<int, double> m = {};
    TC_ASSERT_EXPR(m.empty());
    TC_ASSERT_EXPR(m.begin() == m.end());
    }
#endif

  return 0;
}
