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

// is_fundamental

#include <type_traits>
#include <cstddef>         // for std::nullptr_t
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_is_fundamental()
{
    static_assert( std::is_fundamental<T>::value, "");
    static_assert( std::is_fundamental<const T>::value, "");
    static_assert( std::is_fundamental<volatile T>::value, "");
    static_assert( std::is_fundamental<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_fundamental_v<T>, "");
    static_assert( std::is_fundamental_v<const T>, "");
    static_assert( std::is_fundamental_v<volatile T>, "");
    static_assert( std::is_fundamental_v<const volatile T>, "");
#endif
}

template <class T>
void test_is_not_fundamental()
{
    static_assert(!std::is_fundamental<T>::value, "");
    static_assert(!std::is_fundamental<const T>::value, "");
    static_assert(!std::is_fundamental<volatile T>::value, "");
    static_assert(!std::is_fundamental<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_fundamental_v<T>, "");
    static_assert(!std::is_fundamental_v<const T>, "");
    static_assert(!std::is_fundamental_v<volatile T>, "");
    static_assert(!std::is_fundamental_v<const volatile T>, "");
#endif
}

class incomplete_type;

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

typedef void (*FunctionPtr)();


int tc_utilities_meta_meta_unary_meta_unary_comp_is_fundamental(void) {
    test_is_fundamental<std::nullptr_t>();
    test_is_fundamental<void>();
    test_is_fundamental<short>();
    test_is_fundamental<unsigned short>();
    test_is_fundamental<int>();
    test_is_fundamental<unsigned int>();
    test_is_fundamental<long>();
    test_is_fundamental<unsigned long>();
    test_is_fundamental<long long>();
    test_is_fundamental<unsigned long long>();
    test_is_fundamental<bool>();
    test_is_fundamental<char>();
    test_is_fundamental<signed char>();
    test_is_fundamental<unsigned char>();
    test_is_fundamental<wchar_t>();
    test_is_fundamental<double>();
    test_is_fundamental<float>();
    test_is_fundamental<double>();
    test_is_fundamental<long double>();
    test_is_fundamental<char16_t>();
    test_is_fundamental<char32_t>();

    test_is_not_fundamental<char[3]>();
    test_is_not_fundamental<char[]>();
    test_is_not_fundamental<void *>();
    test_is_not_fundamental<FunctionPtr>();
    test_is_not_fundamental<int&>();
    test_is_not_fundamental<int&&>();
    test_is_not_fundamental<Union>();
    test_is_not_fundamental<Empty>();
    test_is_not_fundamental<incomplete_type>();
    test_is_not_fundamental<bit_zero>();
    test_is_not_fundamental<int*>();
    test_is_not_fundamental<const int*>();
    test_is_not_fundamental<Enum>();
    test_is_not_fundamental<NotEmpty>();
    test_is_not_fundamental<Abstract>();

  return 0;
}
