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
//   typename tuple_element<I, tuple<Types...> >::type&
//   get(tuple<Types...>& t);

// UNSUPPORTED: c++03

#include <tuple>
#include <string>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER > 11

struct Empty {};

struct S {
   std::tuple<int, Empty> a;
   int k;
   Empty e;
   constexpr S() : a{1,Empty{}}, k(std::get<0>(a)), e(std::get<1>(a)) {}
   };

constexpr std::tuple<int, int> getP () { return { 3, 4 }; }
#endif

int tc_utilities_tuple_tuple_tuple_tuple_elem_get_non_const(void) {
    {
        typedef std::tuple<int> T;
        T t(3);
        TC_ASSERT_EXPR(std::get<0>(t) == 3);
        std::get<0>(t) = 2;
        TC_ASSERT_EXPR(std::get<0>(t) == 2);
    }
    {
        typedef std::tuple<std::string, int> T;
        T t("high", 5);
        TC_ASSERT_EXPR(std::get<0>(t) == "high");
        TC_ASSERT_EXPR(std::get<1>(t) == 5);
        std::get<0>(t) = "four";
        std::get<1>(t) = 4;
        TC_ASSERT_EXPR(std::get<0>(t) == "four");
        TC_ASSERT_EXPR(std::get<1>(t) == 4);
    }
    {
        typedef std::tuple<double&, std::string, int> T;
        double d = 1.5;
        T t(d, "high", 5);
        TC_ASSERT_EXPR(std::get<0>(t) == 1.5);
        TC_ASSERT_EXPR(std::get<1>(t) == "high");
        TC_ASSERT_EXPR(std::get<2>(t) == 5);
        std::get<0>(t) = 2.5;
        std::get<1>(t) = "four";
        std::get<2>(t) = 4;
        TC_ASSERT_EXPR(std::get<0>(t) == 2.5);
        TC_ASSERT_EXPR(std::get<1>(t) == "four");
        TC_ASSERT_EXPR(std::get<2>(t) == 4);
        TC_ASSERT_EXPR(d == 2.5);
    }
#if TEST_STD_VER > 11
    { // get on an rvalue tuple
        static_assert ( std::get<0> ( std::make_tuple ( 0.0f, 1, 2.0, 3L )) == 0, "" );
        static_assert ( std::get<1> ( std::make_tuple ( 0.0f, 1, 2.0, 3L )) == 1, "" );
        static_assert ( std::get<2> ( std::make_tuple ( 0.0f, 1, 2.0, 3L )) == 2, "" );
        static_assert ( std::get<3> ( std::make_tuple ( 0.0f, 1, 2.0, 3L )) == 3, "" );
        static_assert(S().k == 1, "");
        static_assert(std::get<1>(getP()) == 4, "");
    }
#endif


  return 0;
}
