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

// has_nothrow_move_constructor

#include <type_traits>
#include "test_macros.h"

#include "common.h"
#include "libcxx_tc_common.h"

template <class T>
void test_is_nothrow_move_constructible()
{
    static_assert( std::is_nothrow_move_constructible<T>::value, "");
    static_assert( std::is_nothrow_move_constructible<const T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_nothrow_move_constructible_v<T>, "");
    static_assert( std::is_nothrow_move_constructible_v<const T>, "");
#endif
}

template <class T>
void test_has_not_nothrow_move_constructor()
{
    static_assert(!std::is_nothrow_move_constructible<T>::value, "");
    static_assert(!std::is_nothrow_move_constructible<const T>::value, "");
    static_assert(!std::is_nothrow_move_constructible<volatile T>::value, "");
    static_assert(!std::is_nothrow_move_constructible<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_nothrow_move_constructible_v<T>, "");
    static_assert(!std::is_nothrow_move_constructible_v<const T>, "");
    static_assert(!std::is_nothrow_move_constructible_v<volatile T>, "");
    static_assert(!std::is_nothrow_move_constructible_v<const volatile T>, "");
#endif
}

int tc_utilities_meta_meta_unary_meta_unary_prop_is_nothrow_move_constructible(void) {
    test_has_not_nothrow_move_constructor<void>();
    test_has_not_nothrow_move_constructor<A>();
// TODO: enable the test for GCC once https://gcc.gnu.org/bugzilla/show_bug.cgi?id=106611 is resolved
#if TEST_STD_VER >= 11 && !defined(TEST_COMPILER_GCC)
    test_has_not_nothrow_move_constructor<TrivialNotNoexcept>();
#endif

    test_is_nothrow_move_constructible<int&>();
    test_is_nothrow_move_constructible<Union>();
    test_is_nothrow_move_constructible<Empty>();
    test_is_nothrow_move_constructible<int>();
    test_is_nothrow_move_constructible<double>();
    test_is_nothrow_move_constructible<int*>();
    test_is_nothrow_move_constructible<const int*>();
    test_is_nothrow_move_constructible<bit_zero>();

  return 0;
}
