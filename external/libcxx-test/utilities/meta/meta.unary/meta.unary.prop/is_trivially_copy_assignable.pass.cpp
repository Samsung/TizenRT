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

// is_trivially_copy_assignable

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_has_trivially_copy_assignable()
{
    static_assert( std::is_trivially_copy_assignable<T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_trivially_copy_assignable_v<T>, "");
#endif
}

template <class T>
void test_has_not_trivially_copy_assignable()
{
    static_assert(!std::is_trivially_copy_assignable<T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_trivially_copy_assignable_v<T>, "");
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
    A& operator=(const A&);
};

int tc_utilities_meta_meta_unary_meta_unary_prop_is_trivially_copy_assignable(void) {
    test_has_trivially_copy_assignable<int&>();
    test_has_trivially_copy_assignable<Union>();
    test_has_trivially_copy_assignable<Empty>();
    test_has_trivially_copy_assignable<int>();
    test_has_trivially_copy_assignable<double>();
    test_has_trivially_copy_assignable<int*>();
    test_has_trivially_copy_assignable<const int*>();
    test_has_trivially_copy_assignable<bit_zero>();

    test_has_not_trivially_copy_assignable<void>();
    test_has_not_trivially_copy_assignable<A>();
    test_has_not_trivially_copy_assignable<NotEmpty>();
    test_has_not_trivially_copy_assignable<Abstract>();
    test_has_not_trivially_copy_assignable<const Empty>();


  return 0;
}
