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

#include <utility>
#include <string>
#include <type_traits>
#include <complex>
#include <memory>

#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_utility_pairs_pair_astuple_pairs_by_type(void) {
    typedef std::complex<float> cf;
    {
    auto t1 = std::make_pair<int, cf> ( 42, { 1,2 } );
    TC_ASSERT_EXPR( std::get<int>(t1) == 42 );
    TC_ASSERT_EXPR( std::get<cf>(t1).real() == 1 );
    TC_ASSERT_EXPR( std::get<cf>(t1).imag() == 2 );
    }

    {
    const std::pair<int, const int> p1 { 1, 2 };
    const int &i1 = std::get<int>(p1);
    const int &i2 = std::get<const int>(p1);
    TC_ASSERT_EXPR( i1 == 1 );
    TC_ASSERT_EXPR( i2 == 2 );
    }

    {
    typedef std::unique_ptr<int> upint;
    std::pair<upint, int> t(upint(new int(4)), 42);
    upint p = std::get<upint>(std::move(t)); // get rvalue
    TC_ASSERT_EXPR(*p == 4);
    TC_ASSERT_EXPR(std::get<upint>(t) == nullptr); // has been moved from
    }

    {
    typedef std::unique_ptr<int> upint;
    const std::pair<upint, int> t(upint(new int(4)), 42);
    static_assert(std::is_same<const upint&&, decltype(std::get<upint>(std::move(t)))>::value, "");
    static_assert(noexcept(std::get<upint>(std::move(t))), "");
    static_assert(std::is_same<const int&&, decltype(std::get<int>(std::move(t)))>::value, "");
    static_assert(noexcept(std::get<int>(std::move(t))), "");
    auto&& p = std::get<upint>(std::move(t)); // get const rvalue
    auto&& i = std::get<int>(std::move(t)); // get const rvalue
    TC_ASSERT_EXPR(*p == 4);
    TC_ASSERT_EXPR(i == 42);
    TC_ASSERT_EXPR(std::get<upint>(t) != nullptr);
    }

    {
    int x = 42;
    int const y = 43;
    std::pair<int&, int const&> const p(x, y);
    static_assert(std::is_same<int&, decltype(std::get<int&>(std::move(p)))>::value, "");
    static_assert(noexcept(std::get<int&>(std::move(p))), "");
    static_assert(std::is_same<int const&, decltype(std::get<int const&>(std::move(p)))>::value, "");
    static_assert(noexcept(std::get<int const&>(std::move(p))), "");
    }

    {
    int x = 42;
    int const y = 43;
    std::pair<int&&, int const&&> const p(std::move(x), std::move(y));
    static_assert(std::is_same<int&&, decltype(std::get<int&&>(std::move(p)))>::value, "");
    static_assert(noexcept(std::get<int&&>(std::move(p))), "");
    static_assert(std::is_same<int const&&, decltype(std::get<int const&&>(std::move(p)))>::value, "");
    static_assert(noexcept(std::get<int const&&>(std::move(p))), "");
    }

    {
    constexpr const std::pair<int, const int> p { 1, 2 };
    static_assert(std::get<int>(std::move(p)) == 1, "");
    static_assert(std::get<const int>(std::move(p)) == 2, "");
    }

  return 0;
}
