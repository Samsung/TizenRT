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

// is_trivially_copy_constructible

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_is_trivially_copy_constructible()
{
    static_assert( std::is_trivially_copy_constructible<T>::value, "");
    static_assert( std::is_trivially_copy_constructible<const T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_trivially_copy_constructible_v<T>, "");
    static_assert( std::is_trivially_copy_constructible_v<const T>, "");
#endif
}

template <class T>
void test_has_not_trivial_copy_constructor()
{
    static_assert(!std::is_trivially_copy_constructible<T>::value, "");
    static_assert(!std::is_trivially_copy_constructible<const T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_trivially_copy_constructible_v<T>, "");
    static_assert(!std::is_trivially_copy_constructible_v<const T>, "");
#endif
}

class Empty
{
};

class NotEmpty
{
public:
    virtual ~NotEmpty();
};

union Union {};

struct bit_zero
{
    int :  0;
};

class Abstract
{
public:
    virtual ~Abstract() = 0;
};

struct A
{
    A(const A&);
};

int tc_utilities_meta_meta_unary_meta_unary_prop_is_trivially_copy_constructible(void) {
    test_has_not_trivial_copy_constructor<void>();
    test_has_not_trivial_copy_constructor<A>();
    test_has_not_trivial_copy_constructor<Abstract>();
    test_has_not_trivial_copy_constructor<NotEmpty>();

    test_is_trivially_copy_constructible<int&>();
    test_is_trivially_copy_constructible<Union>();
    test_is_trivially_copy_constructible<Empty>();
    test_is_trivially_copy_constructible<int>();
    test_is_trivially_copy_constructible<double>();
    test_is_trivially_copy_constructible<int*>();
    test_is_trivially_copy_constructible<const int*>();
    test_is_trivially_copy_constructible<bit_zero>();

  return 0;
}
