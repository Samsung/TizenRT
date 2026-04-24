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

// void resize(size_type sz, const value_type& x);

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
        std::vector<bool> v(100);
        v.resize(50, 1);
        TC_ASSERT_EXPR(v.size() == 50);
        TC_ASSERT_EXPR(v.capacity() >= 100);
        TC_ASSERT_EXPR(v == std::vector<bool>(50));
        v.resize(200, 1);
        TC_ASSERT_EXPR(v.size() == 200);
        TC_ASSERT_EXPR(v.capacity() >= 200);
        for (unsigned i = 0; i < 50; ++i)
            TC_ASSERT_EXPR(v[i] == 0);
        for (unsigned i = 50; i < 200; ++i)
            TC_ASSERT_EXPR(v[i] == 1);
    }
#if TEST_STD_VER >= 11
    {
        std::vector<bool, min_allocator<bool>> v(100);
        v.resize(50, 1);
        TC_ASSERT_EXPR(v.size() == 50);
        TC_ASSERT_EXPR(v.capacity() >= 100);
        TC_ASSERT_EXPR((v == std::vector<bool, min_allocator<bool>>(50)));
        v.resize(200, 1);
        TC_ASSERT_EXPR(v.size() == 200);
        TC_ASSERT_EXPR(v.capacity() >= 200);
        for (unsigned i = 0; i < 50; ++i)
            TC_ASSERT_EXPR(v[i] == 0);
        for (unsigned i = 50; i < 200; ++i)
            TC_ASSERT_EXPR(v[i] == 1);
    }
#endif

    return true;
}

int tc_containers_sequences_vector_bool_resize_size_value(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
