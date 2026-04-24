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

// template<class... Types>
//     tuple<Types&&...> forward_as_tuple(Types&&... t);

// UNSUPPORTED: c++03

#include <tuple>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class Tuple>
void
test0(const Tuple&)
{
    static_assert(std::tuple_size<Tuple>::value == 0, "");
}

template <class Tuple>
void
test1a(const Tuple& t)
{
    static_assert(std::tuple_size<Tuple>::value == 1, "");
    static_assert(std::is_same<typename std::tuple_element<0, Tuple>::type, int&&>::value, "");
    TC_ASSERT_EXPR(std::get<0>(t) == 1);
}

template <class Tuple>
void
test1b(const Tuple& t)
{
    static_assert(std::tuple_size<Tuple>::value == 1, "");
    static_assert(std::is_same<typename std::tuple_element<0, Tuple>::type, int&>::value, "");
    TC_ASSERT_EXPR(std::get<0>(t) == 2);
}

template <class Tuple>
void
test2a(const Tuple& t)
{
    static_assert(std::tuple_size<Tuple>::value == 2, "");
    static_assert(std::is_same<typename std::tuple_element<0, Tuple>::type, double&>::value, "");
    static_assert(std::is_same<typename std::tuple_element<1, Tuple>::type, char&>::value, "");
    TC_ASSERT_EXPR(std::get<0>(t) == 2.5);
    TC_ASSERT_EXPR(std::get<1>(t) == 'a');
}

#if TEST_STD_VER > 11
template <class Tuple>
constexpr int
test3(const Tuple&)
{
    return std::tuple_size<Tuple>::value;
}
#endif

int tc_utilities_tuple_tuple_tuple_tuple_creation_forward_as_tuple(void) {
    {
        test0(std::forward_as_tuple());
    }
    {
        test1a(std::forward_as_tuple(1));
    }
    {
        int i = 2;
        test1b(std::forward_as_tuple(i));
    }
    {
        double i = 2.5;
        char c = 'a';
        test2a(std::forward_as_tuple(i, c));
#if TEST_STD_VER > 11
        static_assert ( test3 (std::forward_as_tuple(i, c)) == 2, "" );
#endif
    }

  return 0;
}
