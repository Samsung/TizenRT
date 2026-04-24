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

// is_null_pointer

// UNSUPPORTED: c++03, c++11

#include <type_traits>
#include <cstddef>        // for std::nullptr_t
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_is_null_pointer()
{
    static_assert( std::is_null_pointer<T>::value, "");
    static_assert( std::is_null_pointer<const T>::value, "");
    static_assert( std::is_null_pointer<volatile T>::value, "");
    static_assert( std::is_null_pointer<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_null_pointer_v<T>, "");
    static_assert( std::is_null_pointer_v<const T>, "");
    static_assert( std::is_null_pointer_v<volatile T>, "");
    static_assert( std::is_null_pointer_v<const volatile T>, "");
#endif
}

template <class T>
void test_is_not_null_pointer()
{
    static_assert(!std::is_null_pointer<T>::value, "");
    static_assert(!std::is_null_pointer<const T>::value, "");
    static_assert(!std::is_null_pointer<volatile T>::value, "");
    static_assert(!std::is_null_pointer<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_null_pointer_v<T>, "");
    static_assert(!std::is_null_pointer_v<const T>, "");
    static_assert(!std::is_null_pointer_v<volatile T>, "");
    static_assert(!std::is_null_pointer_v<const volatile T>, "");
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

int tc_utilities_meta_meta_unary_meta_unary_cat_is_null_pointer(void) {
    test_is_null_pointer<std::nullptr_t>();

    test_is_not_null_pointer<void>();
    test_is_not_null_pointer<int>();
    test_is_not_null_pointer<int&>();
    test_is_not_null_pointer<int&&>();
    test_is_not_null_pointer<int*>();
    test_is_not_null_pointer<double>();
    test_is_not_null_pointer<const int*>();
    test_is_not_null_pointer<char[3]>();
    test_is_not_null_pointer<char[]>();
    test_is_not_null_pointer<Union>();
    test_is_not_null_pointer<Enum>();
    test_is_not_null_pointer<FunctionPtr>();
    test_is_not_null_pointer<Empty>();
    test_is_not_null_pointer<bit_zero>();
    test_is_not_null_pointer<NotEmpty>();
    test_is_not_null_pointer<Abstract>();
    test_is_not_null_pointer<incomplete_type>();

  return 0;
}
