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

// is_function

#include <type_traits>
#include <cstddef>        // for std::nullptr_t

#include "test_macros.h"
#include "libcxx_tc_common.h"

// NOTE: On Windows the function `test_is_function<void()>` and
// `test_is_function<void() noexcept> has the same mangled despite being
// a distinct instantiation. This causes Clang to emit an error. However
// structs do not have this problem.

template <class T>
struct test_is_function {
    static_assert( std::is_function<T>::value, "");
    static_assert( std::is_function<const T>::value, "");
    static_assert( std::is_function<volatile T>::value, "");
    static_assert( std::is_function<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_function_v<T>, "");
    static_assert( std::is_function_v<const T>, "");
    static_assert( std::is_function_v<volatile T>, "");
    static_assert( std::is_function_v<const volatile T>, "");
#endif
};

template <class T>
struct test_is_not_function {
    static_assert(!std::is_function<T>::value, "");
    static_assert(!std::is_function<const T>::value, "");
    static_assert(!std::is_function<volatile T>::value, "");
    static_assert(!std::is_function<const volatile T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_function_v<T>, "");
    static_assert(!std::is_function_v<const T>, "");
    static_assert(!std::is_function_v<volatile T>, "");
    static_assert(!std::is_function_v<const volatile T>, "");
#endif
};

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

int tc_utilities_meta_meta_unary_meta_unary_cat_is_function(void) {
    test_is_function<void(void)>();
    test_is_function<int(int)>();
    test_is_function<int(int, double)>();
    test_is_function<int(Abstract *)>();
    test_is_function<void(...)>();

  test_is_not_function<std::nullptr_t>();
  test_is_not_function<void>();
  test_is_not_function<int>();
  test_is_not_function<int&>();
  test_is_not_function<int&&>();
  test_is_not_function<int*>();
  test_is_not_function<double>();
  test_is_not_function<char[3]>();
  test_is_not_function<char[]>();
  test_is_not_function<Union>();
  test_is_not_function<Enum>();
  test_is_not_function<FunctionPtr>(); // function pointer is not a function
  test_is_not_function<Empty>();
  test_is_not_function<bit_zero>();
  test_is_not_function<NotEmpty>();
  test_is_not_function<Abstract>();
  test_is_not_function<Abstract*>();
  test_is_not_function<incomplete_type>();

#if TEST_STD_VER >= 11
  test_is_function<void() noexcept>();
  test_is_function<void() const && noexcept>();
#endif

  return 0;
}
