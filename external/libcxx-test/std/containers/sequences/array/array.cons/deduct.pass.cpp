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
// UNSUPPORTED: c++03, c++11, c++14

// template <class T, class... U>
//   array(T, U...) -> array<T, 1 + sizeof...(U)>;
//
//  Requires: (is_same_v<T, U> && ...) is true. Otherwise the program is ill-formed.

#include <array>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "libcxx_tc_common.h"

constexpr bool tests()
{
    //  Test the explicit deduction guides
    {
        std::array arr{1,2,3};  // array(T, U...)
        static_assert(std::is_same_v<decltype(arr), std::array<int, 3>>, "");
        TC_ASSERT_EXPR(arr[0] == 1);
        TC_ASSERT_EXPR(arr[1] == 2);
        TC_ASSERT_EXPR(arr[2] == 3);
    }

    {
        const long l1 = 42;
        std::array arr{1L, 4L, 9L, l1}; // array(T, U...)
        static_assert(std::is_same_v<decltype(arr)::value_type, long>, "");
        static_assert(arr.size() == 4, "");
        TC_ASSERT_EXPR(arr[0] == 1);
        TC_ASSERT_EXPR(arr[1] == 4);
        TC_ASSERT_EXPR(arr[2] == 9);
        TC_ASSERT_EXPR(arr[3] == l1);
    }

    //  Test the implicit deduction guides
    {
        std::array<double, 2> source = {4.0, 5.0};
        std::array arr(source);   // array(array)
        static_assert(std::is_same_v<decltype(arr), decltype(source)>, "");
        static_assert(std::is_same_v<decltype(arr), std::array<double, 2>>, "");
        TC_ASSERT_EXPR(arr[0] == 4.0);
        TC_ASSERT_EXPR(arr[1] == 5.0);
    }

    return true;
}

int tc_containers_sequences_array_array_cons_deduct(void) {
    tests();
    static_assert(tests(), "");
    return 0;
}
