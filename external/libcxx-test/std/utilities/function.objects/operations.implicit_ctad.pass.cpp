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
// UNSUPPORTED: c++98, c++03, c++11, c++14

// <functional>

// Make sure that we can use CTAD with operations in <functional>

#include <functional>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_operations_implicit_ctad(void) {
    {
        std::plus f;
        ASSERT_SAME_TYPE(decltype(f), std::plus<>);
    }
    {
        std::minus f;
        ASSERT_SAME_TYPE(decltype(f), std::minus<>);
    }
    {
        std::multiplies f;
        ASSERT_SAME_TYPE(decltype(f), std::multiplies<>);
    }
    {
        std::divides f;
        ASSERT_SAME_TYPE(decltype(f), std::divides<>);
    }
    {
        std::modulus f;
        ASSERT_SAME_TYPE(decltype(f), std::modulus<>);
    }
    {
        std::negate f;
        ASSERT_SAME_TYPE(decltype(f), std::negate<>);
    }
    {
        std::bit_and f;
        ASSERT_SAME_TYPE(decltype(f), std::bit_and<>);
    }
    {
        std::bit_not f;
        ASSERT_SAME_TYPE(decltype(f), std::bit_not<>);
    }
    {
        std::bit_or f;
        ASSERT_SAME_TYPE(decltype(f), std::bit_or<>);
    }
    {
        std::bit_xor f;
        ASSERT_SAME_TYPE(decltype(f), std::bit_xor<>);
    }
    {
        std::equal_to f;
        ASSERT_SAME_TYPE(decltype(f), std::equal_to<>);
    }
    {
        std::not_equal_to f;
        ASSERT_SAME_TYPE(decltype(f), std::not_equal_to<>);
    }
    {
        std::less f;
        ASSERT_SAME_TYPE(decltype(f), std::less<>);
    }
    {
        std::less_equal f;
        ASSERT_SAME_TYPE(decltype(f), std::less_equal<>);
    }
    {
        std::greater_equal f;
        ASSERT_SAME_TYPE(decltype(f), std::greater_equal<>);
    }
    {
        std::greater f;
        ASSERT_SAME_TYPE(decltype(f), std::greater<>);
    }
    {
        std::logical_and f;
        ASSERT_SAME_TYPE(decltype(f), std::logical_and<>);
    }
    {
        std::logical_not f;
        ASSERT_SAME_TYPE(decltype(f), std::logical_not<>);
    }
    {
        std::logical_or f;
        ASSERT_SAME_TYPE(decltype(f), std::logical_or<>);
    }

    return 0;
}
