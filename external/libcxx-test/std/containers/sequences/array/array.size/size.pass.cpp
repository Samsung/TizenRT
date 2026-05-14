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

// template <class T, size_t N> constexpr size_type array<T,N>::size();

#include <array>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_array_array_size_size(void) {
    {
        typedef double T;
        typedef std::array<T, 3> C;
        C c = {1, 2, 3.5};
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.max_size() == 3);
        TC_ASSERT_EXPR(!c.empty());
    }
    {
        typedef double T;
        typedef std::array<T, 0> C;
        C c = {};
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(c.max_size() == 0);
        TC_ASSERT_EXPR(c.empty());
    }
#if TEST_STD_VER >= 11
    {
        typedef double T;
        typedef std::array<T, 3> C;
        constexpr C c = {1, 2, 3.5};
        static_assert(c.size() == 3, "");
        static_assert(c.max_size() == 3, "");
        static_assert(!c.empty(), "");
    }
    {
        typedef double T;
        typedef std::array<T, 0> C;
        constexpr C c = {};
        static_assert(c.size() == 0, "");
        static_assert(c.max_size() == 0, "");
        static_assert(c.empty(), "");
    }
#endif

    return 0;
}
