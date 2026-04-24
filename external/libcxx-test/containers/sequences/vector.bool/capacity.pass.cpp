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
// vector<bool>

// size_type capacity() const;

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
        std::vector<bool> v;
        TC_ASSERT_EXPR(v.capacity() == 0);
    }
    {
        std::vector<bool> v(100);
        TC_ASSERT_EXPR(v.capacity() >= 100);
        v.push_back(0);
        TC_ASSERT_EXPR(v.capacity() >= 101);
    }
#if TEST_STD_VER >= 11
    {
        std::vector<bool, min_allocator<bool>> v;
        TC_ASSERT_EXPR(v.capacity() == 0);
    }
    {
        std::vector<bool, min_allocator<bool>> v(100);
        TC_ASSERT_EXPR(v.capacity() >= 100);
        v.push_back(0);
        TC_ASSERT_EXPR(v.capacity() >= 101);
    }
#endif

    return true;
}

int tc_containers_sequences_vector_bool_capacity(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
