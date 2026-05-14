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

// is_standard_layout

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_is_standard_layout()
{
    static_assert( std::is_standard_layout<T>::value, "");
    static_assert( std::is_standard_layout<const T>::value, "");
    static_assert( std::is_standard_layout<volatile T>::value, "");
    static_assert( std::is_standard_layout<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_standard_layout_v<T>, "");
    static_assert( std::is_standard_layout_v<const T>, "");
    static_assert( std::is_standard_layout_v<volatile T>, "");
    static_assert( std::is_standard_layout_v<const volatile T>, "");
#endif
}

template <class T>
void test_is_not_standard_layout()
{
    static_assert(!std::is_standard_layout<T>::value, "");
    static_assert(!std::is_standard_layout<const T>::value, "");
    static_assert(!std::is_standard_layout<volatile T>::value, "");
    static_assert(!std::is_standard_layout<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_standard_layout_v<T>, "");
    static_assert(!std::is_standard_layout_v<const T>, "");
    static_assert(!std::is_standard_layout_v<volatile T>, "");
    static_assert(!std::is_standard_layout_v<const volatile T>, "");
#endif
}

template <class T1, class T2>
struct pair
{
    T1 first;
    T2 second;
};

int tc_utilities_meta_meta_unary_meta_unary_prop_is_standard_layout(void) {
    test_is_standard_layout<int> ();
    test_is_standard_layout<int[3]> ();
    test_is_standard_layout<pair<int, double> > ();

    test_is_not_standard_layout<int&> ();

  return 0;
}
