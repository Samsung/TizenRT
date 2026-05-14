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

// is_compound

#include <type_traits>
#include <cstddef>         // for std::nullptr_t
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_is_compound()
{
    static_assert( std::is_compound<T>::value, "");
    static_assert( std::is_compound<const T>::value, "");
    static_assert( std::is_compound<volatile T>::value, "");
    static_assert( std::is_compound<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_compound_v<T>, "");
    static_assert( std::is_compound_v<const T>, "");
    static_assert( std::is_compound_v<volatile T>, "");
    static_assert( std::is_compound_v<const volatile T>, "");
#endif
}

template <class T>
void test_is_not_compound()
{
    static_assert(!std::is_compound<T>::value, "");
    static_assert(!std::is_compound<const T>::value, "");
    static_assert(!std::is_compound<volatile T>::value, "");
    static_assert(!std::is_compound<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_compound_v<T>, "");
    static_assert(!std::is_compound_v<const T>, "");
    static_assert(!std::is_compound_v<volatile T>, "");
    static_assert(!std::is_compound_v<const volatile T>, "");
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


int tc_utilities_meta_meta_unary_meta_unary_comp_is_compound(void) {
    test_is_compound<char[3]>();
    test_is_compound<char[]>();
    test_is_compound<void *>();
    test_is_compound<FunctionPtr>();
    test_is_compound<int&>();
    test_is_compound<int&&>();
    test_is_compound<Union>();
    test_is_compound<Empty>();
    test_is_compound<incomplete_type>();
    test_is_compound<bit_zero>();
    test_is_compound<int*>();
    test_is_compound<const int*>();
    test_is_compound<Enum>();
    test_is_compound<NotEmpty>();
    test_is_compound<Abstract>();

    test_is_not_compound<std::nullptr_t>();
    test_is_not_compound<void>();
    test_is_not_compound<int>();
    test_is_not_compound<double>();

  return 0;
}
