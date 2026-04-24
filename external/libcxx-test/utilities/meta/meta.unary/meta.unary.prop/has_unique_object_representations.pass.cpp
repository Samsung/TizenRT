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
// UNSUPPORTED: c++03, c++11, c++14

// type_traits

// has_unique_object_representations

#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_has_unique_object_representations()
{
    static_assert( std::has_unique_object_representations<T>::value, "");
    static_assert( std::has_unique_object_representations<const T>::value, "");
    static_assert( std::has_unique_object_representations<volatile T>::value, "");
    static_assert( std::has_unique_object_representations<const volatile T>::value, "");

    static_assert( std::has_unique_object_representations_v<T>, "");
    static_assert( std::has_unique_object_representations_v<const T>, "");
    static_assert( std::has_unique_object_representations_v<volatile T>, "");
    static_assert( std::has_unique_object_representations_v<const volatile T>, "");
}

template <class T>
void test_has_not_has_unique_object_representations()
{
    static_assert(!std::has_unique_object_representations<T>::value, "");
    static_assert(!std::has_unique_object_representations<const T>::value, "");
    static_assert(!std::has_unique_object_representations<volatile T>::value, "");
    static_assert(!std::has_unique_object_representations<const volatile T>::value, "");

    static_assert(!std::has_unique_object_representations_v<T>, "");
    static_assert(!std::has_unique_object_representations_v<const T>, "");
    static_assert(!std::has_unique_object_representations_v<volatile T>, "");
    static_assert(!std::has_unique_object_representations_v<const volatile T>, "");
}

class Empty
{
};

class NotEmpty
{
    virtual ~NotEmpty();
};

union EmptyUnion {};
struct NonEmptyUnion {int x; unsigned y;};

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
    unsigned foo;
};

struct B
{
   char bar;
   int foo;
};


int tc_utilities_meta_meta_unary_meta_unary_prop_has_unique_object_representations(void) {
    test_has_not_has_unique_object_representations<void>();
    test_has_not_has_unique_object_representations<Empty>();
    test_has_not_has_unique_object_representations<EmptyUnion>();
    test_has_not_has_unique_object_representations<NotEmpty>();
    test_has_not_has_unique_object_representations<bit_zero>();
    test_has_not_has_unique_object_representations<Abstract>();
    test_has_not_has_unique_object_representations<B>();

//  I would expect all three of these to have unique representations.
//  I would also expect that there are systems where they do not.
//     test_has_not_has_unique_object_representations<int&>();
//     test_has_not_has_unique_object_representations<int *>();
//     test_has_not_has_unique_object_representations<double>();


    test_has_unique_object_representations<unsigned>();
    test_has_unique_object_representations<NonEmptyUnion>();
    test_has_unique_object_representations<char[3]>();
    test_has_unique_object_representations<char[]>();


  return 0;
}
