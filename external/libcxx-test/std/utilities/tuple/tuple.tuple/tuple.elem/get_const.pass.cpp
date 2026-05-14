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

// template <size_t I, class... Types>
//   typename tuple_element<I, tuple<Types...> >::type const&
//   get(const tuple<Types...>& t);

// UNSUPPORTED: c++03

#include <tuple>
#include <string>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct Empty {};

int tc_utilities_tuple_tuple_tuple_tuple_elem_get_const(void) {
    {
        typedef std::tuple<int> T;
        const T t(3);
        TC_ASSERT_EXPR(std::get<0>(t) == 3);
    }
    {
        typedef std::tuple<std::string, int> T;
        const T t("high", 5);
        TC_ASSERT_EXPR(std::get<0>(t) == "high");
        TC_ASSERT_EXPR(std::get<1>(t) == 5);
    }
#if TEST_STD_VER > 11
    {
        typedef std::tuple<double, int> T;
        constexpr T t(2.718, 5);
        static_assert(std::get<0>(t) == 2.718, "");
        static_assert(std::get<1>(t) == 5, "");
    }
    {
        typedef std::tuple<Empty> T;
        constexpr T t{Empty()};
        constexpr Empty e = std::get<0>(t);
        ((void)e); // Prevent unused warning
    }
#endif
    {
        typedef std::tuple<double&, std::string, int> T;
        double d = 1.5;
        const T t(d, "high", 5);
        TC_ASSERT_EXPR(std::get<0>(t) == 1.5);
        TC_ASSERT_EXPR(std::get<1>(t) == "high");
        TC_ASSERT_EXPR(std::get<2>(t) == 5);
        std::get<0>(t) = 2.5;
        TC_ASSERT_EXPR(std::get<0>(t) == 2.5);
        TC_ASSERT_EXPR(std::get<1>(t) == "high");
        TC_ASSERT_EXPR(std::get<2>(t) == 5);
        TC_ASSERT_EXPR(d == 2.5);
    }

  return 0;
}
