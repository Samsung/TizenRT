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

// vector& operator=(const vector& c);

#include <vector>
#include <cassert>
#include "test_macros.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
        std::vector<bool, test_allocator<bool> > l(3, true, test_allocator<bool>(5));
        std::vector<bool, test_allocator<bool> > l2(l, test_allocator<bool>(3));
        l2 = l;
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == test_allocator<bool>(3));
    }
    {
        std::vector<bool, other_allocator<bool> > l(3, true, other_allocator<bool>(5));
        std::vector<bool, other_allocator<bool> > l2(l, other_allocator<bool>(3));
        l2 = l;
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == other_allocator<bool>(5));
    }
#if TEST_STD_VER >= 11
    {
        std::vector<bool, min_allocator<bool> > l(3, true, min_allocator<bool>());
        std::vector<bool, min_allocator<bool> > l2(l, min_allocator<bool>());
        l2 = l;
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == min_allocator<bool>());
    }
#endif

    return true;
}

int tc_containers_sequences_vector_bool_assign_copy(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
