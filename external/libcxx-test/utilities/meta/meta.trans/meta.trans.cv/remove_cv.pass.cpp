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

// remove_cv

#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, class U>
void test_remove_cv_imp()
{
    ASSERT_SAME_TYPE(U, typename std::remove_cv<T>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(U,        std::remove_cv_t<T>);
#endif
}

template <class T>
void test_remove_cv()
{
    test_remove_cv_imp<T, T>();
    test_remove_cv_imp<const T, T>();
    test_remove_cv_imp<volatile T, T>();
    test_remove_cv_imp<const volatile T, T>();
}

int tc_utilities_meta_meta_trans_meta_trans_cv_remove_cv(void) {
    test_remove_cv<void>();
    test_remove_cv<int>();
    test_remove_cv<int[3]>();
    test_remove_cv<int&>();
    test_remove_cv<const int&>();
    test_remove_cv<int*>();
    test_remove_cv<const int*>();

  return 0;
}
