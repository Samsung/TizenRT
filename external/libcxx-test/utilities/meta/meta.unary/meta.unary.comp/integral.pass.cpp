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

// integral

#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_integral_imp()
{
    static_assert(!std::is_reference<T>::value, "");
    static_assert( std::is_arithmetic<T>::value, "");
    static_assert( std::is_fundamental<T>::value, "");
    static_assert( std::is_object<T>::value, "");
    static_assert( std::is_scalar<T>::value, "");
    static_assert(!std::is_compound<T>::value, "");
    static_assert(!std::is_member_pointer<T>::value, "");
}

template <class T>
void test_integral()
{
    test_integral_imp<T>();
    test_integral_imp<const T>();
    test_integral_imp<volatile T>();
    test_integral_imp<const volatile T>();
}

int tc_utilities_meta_meta_unary_meta_unary_comp_integral(void) {
    test_integral<bool>();
    test_integral<char>();
    test_integral<signed char>();
    test_integral<unsigned char>();
    test_integral<wchar_t>();
    test_integral<short>();
    test_integral<unsigned short>();
    test_integral<int>();
    test_integral<unsigned int>();
    test_integral<long>();
    test_integral<unsigned long>();
    test_integral<long long>();
    test_integral<unsigned long long>();
#ifndef TEST_HAS_NO_INT128
    test_integral<__int128_t>();
    test_integral<__uint128_t>();
#endif

  return 0;
}
