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

// template <class... Types> class tuple;

// tuple(const tuple& u) = default;

// UNSUPPORTED: c++03

#include <tuple>
#include <string>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct Empty {};

int tc_utilities_tuple_tuple_tuple_tuple_cnstr_copy(void) {
    {
        typedef std::tuple<> T;
        T t0;
        T t = t0;
        ((void)t); // Prevent unused warning
    }
    {
        typedef std::tuple<int> T;
        T t0(2);
        T t = t0;
        TC_ASSERT_EXPR(std::get<0>(t) == 2);
    }
    {
        typedef std::tuple<int, char> T;
        T t0(2, 'a');
        T t = t0;
        TC_ASSERT_EXPR(std::get<0>(t) == 2);
        TC_ASSERT_EXPR(std::get<1>(t) == 'a');
    }
    {
        typedef std::tuple<int, char, std::string> T;
        const T t0(2, 'a', "some text");
        T t = t0;
        TC_ASSERT_EXPR(std::get<0>(t) == 2);
        TC_ASSERT_EXPR(std::get<1>(t) == 'a');
        TC_ASSERT_EXPR(std::get<2>(t) == "some text");
    }
#if TEST_STD_VER > 11
    {
        typedef std::tuple<int> T;
        constexpr T t0(2);
        constexpr T t = t0;
        static_assert(std::get<0>(t) == 2, "");
    }
    {
        typedef std::tuple<Empty> T;
        constexpr T t0;
        constexpr T t = t0;
        constexpr Empty e = std::get<0>(t);
        ((void)e); // Prevent unused warning
    }
#endif

  return 0;
}
