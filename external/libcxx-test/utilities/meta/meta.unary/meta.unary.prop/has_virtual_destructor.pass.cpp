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

// has_virtual_destructor

#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_has_virtual_destructor()
{
    static_assert( std::has_virtual_destructor<T>::value, "");
    static_assert( std::has_virtual_destructor<const T>::value, "");
    static_assert( std::has_virtual_destructor<volatile T>::value, "");
    static_assert( std::has_virtual_destructor<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::has_virtual_destructor_v<T>, "");
    static_assert( std::has_virtual_destructor_v<const T>, "");
    static_assert( std::has_virtual_destructor_v<volatile T>, "");
    static_assert( std::has_virtual_destructor_v<const volatile T>, "");
#endif
}

template <class T>
void test_has_not_virtual_destructor()
{
    static_assert(!std::has_virtual_destructor<T>::value, "");
    static_assert(!std::has_virtual_destructor<const T>::value, "");
    static_assert(!std::has_virtual_destructor<volatile T>::value, "");
    static_assert(!std::has_virtual_destructor<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::has_virtual_destructor_v<T>, "");
    static_assert(!std::has_virtual_destructor_v<const T>, "");
    static_assert(!std::has_virtual_destructor_v<volatile T>, "");
    static_assert(!std::has_virtual_destructor_v<const volatile T>, "");
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

struct A
{
    ~A();
};

int tc_utilities_meta_meta_unary_meta_unary_prop_has_virtual_destructor(void) {
    test_has_not_virtual_destructor<void>();
    test_has_not_virtual_destructor<A>();
    test_has_not_virtual_destructor<int&>();
    test_has_not_virtual_destructor<Union>();
    test_has_not_virtual_destructor<Empty>();
    test_has_not_virtual_destructor<int>();
    test_has_not_virtual_destructor<double>();
    test_has_not_virtual_destructor<int*>();
    test_has_not_virtual_destructor<const int*>();
    test_has_not_virtual_destructor<char[3]>();
    test_has_not_virtual_destructor<char[]>();
    test_has_not_virtual_destructor<bit_zero>();

    test_has_virtual_destructor<Abstract>();
    test_has_virtual_destructor<NotEmpty>();

  return 0;
}
