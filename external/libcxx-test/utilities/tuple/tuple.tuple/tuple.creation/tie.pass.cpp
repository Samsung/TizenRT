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
//   tuple<Types&...> tie(Types&... t);

// UNSUPPORTED: c++03

#include <tuple>
#include <string>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX14
bool test_tie()
{
    {
        int i = 42;
        double f = 1.1;
        using ExpectT = std::tuple<int&, decltype(std::ignore)&, double&>;
        auto res = std::tie(i, std::ignore, f);
        static_assert(std::is_same<ExpectT, decltype(res)>::value, "");
        TC_ASSERT_EXPR(&std::get<0>(res) == &i);
        TC_ASSERT_EXPR(&std::get<1>(res) == &std::ignore);
        TC_ASSERT_EXPR(&std::get<2>(res) == &f);

#if TEST_STD_VER >= 20
        res = std::make_tuple(101, nullptr, -1.0);
        TC_ASSERT_EXPR(i == 101);
        TC_ASSERT_EXPR(f == -1.0);
#endif
    }
    return true;
}

int tc_utilities_tuple_tuple_tuple_tuple_creation_tie(void) {
    test_tie();
#if TEST_STD_VER >= 14
    static_assert(test_tie(), "");
#endif

    {
        int i = 0;
        std::string s;
        std::tie(i, std::ignore, s) = std::make_tuple(42, 3.14, "C++");
        TC_ASSERT_EXPR(i == 42);
        TC_ASSERT_EXPR(s == "C++");
    }

    return 0;
}
