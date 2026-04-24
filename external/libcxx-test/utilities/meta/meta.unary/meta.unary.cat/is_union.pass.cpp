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

// is_union

#include <type_traits>
#include <cstddef>        // for std::nullptr_t
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_is_union()
{
    static_assert( std::is_union<T>::value, "");
    static_assert( std::is_union<const T>::value, "");
    static_assert( std::is_union<volatile T>::value, "");
    static_assert( std::is_union<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_union_v<T>, "");
    static_assert( std::is_union_v<const T>, "");
    static_assert( std::is_union_v<volatile T>, "");
    static_assert( std::is_union_v<const volatile T>, "");
#endif
}

template <class T>
void test_is_not_union()
{
    static_assert(!std::is_union<T>::value, "");
    static_assert(!std::is_union<const T>::value, "");
    static_assert(!std::is_union<volatile T>::value, "");
    static_assert(!std::is_union<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_union_v<T>, "");
    static_assert(!std::is_union_v<const T>, "");
    static_assert(!std::is_union_v<volatile T>, "");
    static_assert(!std::is_union_v<const volatile T>, "");
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

int tc_utilities_meta_meta_unary_meta_unary_cat_is_union(void) {
    test_is_union<Union>();

    test_is_not_union<std::nullptr_t>();
    test_is_not_union<void>();
    test_is_not_union<int>();
    test_is_not_union<int&>();
    test_is_not_union<int&&>();
    test_is_not_union<int*>();
    test_is_not_union<double>();
    test_is_not_union<const int*>();
    test_is_not_union<char[3]>();
    test_is_not_union<char[]>();
    test_is_not_union<Enum>();
    test_is_not_union<FunctionPtr>();
    test_is_not_union<Empty>();
    test_is_not_union<bit_zero>();
    test_is_not_union<NotEmpty>();
    test_is_not_union<Abstract>();
    test_is_not_union<incomplete_type>();

  return 0;
}
