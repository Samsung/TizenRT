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
// UNSUPPORTED: c++03, c++11, c++14

// template <class T> constexpr add_const<T>& as_const(T& t) noexcept;      // C++17
// template <class T>           add_const<T>& as_const(const T&&) = delete; // C++17

#include <cassert>
#include <type_traits>
#include <utility>
#include "libcxx_tc_common.h"

struct S {int i;};
bool operator==(const S& x, const S& y) { return x.i == y.i; }
bool operator==(const volatile S& x, const volatile S& y) { return x.i == y.i; }

template<typename T>
void test(T& t)
{
    static_assert(std::is_const<typename std::remove_reference<decltype(std::as_const                  (t))>::type>::value, "");
    static_assert(std::is_const<typename std::remove_reference<decltype(std::as_const<               T>(t))>::type>::value, "");
    static_assert(std::is_const<typename std::remove_reference<decltype(std::as_const<const          T>(t))>::type>::value, "");
    static_assert(std::is_const<typename std::remove_reference<decltype(std::as_const<volatile       T>(t))>::type>::value, "");
    static_assert(std::is_const<typename std::remove_reference<decltype(std::as_const<const volatile T>(t))>::type>::value, "");

    TC_ASSERT_EXPR(std::as_const(t) == t);
    TC_ASSERT_EXPR(std::as_const<               T>(t) == t);
    TC_ASSERT_EXPR(std::as_const<const          T>(t) == t);
    TC_ASSERT_EXPR(std::as_const<volatile       T>(t) == t);
    TC_ASSERT_EXPR(std::as_const<const volatile T>(t) == t);
}

int tc_utilities_utility_as_const_as_const(void) {
    int i = 3;
    double d = 4.0;
    S s{2};
    test(i);
    test(d);
    test(s);

  return 0;
}
