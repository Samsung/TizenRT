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

// void_t

// UNSUPPORTED: c++03, c++11, c++14

#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test1()
{
    ASSERT_SAME_TYPE(void, std::void_t<T>);
    ASSERT_SAME_TYPE(void, std::void_t<const T>);
    ASSERT_SAME_TYPE(void, std::void_t<volatile T>);
    ASSERT_SAME_TYPE(void, std::void_t<const volatile T>);
}

template <class T, class U>
void test2()
{
    ASSERT_SAME_TYPE(void, std::void_t<T, U>);
    ASSERT_SAME_TYPE(void, std::void_t<const T, U>);
    ASSERT_SAME_TYPE(void, std::void_t<volatile T, U>);
    ASSERT_SAME_TYPE(void, std::void_t<const volatile T, U>);

    ASSERT_SAME_TYPE(void, std::void_t<U, T>);
    ASSERT_SAME_TYPE(void, std::void_t<U, const T>);
    ASSERT_SAME_TYPE(void, std::void_t<U, volatile T>);
    ASSERT_SAME_TYPE(void, std::void_t<U, const volatile T>);

    ASSERT_SAME_TYPE(void, std::void_t<T, const U>);
    ASSERT_SAME_TYPE(void, std::void_t<const T, const U>);
    ASSERT_SAME_TYPE(void, std::void_t<volatile T, const U>);
    ASSERT_SAME_TYPE(void, std::void_t<const volatile T, const U>);
}

class Class
{
public:
    ~Class();
};

int tc_utilities_meta_meta_unary_prop_query_void_t(void) {
    ASSERT_SAME_TYPE(void, std::void_t<>);

    test1<void>();
    test1<int>();
    test1<double>();
    test1<int&>();
    test1<Class>();
    test1<Class[]>();
    test1<Class[5]>();

    test2<void, int>();
    test2<double, int>();
    test2<int&, int>();
    test2<Class&, bool>();
    test2<void *, int&>();

    ASSERT_SAME_TYPE(void, std::void_t<int, double const &, Class, volatile int[], void>);

  return 0;
}
