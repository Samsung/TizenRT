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

// extent

#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, unsigned A>
void test_extent()
{
    static_assert((std::extent<T>::value == A), "");
    static_assert((std::extent<const T>::value == A), "");
    static_assert((std::extent<volatile T>::value == A), "");
    static_assert((std::extent<const volatile T>::value == A), "");
#if TEST_STD_VER > 14
    static_assert((std::extent_v<T> == A), "");
    static_assert((std::extent_v<const T> == A), "");
    static_assert((std::extent_v<volatile T> == A), "");
    static_assert((std::extent_v<const volatile T> == A), "");
#endif
}

template <class T, unsigned A>
void test_extent1()
{
    static_assert((std::extent<T, 1>::value == A), "");
    static_assert((std::extent<const T, 1>::value == A), "");
    static_assert((std::extent<volatile T, 1>::value == A), "");
    static_assert((std::extent<const volatile T, 1>::value == A), "");
#if TEST_STD_VER > 14
    static_assert((std::extent_v<T, 1> == A), "");
    static_assert((std::extent_v<const T, 1> == A), "");
    static_assert((std::extent_v<volatile T, 1> == A), "");
    static_assert((std::extent_v<const volatile T, 1> == A), "");
#endif
}

class Class
{
public:
    ~Class();
};

int tc_utilities_meta_meta_unary_prop_query_extent(void) {
    test_extent<void, 0>();
    test_extent<int&, 0>();
    test_extent<Class, 0>();
    test_extent<int*, 0>();
    test_extent<const int*, 0>();
    test_extent<int, 0>();
    test_extent<double, 0>();
    test_extent<bool, 0>();
    test_extent<unsigned, 0>();

    test_extent<int[2], 2>();
    test_extent<int[2][4], 2>();
    test_extent<int[][4], 0>();

    test_extent1<int, 0>();
    test_extent1<int[2], 0>();
    test_extent1<int[2][4], 4>();
    test_extent1<int[][4], 4>();

  return 0;
}
