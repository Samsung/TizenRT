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
// UNSUPPORTED: c++03, c++11
// type_traits

// is_final

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

struct P final { };
union U1 { };
union U2 final { };

template <class T>
void test_is_final()
{
    static_assert( std::is_final<T>::value, "");
    static_assert( std::is_final<const T>::value, "");
    static_assert( std::is_final<volatile T>::value, "");
    static_assert( std::is_final<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_final_v<T>, "");
    static_assert( std::is_final_v<const T>, "");
    static_assert( std::is_final_v<volatile T>, "");
    static_assert( std::is_final_v<const volatile T>, "");
#endif
}

template <class T>
void test_is_not_final()
{
    static_assert(!std::is_final<T>::value, "");
    static_assert(!std::is_final<const T>::value, "");
    static_assert(!std::is_final<volatile T>::value, "");
    static_assert(!std::is_final<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_final_v<T>, "");
    static_assert(!std::is_final_v<const T>, "");
    static_assert(!std::is_final_v<volatile T>, "");
    static_assert(!std::is_final_v<const volatile T>, "");
#endif
}

int tc_utilities_meta_meta_unary_meta_unary_prop_is_final(void) {
    test_is_not_final<int>();
    test_is_not_final<int*>();
    test_is_final    <P>();
    test_is_not_final<P*>();
    test_is_not_final<U1>();
    test_is_not_final<U1*>();
    test_is_final    <U2>();
    test_is_not_final<U2*>();

  return 0;
}
