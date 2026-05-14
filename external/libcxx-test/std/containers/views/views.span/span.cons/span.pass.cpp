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

// template<class OtherElementType, size_t OtherExtent>
//    constexpr span(const span<OtherElementType, OtherExtent>& s) noexcept;
//
//  Remarks: This constructor shall not participate in overload resolution unless:
//      Extent == dynamic_extent || Extent == OtherExtent is true, and
//      OtherElementType(*)[] is convertible to ElementType(*)[].


#include <span>
#include <cassert>
#include <string>

#include "test_macros.h"
#include "libcxx_tc_common.h"

void checkCV()
{
    std::span<               int>   sp;
//  std::span<const          int>  csp;
    std::span<      volatile int>  vsp;
//  std::span<const volatile int> cvsp;

    std::span<               int, 0>   sp0;
//  std::span<const          int, 0>  csp0;
    std::span<      volatile int, 0>  vsp0;
//  std::span<const volatile int, 0> cvsp0;

//  dynamic -> dynamic
    {
        std::span<const          int> s1{  sp}; // a span<const          int> pointing at int.
        std::span<      volatile int> s2{  sp}; // a span<      volatile int> pointing at int.
        std::span<const volatile int> s3{  sp}; // a span<const volatile int> pointing at int.
        std::span<const volatile int> s4{ vsp}; // a span<const volatile int> pointing at volatile int.
        TC_ASSERT_EXPR(s1.size() + s2.size() + s3.size() + s4.size() == 0);
    }

//  static -> static
    {
        std::span<const          int, 0> s1{  sp0}; // a span<const          int> pointing at int.
        std::span<      volatile int, 0> s2{  sp0}; // a span<      volatile int> pointing at int.
        std::span<const volatile int, 0> s3{  sp0}; // a span<const volatile int> pointing at int.
        std::span<const volatile int, 0> s4{ vsp0}; // a span<const volatile int> pointing at volatile int.
        TC_ASSERT_EXPR(s1.size() + s2.size() + s3.size() + s4.size() == 0);
    }

//  static -> dynamic
    {
        std::span<const          int> s1{  sp0};    // a span<const          int> pointing at int.
        std::span<      volatile int> s2{  sp0};    // a span<      volatile int> pointing at int.
        std::span<const volatile int> s3{  sp0};    // a span<const volatile int> pointing at int.
        std::span<const volatile int> s4{ vsp0};    // a span<const volatile int> pointing at volatile int.
        TC_ASSERT_EXPR(s1.size() + s2.size() + s3.size() + s4.size() == 0);
    }

//  dynamic -> static (not allowed)
}


template <typename T>
constexpr bool testConstexprSpan()
{
    std::span<T>    s0{};
    std::span<T, 0> s1{};
    std::span<T>    s2(s1); // static -> dynamic
    ASSERT_NOEXCEPT(std::span<T>   {s0});
    ASSERT_NOEXCEPT(std::span<T, 0>{s1});
    ASSERT_NOEXCEPT(std::span<T>   {s1});

    return
        s1.data() == nullptr && s1.size() == 0
    &&  s2.data() == nullptr && s2.size() == 0;
}


template <typename T>
void testRuntimeSpan()
{
    std::span<T>    s0{};
    std::span<T, 0> s1{};
    std::span<T>    s2(s1); // static -> dynamic
    ASSERT_NOEXCEPT(std::span<T>   {s0});
    ASSERT_NOEXCEPT(std::span<T, 0>{s1});
    ASSERT_NOEXCEPT(std::span<T>   {s1});

    TC_ASSERT_EXPR(s1.data() == nullptr && s1.size() == 0);
    TC_ASSERT_EXPR(s2.data() == nullptr && s2.size() == 0);
}


struct A{};

int tc_containers_views_views_span_span_cons_span(void) {
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

  return 0;
}
