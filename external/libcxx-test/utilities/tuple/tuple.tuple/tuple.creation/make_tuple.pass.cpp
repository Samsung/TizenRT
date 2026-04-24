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

// template<class... Types>
//   tuple<VTypes...> make_tuple(Types&&... t);

// UNSUPPORTED: c++03

#include <tuple>
#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20
bool test()
{
    int i = 0;
    float j = 0;
    std::tuple<int, int&, float&> t =
        std::make_tuple(1, std::ref(i), std::ref(j));
    TC_ASSERT_EXPR(std::get<0>(t) == 1);
    TC_ASSERT_EXPR(std::get<1>(t) == 0);
    TC_ASSERT_EXPR(std::get<2>(t) == 0);
    i = 2;
    j = 3.5;
    TC_ASSERT_EXPR(std::get<0>(t) == 1);
    TC_ASSERT_EXPR(std::get<1>(t) == 2);
    TC_ASSERT_EXPR(std::get<2>(t) == 3.5);
    std::get<1>(t) = 0;
    std::get<2>(t) = 0;
    TC_ASSERT_EXPR(i == 0);
    TC_ASSERT_EXPR(j == 0);

    return true;
}

int tc_utilities_tuple_tuple_tuple_tuple_creation_make_tuple(void) {
    test();
#if TEST_STD_VER >= 20
    static_assert(test());
#endif

#if TEST_STD_VER > 11
    {
        constexpr auto t1 = std::make_tuple(0, 1, 3.14);
        constexpr int i1 = std::get<1>(t1);
        constexpr double d1 = std::get<2>(t1);
        static_assert (i1 == 1, "" );
        static_assert (d1 == 3.14, "" );
    }
#endif

    return 0;
}
