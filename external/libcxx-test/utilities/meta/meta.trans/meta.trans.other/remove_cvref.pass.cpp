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

// remove_cvref

#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, class U>
void test_remove_cvref()
{
    ASSERT_SAME_TYPE(U, typename std::remove_cvref<T>::type);
    ASSERT_SAME_TYPE(U,        std::remove_cvref_t<T>);
}

int tc_utilities_meta_meta_trans_meta_trans_other_remove_cvref(void) {
    test_remove_cvref<void, void>();
    test_remove_cvref<int, int>();
    test_remove_cvref<const int, int>();
    test_remove_cvref<const volatile int, int>();
    test_remove_cvref<volatile int, int>();

// Doesn't decay
    test_remove_cvref<int[3],                 int[3]>();
    test_remove_cvref<int const [3],          int[3]>();
    test_remove_cvref<int volatile [3],       int[3]>();
    test_remove_cvref<int const volatile [3], int[3]>();
    test_remove_cvref<void(), void ()>();

    test_remove_cvref<int &, int>();
    test_remove_cvref<const int &, int>();
    test_remove_cvref<const volatile int &, int>();
    test_remove_cvref<volatile int &, int>();

    test_remove_cvref<int*, int*>();
    test_remove_cvref<int(int) const, int(int) const>();
    test_remove_cvref<int(int) volatile, int(int) volatile>();
    test_remove_cvref<int(int)  &, int(int)  &>();
    test_remove_cvref<int(int) &&, int(int) &&>();

  return 0;
}
