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
// UNSUPPORTED: c++03, c++11, c++14, c++17

// type_traits

// is_bounded_array<T>
// T is an array type of known bound ([dcl.array])

#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, bool B>
void test_array_imp()
{
    static_assert( B == std::is_bounded_array<T>::value, "" );
    static_assert( B == std::is_bounded_array_v<T>, "" );
}

template <class T, bool B>
void test_array()
{
    test_array_imp<T, B>();
    test_array_imp<const T, B>();
    test_array_imp<volatile T, B>();
    test_array_imp<const volatile T, B>();
}

class incomplete_type;

class Empty {};
union Union {};

class Abstract
{
    virtual ~Abstract() = 0;
};

enum Enum {zero, one};
typedef void (*FunctionPtr)();

int tc_utilities_meta_meta_unary_meta_unary_comp_is_bounded_array(void) {
    // Non-array types
    test_array<void,           false>();
    test_array<std::nullptr_t, false>();
    test_array<int,            false>();
    test_array<double,         false>();
    test_array<void *,         false>();
    test_array<int &,          false>();
    test_array<int &&,         false>();
    test_array<Empty,          false>();
    test_array<Union,          false>();
    test_array<Abstract,       false>();
    test_array<Enum,           false>();
    test_array<FunctionPtr,    false>();

    // Array types
    test_array<char[3],           true>();
    test_array<int[0],            false>();
    test_array<char[],            false>();
    test_array<incomplete_type[], false>();

  return 0;
}
