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
// <array>

// class array

// constexpr bool max_size() const noexcept;

#include <array>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX14 bool tests()
{
    {
        typedef std::array<int, 2> C;
        C c = {};
        ASSERT_NOEXCEPT(c.max_size());
        TC_ASSERT_EXPR(c.max_size() == 2);
    }
    {
        typedef std::array<int, 0> C;
        C c = {};
        ASSERT_NOEXCEPT(c.max_size());
        TC_ASSERT_EXPR(c.max_size() == 0);
    }

    return true;
}

int tc_containers_sequences_array_max_size(void) {
    tests();
#if TEST_STD_VER >= 14
    static_assert(tests(), "");
#endif

#if TEST_STD_VER >= 11
    // Sanity check for constexpr in C++11
    {
        constexpr std::array<int, 3> array = {};
        static_assert(array.max_size() == 3, "");
    }
#endif

    return 0;
}
