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

// is_abstract

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test_is_abstract()
{
    static_assert( std::is_abstract<T>::value, "");
    static_assert( std::is_abstract<const T>::value, "");
    static_assert( std::is_abstract<volatile T>::value, "");
    static_assert( std::is_abstract<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_abstract_v<T>, "");
    static_assert( std::is_abstract_v<const T>, "");
    static_assert( std::is_abstract_v<volatile T>, "");
    static_assert( std::is_abstract_v<const volatile T>, "");
#endif
}

template <class T>
void test_is_not_abstract()
{
    static_assert(!std::is_abstract<T>::value, "");
    static_assert(!std::is_abstract<const T>::value, "");
    static_assert(!std::is_abstract<volatile T>::value, "");
    static_assert(!std::is_abstract<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_abstract_v<T>, "");
    static_assert(!std::is_abstract_v<const T>, "");
    static_assert(!std::is_abstract_v<volatile T>, "");
    static_assert(!std::is_abstract_v<const volatile T>, "");
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

template <class>
struct AbstractTemplate {
  virtual void test() = 0;
};

template <>
struct AbstractTemplate<double> {};

int tc_utilities_meta_meta_unary_meta_unary_prop_is_abstract(void) {
    test_is_not_abstract<void>();
    test_is_not_abstract<int&>();
    test_is_not_abstract<int>();
    test_is_not_abstract<double>();
    test_is_not_abstract<int*>();
    test_is_not_abstract<const int*>();
    test_is_not_abstract<char[3]>();
    test_is_not_abstract<char[]>();
    test_is_not_abstract<Union>();
    test_is_not_abstract<Empty>();
    test_is_not_abstract<bit_zero>();
    test_is_not_abstract<NotEmpty>();

    test_is_abstract<Abstract>();
    test_is_abstract<AbstractTemplate<int> >();
    test_is_not_abstract<AbstractTemplate<double> >();

  return 0;
}
