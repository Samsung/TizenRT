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

// is_volatile

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_is_volatile()
{
    static_assert(!std::is_volatile<T>::value, "");
    static_assert(!std::is_volatile<const T>::value, "");
    static_assert( std::is_volatile<volatile T>::value, "");
    static_assert( std::is_volatile<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_volatile_v<T>, "");
    static_assert(!std::is_volatile_v<const T>, "");
    static_assert( std::is_volatile_v<volatile T>, "");
    static_assert( std::is_volatile_v<const volatile T>, "");
#endif
}

struct A; // incomplete

int tc_utilities_meta_meta_unary_meta_unary_prop_is_volatile(void) {
    test_is_volatile<void>();
    test_is_volatile<int>();
    test_is_volatile<double>();
    test_is_volatile<int*>();
    test_is_volatile<const int*>();
    test_is_volatile<char[3]>();
    test_is_volatile<char[]>();

    test_is_volatile<A>();

    static_assert(!std::is_volatile<int&>::value, "");
    static_assert(!std::is_volatile<volatile int&>::value, "");

  return 0;
}
