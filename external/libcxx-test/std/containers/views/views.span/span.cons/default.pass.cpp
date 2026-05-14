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

// constexpr span() noexcept;

#include <span>
#include <cassert>
#include <string>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

void checkCV()
{
//  Types the same (dynamic sized)
    {
    std::span<               int> s1;
    std::span<const          int> s2;
    std::span<      volatile int> s3;
    std::span<const volatile int> s4;
    TC_ASSERT_EXPR(s1.size() + s2.size() + s3.size() + s4.size() == 0);
    }

//  Types the same (static sized)
    {
    std::span<               int,0> s1;
    std::span<const          int,0> s2;
    std::span<      volatile int,0> s3;
    std::span<const volatile int,0> s4;
    TC_ASSERT_EXPR(s1.size() + s2.size() + s3.size() + s4.size() == 0);
    }
}


template <typename T>
constexpr bool testConstexprSpan()
{
    std::span<const T>    s1;
    std::span<const T, 0> s2;
    return
        s1.data() == nullptr && s1.size() == 0
    &&  s2.data() == nullptr && s2.size() == 0;
}


template <typename T>
void testRuntimeSpan()
{
    ASSERT_NOEXCEPT(T{});
    std::span<const T>    s1;
    std::span<const T, 0> s2;
    TC_ASSERT_EXPR(s1.data() == nullptr && s1.size() == 0);
    TC_ASSERT_EXPR(s2.data() == nullptr && s2.size() == 0);
}


struct A{};

int tc_containers_views_views_span_span_cons_default(void) {
    static_assert(testConstexprSpan<int>(),    "");
    static_assert(testConstexprSpan<long>(),   "");
    static_assert(testConstexprSpan<double>(), "");
    static_assert(testConstexprSpan<A>(),      "");

    testRuntimeSpan<int>();
    testRuntimeSpan<long>();
    testRuntimeSpan<double>();
    testRuntimeSpan<std::string>();
    testRuntimeSpan<A>();

    checkCV();

    static_assert( std::is_default_constructible_v<std::span<int, std::dynamic_extent>>, "");
    static_assert( std::is_default_constructible_v<std::span<int, 0>>, "");
    static_assert(!std::is_default_constructible_v<std::span<int, 2>>, "");

    return 0;
}
