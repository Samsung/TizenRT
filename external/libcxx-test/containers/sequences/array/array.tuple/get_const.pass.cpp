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

// template <size_t I, class T, size_t N> const T& get(const array<T, N>& a);

#include <array>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX14 bool tests()
{
    {
        std::array<double, 1> const array = {3.3};
        TC_ASSERT_EXPR(std::get<0>(array) == 3.3);
    }
    {
        std::array<double, 2> const array = {3.3, 4.4};
        TC_ASSERT_EXPR(std::get<0>(array) == 3.3);
        TC_ASSERT_EXPR(std::get<1>(array) == 4.4);
    }
    {
        std::array<double, 3> const array = {3.3, 4.4, 5.5};
        TC_ASSERT_EXPR(std::get<0>(array) == 3.3);
        TC_ASSERT_EXPR(std::get<1>(array) == 4.4);
        TC_ASSERT_EXPR(std::get<2>(array) == 5.5);
    }
    {
        std::array<double, 1> const array = {3.3};
        static_assert(std::is_same<double const&, decltype(std::get<0>(array))>::value, "");
    }

    return true;
}

int tc_containers_sequences_array_array_tuple_get_const(void) {
    tests();
#if TEST_STD_VER >= 14
    static_assert(tests(), "");
#endif
    return 0;
}
