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

// reference front();  // constexpr in C++17
// reference back();   // constexpr in C++17

#include <array>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX17 bool tests()
{
    {
        typedef double T;
        typedef std::array<T, 3> C;
        C c = {1, 2, 3.5};

        C::reference r1 = c.front();
        TC_ASSERT_EXPR(r1 == 1);
        r1 = 5.5;
        TC_ASSERT_EXPR(c[0] == 5.5);

        C::reference r2 = c.back();
        TC_ASSERT_EXPR(r2 == 3.5);
        r2 = 7.5;
        TC_ASSERT_EXPR(c[2] == 7.5);
    }
    {
        typedef double T;
        typedef std::array<T, 0> C;
        C c = {};
        ASSERT_SAME_TYPE(decltype(c.back()), C::reference);
        LIBCPP_ASSERT_NOEXCEPT(c.back());
        ASSERT_SAME_TYPE(decltype(c.front()), C::reference);
        LIBCPP_ASSERT_NOEXCEPT(c.front());
        if (c.size() > (0)) { // always false
            TEST_IGNORE_NODISCARD c.front();
            TEST_IGNORE_NODISCARD c.back();
        }
    }
    {
        typedef double T;
        typedef std::array<const T, 0> C;
        C c = {};
        ASSERT_SAME_TYPE(decltype( c.back()), C::reference);
        LIBCPP_ASSERT_NOEXCEPT(    c.back());
        ASSERT_SAME_TYPE(decltype( c.front()), C::reference);
        LIBCPP_ASSERT_NOEXCEPT(    c.front());
        if (c.size() > (0)) {
            TEST_IGNORE_NODISCARD c.front();
            TEST_IGNORE_NODISCARD c.back();
        }
    }

    return true;
}

int tc_containers_sequences_array_front_back(void) {
    tests();
#if TEST_STD_VER >= 17
    static_assert(tests(), "");
#endif
    return 0;
}
