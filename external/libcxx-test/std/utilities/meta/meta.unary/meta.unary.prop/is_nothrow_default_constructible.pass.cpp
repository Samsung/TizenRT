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
// type_traits

// is_nothrow_default_constructible

#include <type_traits>
#include "test_macros.h"

#include "common.h"
#include "libcxx_tc_common.h"

template <class T>
void test_is_nothrow_default_constructible()
{
    static_assert( std::is_nothrow_default_constructible<T>::value, "");
    static_assert( std::is_nothrow_default_constructible<const T>::value, "");
    static_assert( std::is_nothrow_default_constructible<volatile T>::value, "");
    static_assert( std::is_nothrow_default_constructible<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_nothrow_default_constructible_v<T>, "");
    static_assert( std::is_nothrow_default_constructible_v<const T>, "");
    static_assert( std::is_nothrow_default_constructible_v<volatile T>, "");
    static_assert( std::is_nothrow_default_constructible_v<const volatile T>, "");
#endif
}

template <class T>
void test_has_not_nothrow_default_constructor()
{
    static_assert(!std::is_nothrow_default_constructible<T>::value, "");
    static_assert(!std::is_nothrow_default_constructible<const T>::value, "");
    static_assert(!std::is_nothrow_default_constructible<volatile T>::value, "");
    static_assert(!std::is_nothrow_default_constructible<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_nothrow_default_constructible_v<T>, "");
    static_assert(!std::is_nothrow_default_constructible_v<const T>, "");
    static_assert(!std::is_nothrow_default_constructible_v<volatile T>, "");
    static_assert(!std::is_nothrow_default_constructible_v<const volatile T>, "");
#endif
}

#if TEST_STD_VER >= 11
struct DThrows
{
    DThrows()  noexcept(true) {}
    ~DThrows() noexcept(false) {}
};
#endif

int tc_utilities_meta_meta_unary_meta_unary_prop_is_nothrow_default_constructible(void) {
    test_has_not_nothrow_default_constructor<void>();
    test_has_not_nothrow_default_constructor<int&>();
    test_has_not_nothrow_default_constructor<A>();
#if TEST_STD_VER >= 11
    test_has_not_nothrow_default_constructor<DThrows>(); // This is LWG2116
// TODO: enable the test for GCC once https://gcc.gnu.org/bugzilla/show_bug.cgi?id=106611 is resolved
#ifndef TEST_COMPILER_GCC
    test_has_not_nothrow_default_constructor<TrivialNotNoexcept>();
#endif
#endif

    test_is_nothrow_default_constructible<Union>();
    test_is_nothrow_default_constructible<Empty>();
    test_is_nothrow_default_constructible<int>();
    test_is_nothrow_default_constructible<double>();
    test_is_nothrow_default_constructible<int*>();
    test_is_nothrow_default_constructible<const int*>();
    test_is_nothrow_default_constructible<char[3]>();
    test_is_nothrow_default_constructible<bit_zero>();

  return 0;
}
