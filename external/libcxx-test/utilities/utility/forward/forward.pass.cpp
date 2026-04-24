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
// UNSUPPORTED: c++03

// test forward

#include <utility>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A
{
};

A source() TEST_NOEXCEPT {return A();}
const A csource() TEST_NOEXCEPT {return A();}


#if TEST_STD_VER > 11
constexpr bool test_constexpr_forward() {
    int x = 42;
    const int cx = 101;
    return std::forward<int&>(x)        == 42
        && std::forward<int>(x)         == 42
        && std::forward<const int&>(x)  == 42
        && std::forward<const int>(x)   == 42
        && std::forward<int&&>(x)       == 42
        && std::forward<const int&&>(x) == 42
        && std::forward<const int&>(cx) == 101
        && std::forward<const int>(cx)  == 101;
}
#endif

int tc_utilities_utility_forward_forward(void) {
    A a;
    const A ca = A();

    ((void)a); // Prevent unused warning
    ((void)ca); // Prevent unused warning

    static_assert(std::is_same<decltype(std::forward<A&>(a)), A&>::value, "");
    static_assert(std::is_same<decltype(std::forward<A>(a)), A&&>::value, "");
    static_assert(std::is_same<decltype(std::forward<A>(source())), A&&>::value, "");
    ASSERT_NOEXCEPT(std::forward<A&>(a));
    ASSERT_NOEXCEPT(std::forward<A>(a));
    ASSERT_NOEXCEPT(std::forward<A>(source()));

    static_assert(std::is_same<decltype(std::forward<const A&>(a)), const A&>::value, "");
    static_assert(std::is_same<decltype(std::forward<const A>(a)), const A&&>::value, "");
    static_assert(std::is_same<decltype(std::forward<const A>(source())), const A&&>::value, "");
    ASSERT_NOEXCEPT(std::forward<const A&>(a));
    ASSERT_NOEXCEPT(std::forward<const A>(a));
    ASSERT_NOEXCEPT(std::forward<const A>(source()));

    static_assert(std::is_same<decltype(std::forward<const A&>(ca)), const A&>::value, "");
    static_assert(std::is_same<decltype(std::forward<const A>(ca)), const A&&>::value, "");
    static_assert(std::is_same<decltype(std::forward<const A>(csource())), const A&&>::value, "");
    ASSERT_NOEXCEPT(std::forward<const A&>(ca));
    ASSERT_NOEXCEPT(std::forward<const A>(ca));
    ASSERT_NOEXCEPT(std::forward<const A>(csource()));

#if TEST_STD_VER > 11
    {
    constexpr int i2 = std::forward<int>(42);
    static_assert(std::forward<int>(42) == 42, "");
    static_assert(std::forward<const int&>(i2) == 42, "");
    static_assert(test_constexpr_forward(), "");
    }
#endif
#if TEST_STD_VER == 11 && defined(_LIBCPP_VERSION)
    // Test that std::forward is constexpr in C++11. This is an extension
    // provided by both libc++ and libstdc++.
    {
    constexpr int i2 = std::forward<int>(42);
    static_assert(std::forward<int>(42) == 42, "" );
    static_assert(std::forward<const int&>(i2) == 42, "");
    }
#endif

  return 0;
}
