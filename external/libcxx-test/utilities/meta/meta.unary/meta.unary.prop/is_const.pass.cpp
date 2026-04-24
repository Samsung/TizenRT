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

// is_const

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_is_const()
{
    static_assert(!std::is_const<T>::value, "");
    static_assert( std::is_const<const T>::value, "");
    static_assert(!std::is_const<volatile T>::value, "");
    static_assert( std::is_const<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_const_v<T>, "");
    static_assert( std::is_const_v<const T>, "");
    static_assert(!std::is_const_v<volatile T>, "");
    static_assert( std::is_const_v<const volatile T>, "");
#endif
}

struct A; // incomplete

int tc_utilities_meta_meta_unary_meta_unary_prop_is_const(void) {
    test_is_const<void>();
    test_is_const<int>();
    test_is_const<double>();
    test_is_const<int*>();
    test_is_const<const int*>();
    test_is_const<char[3]>();
    test_is_const<char[]>();

    test_is_const<A>();

    static_assert(!std::is_const<int&>::value, "");
    static_assert(!std::is_const<const int&>::value, "");

  return 0;
}
