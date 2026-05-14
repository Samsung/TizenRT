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
// UNSUPPORTED: c++03, c++11

#include <tuple>
#include <utility>
#include <memory>
#include <string>
#include <complex>
#include <type_traits>

#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_tuple_tuple_tuple_tuple_elem_tuple_by_type(void) {
    typedef std::complex<float> cf;
    {
    auto t1 = std::tuple<int, std::string, cf> { 42, "Hi", { 1,2 }};
    TC_ASSERT_EXPR( std::get<int>(t1) == 42 ); // find at the beginning
    TC_ASSERT_EXPR( std::get<std::string>(t1) == "Hi" ); // find in the middle
    TC_ASSERT_EXPR( std::get<cf>(t1).real() == 1 ); // find at the end
    TC_ASSERT_EXPR( std::get<cf>(t1).imag() == 2 );
    }

    {
    auto t2 = std::tuple<int, std::string, int, cf> { 42, "Hi", 23, { 1,2 }};
//  get<int> would fail!
    TC_ASSERT_EXPR( std::get<std::string>(t2) == "Hi" );
    TC_ASSERT_EXPR(( std::get<cf>(t2) == cf{ 1,2 } ));
    }

    {
    constexpr std::tuple<int, const int, double, double> p5 { 1, 2, 3.4, 5.6 };
    static_assert ( std::get<int>(p5) == 1, "" );
    static_assert ( std::get<const int>(p5) == 2, "" );
    }

    {
    const std::tuple<int, const int, double, double> p5 { 1, 2, 3.4, 5.6 };
    const int &i1 = std::get<int>(p5);
    const int &i2 = std::get<const int>(p5);
    TC_ASSERT_EXPR( i1 == 1 );
    TC_ASSERT_EXPR( i2 == 2 );
    }

    {
    typedef std::unique_ptr<int> upint;
    std::tuple<upint> t(upint(new int(4)));
    upint p = std::get<upint>(std::move(t)); // get rvalue
    TC_ASSERT_EXPR(*p == 4);
    TC_ASSERT_EXPR(std::get<upint>(t) == nullptr); // has been moved from
    }

    {
    typedef std::unique_ptr<int> upint;
    const std::tuple<upint> t(upint(new int(4)));
    const upint&& p = std::get<upint>(std::move(t)); // get const rvalue
    TC_ASSERT_EXPR(*p == 4);
    TC_ASSERT_EXPR(std::get<upint>(t) != nullptr);
    }

    {
    int x = 42;
    int y = 43;
    std::tuple<int&, int const&> const t(x, y);
    static_assert(std::is_same<int&, decltype(std::get<int&>(std::move(t)))>::value, "");
    static_assert(noexcept(std::get<int&>(std::move(t))), "");
    static_assert(std::is_same<int const&, decltype(std::get<int const&>(std::move(t)))>::value, "");
    static_assert(noexcept(std::get<int const&>(std::move(t))), "");
    }

    {
    int x = 42;
    int y = 43;
    std::tuple<int&&, int const&&> const t(std::move(x), std::move(y));
    static_assert(std::is_same<int&&, decltype(std::get<int&&>(std::move(t)))>::value, "");
    static_assert(noexcept(std::get<int&&>(std::move(t))), "");
    static_assert(std::is_same<int const&&, decltype(std::get<int const&&>(std::move(t)))>::value, "");
    static_assert(noexcept(std::get<int const&&>(std::move(t))), "");
    }

    {
    constexpr const std::tuple<int, const int, double, double> t { 1, 2, 3.4, 5.6 };
    static_assert(std::get<int>(std::move(t)) == 1, "");
    static_assert(std::get<const int>(std::move(t)) == 2, "");
    }

  return 0;
}
