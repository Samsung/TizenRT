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
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <span>

// [[nodiscard]] constexpr bool empty() const noexcept;
//


#include <span>
#include <cassert>
#include <string>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A{};
constexpr int iArr1[] = { 0,  1,  2,  3,  4,  5,  6,  7,  8,  9};
          int iArr2[] = {10, 11, 12, 13, 14, 15, 16, 17, 18, 19};

int tc_containers_views_views_span_span_obs_empty(void) {
    static_assert( noexcept(std::span<int>   ().empty()), "");
    static_assert( noexcept(std::span<int, 0>().empty()), "");


    static_assert( std::span<int>().empty(),            "");
    static_assert( std::span<long>().empty(),           "");
    static_assert( std::span<double>().empty(),         "");
    static_assert( std::span<A>().empty(),              "");
    static_assert( std::span<std::string>().empty(),    "");

    static_assert( std::span<int, 0>().empty(),         "");
    static_assert( std::span<long, 0>().empty(),        "");
    static_assert( std::span<double, 0>().empty(),      "");
    static_assert( std::span<A, 0>().empty(),           "");
    static_assert( std::span<std::string, 0>().empty(), "");

    static_assert(!std::span<const int>(iArr1, 1).empty(), "");
    static_assert(!std::span<const int>(iArr1, 2).empty(), "");
    static_assert(!std::span<const int>(iArr1, 3).empty(), "");
    static_assert(!std::span<const int>(iArr1, 4).empty(), "");
    static_assert(!std::span<const int>(iArr1, 5).empty(), "");

    TC_ASSERT_EXPR( (std::span<int>().empty()           ));
    TC_ASSERT_EXPR( (std::span<long>().empty()          ));
    TC_ASSERT_EXPR( (std::span<double>().empty()        ));
    TC_ASSERT_EXPR( (std::span<A>().empty()             ));
    TC_ASSERT_EXPR( (std::span<std::string>().empty()   ));

    TC_ASSERT_EXPR( (std::span<int, 0>().empty()        ));
    TC_ASSERT_EXPR( (std::span<long, 0>().empty()       ));
    TC_ASSERT_EXPR( (std::span<double, 0>().empty()     ));
    TC_ASSERT_EXPR( (std::span<A, 0>().empty()          ));
    TC_ASSERT_EXPR( (std::span<std::string, 0>().empty()));

    TC_ASSERT_EXPR(!(std::span<int, 1>(iArr2, 1).empty()));
    TC_ASSERT_EXPR(!(std::span<int, 2>(iArr2, 2).empty()));
    TC_ASSERT_EXPR(!(std::span<int, 3>(iArr2, 3).empty()));
    TC_ASSERT_EXPR(!(std::span<int, 4>(iArr2, 4).empty()));
    TC_ASSERT_EXPR(!(std::span<int, 5>(iArr2, 5).empty()));

    std::string s;
    TC_ASSERT_EXPR( ((std::span<std::string>(&s, (std::size_t) 0)).empty()));
    TC_ASSERT_EXPR(!((std::span<std::string>(&s, 1).empty())));

  return 0;
}
