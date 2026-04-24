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

// is_integral

#include <type_traits>
#include <cstddef>        // for std::nullptr_t
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_is_integral()
{
    static_assert( std::is_integral<T>::value, "");
    static_assert( std::is_integral<const T>::value, "");
    static_assert( std::is_integral<volatile T>::value, "");
    static_assert( std::is_integral<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_integral_v<T>, "");
    static_assert( std::is_integral_v<const T>, "");
    static_assert( std::is_integral_v<volatile T>, "");
    static_assert( std::is_integral_v<const volatile T>, "");
#endif
}

template <class T>
void test_is_not_integral()
{
    static_assert(!std::is_integral<T>::value, "");
    static_assert(!std::is_integral<const T>::value, "");
    static_assert(!std::is_integral<volatile T>::value, "");
    static_assert(!std::is_integral<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_integral_v<T>, "");
    static_assert(!std::is_integral_v<const T>, "");
    static_assert(!std::is_integral_v<volatile T>, "");
    static_assert(!std::is_integral_v<const volatile T>, "");
#endif
}

class Empty
{
};

class NotEmpty
{
    virtual ~NotEmpty();
};

union Union {};

struct bit_zero
{
    int :  0;
};

class Abstract
{
    virtual ~Abstract() = 0;
};

enum Enum {zero, one};
struct incomplete_type;

typedef void (*FunctionPtr)();


int tc_utilities_meta_meta_unary_meta_unary_cat_is_integral(void) {
    test_is_integral<short>();
    test_is_integral<unsigned short>();
    test_is_integral<int>();
    test_is_integral<unsigned int>();
    test_is_integral<long>();
    test_is_integral<unsigned long>();
    test_is_integral<bool>();
    test_is_integral<char>();
    test_is_integral<signed char>();
    test_is_integral<unsigned char>();
    test_is_integral<wchar_t>();
#ifndef TEST_HAS_NO_CHAR8_T
    test_is_integral<char8_t>();
#endif

    test_is_not_integral<std::nullptr_t>();
    test_is_not_integral<void>();
    test_is_not_integral<int&>();
    test_is_not_integral<int&&>();
    test_is_not_integral<int*>();
    test_is_not_integral<double>();
    test_is_not_integral<const int*>();
    test_is_not_integral<char[3]>();
    test_is_not_integral<char[]>();
    test_is_not_integral<Union>();
    test_is_not_integral<Enum>();
    test_is_not_integral<FunctionPtr>();
    test_is_not_integral<Empty>();
    test_is_not_integral<bit_zero>();
    test_is_not_integral<NotEmpty>();
    test_is_not_integral<Abstract>();
    test_is_not_integral<incomplete_type>();

  return 0;
}
