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
// <tuple>

// constexpr unspecified ignore;

// UNSUPPORTED: c++03

#include <cassert>
#include <tuple>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

constexpr bool test_ignore_constexpr()
{
#if TEST_STD_VER > 11
    { // Test that std::ignore provides constexpr converting assignment.
        auto& res = (std::ignore = 42);
        TC_ASSERT_EXPR(&res == &std::ignore);
    }
    { // Test that std::ignore provides constexpr copy/move constructors
        auto copy = std::ignore;
        auto moved = std::move(copy);
        ((void)moved);
    }
    { // Test that std::ignore provides constexpr copy/move assignment
        auto copy = std::ignore;
        copy = std::ignore;
        auto moved = std::ignore;
        moved = std::move(copy);
    }
#endif
    return true;
}

int tc_utilities_tuple_tuple_general_ignore(void) {
    {
        constexpr auto& ignore_v = std::ignore;
        ((void)ignore_v);
    }
    {
        static_assert(test_ignore_constexpr(), "");
    }
    {
        LIBCPP_STATIC_ASSERT(std::is_trivial<decltype(std::ignore)>::value, "");
    }

  return 0;
}
