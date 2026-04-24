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
//   const typename tuple_element<I, tuple<Types...> >::type&&
//   get(const tuple<Types...>&& t);

// UNSUPPORTED: c++03

#include <tuple>
#include <utility>
#include <string>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_tuple_tuple_tuple_tuple_elem_get_const_rv(void) {
    {
    typedef std::tuple<int> T;
    const T t(3);
    static_assert(std::is_same<const int&&, decltype(std::get<0>(std::move(t)))>::value, "");
    static_assert(noexcept(std::get<0>(std::move(t))), "");
    const int&& i = std::get<0>(std::move(t));
    TC_ASSERT_EXPR(i == 3);
    }

    {
    typedef std::tuple<std::string, int> T;
    const T t("high", 5);
    static_assert(std::is_same<const std::string&&, decltype(std::get<0>(std::move(t)))>::value, "");
    static_assert(noexcept(std::get<0>(std::move(t))), "");
    static_assert(std::is_same<const int&&, decltype(std::get<1>(std::move(t)))>::value, "");
    static_assert(noexcept(std::get<1>(std::move(t))), "");
    const std::string&& s = std::get<0>(std::move(t));
    const int&& i = std::get<1>(std::move(t));
    TC_ASSERT_EXPR(s == "high");
    TC_ASSERT_EXPR(i == 5);
    }

    {
    int x = 42;
    int const y = 43;
    std::tuple<int&, int const&> const p(x, y);
    static_assert(std::is_same<int&, decltype(std::get<0>(std::move(p)))>::value, "");
    static_assert(noexcept(std::get<0>(std::move(p))), "");
    static_assert(std::is_same<int const&, decltype(std::get<1>(std::move(p)))>::value, "");
    static_assert(noexcept(std::get<1>(std::move(p))), "");
    }

    {
    int x = 42;
    int const y = 43;
    std::tuple<int&&, int const&&> const p(std::move(x), std::move(y));
    static_assert(std::is_same<int&&, decltype(std::get<0>(std::move(p)))>::value, "");
    static_assert(noexcept(std::get<0>(std::move(p))), "");
    static_assert(std::is_same<int const&&, decltype(std::get<1>(std::move(p)))>::value, "");
    static_assert(noexcept(std::get<1>(std::move(p))), "");
    }

#if TEST_STD_VER > 11
    {
    typedef std::tuple<double, int> T;
    constexpr const T t(2.718, 5);
    static_assert(std::get<0>(std::move(t)) == 2.718, "");
    static_assert(std::get<1>(std::move(t)) == 5, "");
    }
#endif

  return 0;
}
