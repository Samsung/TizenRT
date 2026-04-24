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

// explicit map(const key_compare& comp);

#include <map>
#include <cassert>

#include "test_macros.h"
#include "../../../test_compare.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_associative_map_map_cons_compare(void) {
    {
    typedef test_less<int> C;
    const std::map<int, double, C> m(C(3));
    TC_ASSERT_EXPR(m.empty());
    TC_ASSERT_EXPR(m.begin() == m.end());
    TC_ASSERT_EXPR(m.key_comp() == C(3));
    }
#if TEST_STD_VER >= 11
    {
    typedef test_less<int> C;
    const std::map<int, double, C, min_allocator<std::pair<const int, double>>> m(C(3));
    TC_ASSERT_EXPR(m.empty());
    TC_ASSERT_EXPR(m.begin() == m.end());
    TC_ASSERT_EXPR(m.key_comp() == C(3));
    }
#endif

  return 0;
}
