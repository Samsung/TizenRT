//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <span>

//  constexpr span(const span& other) noexcept = default;

#include <span>
#include <cassert>
#include <string>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <typename T>
constexpr bool doCopy(const T &rhs)
{
    ASSERT_NOEXCEPT(T{rhs});
    T lhs{rhs};
    return lhs.data() == rhs.data()
     &&    lhs.size() == rhs.size();
}

struct A{};

template <typename T>
void testCV ()
{
    int  arr[] = {1,2,3};
    TC_ASSERT_EXPR((doCopy(std::span<T>  ()          )));
    TC_ASSERT_EXPR((doCopy(std::span<T,0>()          )));
    TC_ASSERT_EXPR((doCopy(std::span<T>  (&arr[0], 1))));
    TC_ASSERT_EXPR((doCopy(std::span<T,1>(&arr[0], 1))));
    TC_ASSERT_EXPR((doCopy(std::span<T>  (&arr[0], 2))));
    TC_ASSERT_EXPR((doCopy(std::span<T,2>(&arr[0], 2))));
}


int tc_containers_views_views_span_span_cons_copy(void) {
    constexpr int carr[] = {1,2,3};

    static_assert(doCopy(std::span<      int>  ()),            "");
    static_assert(doCopy(std::span<      int,0>()),            "");
    static_assert(doCopy(std::span<const int>  (&carr[0], 1)), "");
    static_assert(doCopy(std::span<const int,1>(&carr[0], 1)), "");
    static_assert(doCopy(std::span<const int>  (&carr[0], 2)), "");
    static_assert(doCopy(std::span<const int,2>(&carr[0], 2)), "");

    static_assert(doCopy(std::span<long>()),   "");
    static_assert(doCopy(std::span<double>()), "");
    static_assert(doCopy(std::span<A>()),      "");

    std::string s;
    TC_ASSERT_EXPR(doCopy(std::span<std::string>   ()     ));
    TC_ASSERT_EXPR(doCopy(std::span<std::string, 0>()     ));
    TC_ASSERT_EXPR(doCopy(std::span<std::string>   (&s, 1)));
    TC_ASSERT_EXPR(doCopy(std::span<std::string, 1>(&s, 1)));

    testCV<               int>();
    testCV<const          int>();
    testCV<      volatile int>();
    testCV<const volatile int>();

  return 0;
}
