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

// nullptr_t
//  is_null_pointer

// UNSUPPORTED: c++03, c++11

#include <type_traits>
#include <cstddef>        // for std::nullptr_t

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_nullptr_imp()
{
    static_assert(!std::is_void<T>::value, "");
    static_assert( std::is_null_pointer<T>::value, "");
    static_assert(!std::is_integral<T>::value, "");
    static_assert(!std::is_floating_point<T>::value, "");
    static_assert(!std::is_array<T>::value, "");
    static_assert(!std::is_pointer<T>::value, "");
    static_assert(!std::is_lvalue_reference<T>::value, "");
    static_assert(!std::is_rvalue_reference<T>::value, "");
    static_assert(!std::is_member_object_pointer<T>::value, "");
    static_assert(!std::is_member_function_pointer<T>::value, "");
    static_assert(!std::is_enum<T>::value, "");
    static_assert(!std::is_union<T>::value, "");
    static_assert(!std::is_class<T>::value, "");
    static_assert(!std::is_function<T>::value, "");
}

template <class T>
void test_nullptr()
{
    test_nullptr_imp<T>();
    test_nullptr_imp<const T>();
    test_nullptr_imp<volatile T>();
    test_nullptr_imp<const volatile T>();
}

struct incomplete_type;

int tc_utilities_meta_meta_unary_meta_unary_cat_nullptr(void) {
    test_nullptr<std::nullptr_t>();

//  LWG#2582
    static_assert(!std::is_null_pointer<incomplete_type>::value, "");
    return 0;
}
