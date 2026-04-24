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

// void push_back(const value_type& x);

#include <vector>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
        bool a[] = {0, 1, 1, 0, 1, 0, 0};
        const unsigned N = sizeof(a)/sizeof(a[0]);
        std::vector<bool> c;
        for (unsigned i = 0; i < N; ++i)
        {
            c.push_back(a[i]);
            TC_ASSERT_EXPR(c.size() == i+1);
            for (std::size_t j = 0; j < c.size(); ++j)
                TC_ASSERT_EXPR(c[j] == a[j]);
        }
    }
#if TEST_STD_VER >= 11
    {
        bool a[] = {0, 1, 1, 0, 1, 0, 0};
        const unsigned N = sizeof(a)/sizeof(a[0]);
        std::vector<bool, min_allocator<bool>> c;
        for (unsigned i = 0; i < N; ++i)
        {
            c.push_back(a[i]);
            TC_ASSERT_EXPR(c.size() == i+1);
            for (std::size_t j = 0; j < c.size(); ++j)
                TC_ASSERT_EXPR(c[j] == a[j]);
        }
    }
#endif

    return true;
}

int tc_containers_sequences_vector_bool_push_back(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
