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

// class vector

// bool empty() const noexcept;

#include <vector>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
    typedef std::vector<bool> C;
    C c;
    ASSERT_NOEXCEPT(c.empty());
    TC_ASSERT_EXPR(c.empty());
    c.push_back(false);
    TC_ASSERT_EXPR(!c.empty());
    c.clear();
    TC_ASSERT_EXPR(c.empty());
    }
#if TEST_STD_VER >= 11
    {
    typedef std::vector<bool, min_allocator<bool>> C;
    C c;
    ASSERT_NOEXCEPT(c.empty());
    TC_ASSERT_EXPR(c.empty());
    c.push_back(false);
    TC_ASSERT_EXPR(!c.empty());
    c.clear();
    TC_ASSERT_EXPR(c.empty());
    }
#endif

    return true;
}

int tc_containers_sequences_vector_bool_empty(void) {
    tests();
#if TEST_STD_VER > 17
    static_assert(tests());
#endif
    return 0;
}
