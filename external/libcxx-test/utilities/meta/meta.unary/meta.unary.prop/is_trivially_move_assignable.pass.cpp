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

// is_trivially_move_assignable

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_has_trivial_assign()
{
    static_assert( std::is_trivially_move_assignable<T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_trivially_move_assignable_v<T>, "");
#endif
}

template <class T>
void test_has_not_trivial_assign()
{
    static_assert(!std::is_trivially_move_assignable<T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_trivially_move_assignable_v<T>, "");
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

int tc_utilities_meta_meta_unary_meta_unary_prop_is_trivially_move_assignable(void) {
    test_has_trivial_assign<int&>();
    test_has_trivial_assign<Union>();
    test_has_trivial_assign<Empty>();
    test_has_trivial_assign<int>();
    test_has_trivial_assign<double>();
    test_has_trivial_assign<int*>();
    test_has_trivial_assign<const int*>();
    test_has_trivial_assign<bit_zero>();

    test_has_not_trivial_assign<void>();
    test_has_not_trivial_assign<A>();
    test_has_not_trivial_assign<NotEmpty>();
    test_has_not_trivial_assign<Abstract>();
    test_has_not_trivial_assign<const Empty>();


  return 0;
}
