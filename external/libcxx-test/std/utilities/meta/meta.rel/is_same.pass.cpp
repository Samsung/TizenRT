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

// is_same

#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, class U>
void test_is_same()
{
    static_assert(( std::is_same<T, U>::value), "");
    static_assert((!std::is_same<const T, U>::value), "");
    static_assert((!std::is_same<T, const U>::value), "");
    static_assert(( std::is_same<const T, const U>::value), "");
#if TEST_STD_VER > 14
    static_assert(( std::is_same_v<T, U>), "");
    static_assert((!std::is_same_v<const T, U>), "");
    static_assert((!std::is_same_v<T, const U>), "");
    static_assert(( std::is_same_v<const T, const U>), "");
#endif
}

template <class T, class U>
void test_is_same_ref()
{
    static_assert((std::is_same<T, U>::value), "");
    static_assert((std::is_same<const T, U>::value), "");
    static_assert((std::is_same<T, const U>::value), "");
    static_assert((std::is_same<const T, const U>::value), "");
#if TEST_STD_VER > 14
    static_assert((std::is_same_v<T, U>), "");
    static_assert((std::is_same_v<const T, U>), "");
    static_assert((std::is_same_v<T, const U>), "");
    static_assert((std::is_same_v<const T, const U>), "");
#endif
}

template <class T, class U>
void test_is_not_same()
{
    static_assert((!std::is_same<T, U>::value), "");
}

template <class T>
struct OverloadTest
{
    void fn(std::is_same<T, int>) { }
    void fn(std::false_type) { }
    void x() { fn(std::false_type()); }
};

class Class
{
public:
    ~Class();
};

int tc_utilities_meta_meta_rel_is_same(void) {
    test_is_same<int, int>();
    test_is_same<void, void>();
    test_is_same<Class, Class>();
    test_is_same<int*, int*>();
    test_is_same_ref<int&, int&>();

    test_is_not_same<int, void>();
    test_is_not_same<void, Class>();
    test_is_not_same<Class, int*>();
    test_is_not_same<int*, int&>();
    test_is_not_same<int&, int>();

    OverloadTest<char> t;
    (void)t;

  return 0;
}
