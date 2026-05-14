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

// remove_extent

#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

enum Enum {zero, one_};

template <class T, class U>
void test_remove_extent()
{
    ASSERT_SAME_TYPE(U, typename std::remove_extent<T>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(U,        std::remove_extent_t<T>);
#endif
}


int tc_utilities_meta_meta_trans_meta_trans_arr_remove_extent(void) {
    test_remove_extent<int, int> ();
    test_remove_extent<const Enum, const Enum> ();
    test_remove_extent<int[], int> ();
    test_remove_extent<const int[], const int> ();
    test_remove_extent<int[3], int> ();
    test_remove_extent<const int[3], const int> ();
    test_remove_extent<int[][3], int[3]> ();
    test_remove_extent<const int[][3], const int[3]> ();
    test_remove_extent<int[2][3], int[3]> ();
    test_remove_extent<const int[2][3], const int[3]> ();
    test_remove_extent<int[1][2][3], int[2][3]> ();
    test_remove_extent<const int[1][2][3], const int[2][3]> ();

  return 0;
}
