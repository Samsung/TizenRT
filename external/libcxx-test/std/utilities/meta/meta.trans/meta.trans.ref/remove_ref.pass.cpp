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

// remove_reference

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, class U>
void test_remove_reference()
{
    ASSERT_SAME_TYPE(U, typename std::remove_reference<T>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(U, std::remove_reference_t<T>);
#endif
}

int tc_utilities_meta_meta_trans_meta_trans_ref_remove_ref(void) {
    test_remove_reference<void, void>();
    test_remove_reference<int, int>();
    test_remove_reference<int[3], int[3]>();
    test_remove_reference<int*, int*>();
    test_remove_reference<const int*, const int*>();

    test_remove_reference<int&, int>();
    test_remove_reference<const int&, const int>();
    test_remove_reference<int(&)[3], int[3]>();
    test_remove_reference<int*&, int*>();
    test_remove_reference<const int*&, const int*>();

    test_remove_reference<int&&, int>();
    test_remove_reference<const int&&, const int>();
    test_remove_reference<int(&&)[3], int[3]>();
    test_remove_reference<int*&&, int*>();
    test_remove_reference<const int*&&, const int*>();

  return 0;
}
