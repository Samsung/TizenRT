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

// has_nothrow_move_assign

#include <type_traits>
#include "test_macros.h"

#include "common.h"
#include "libcxx_tc_common.h"

template <class T>
void test_has_nothrow_assign()
{
    static_assert( std::is_nothrow_move_assignable<T>::value, "");
#if TEST_STD_VER > 14
    static_assert( std::is_nothrow_move_assignable_v<T>, "");
#endif
}

template <class T>
void test_has_not_nothrow_assign()
{
    static_assert(!std::is_nothrow_move_assignable<T>::value, "");
#if TEST_STD_VER > 14
    static_assert(!std::is_nothrow_move_assignable_v<T>, "");
#endif
}

int tc_utilities_meta_meta_unary_meta_unary_prop_is_nothrow_move_assignable(void) {
    test_has_nothrow_assign<int&>();
    test_has_nothrow_assign<Union>();
    test_has_nothrow_assign<Empty>();
    test_has_nothrow_assign<int>();
    test_has_nothrow_assign<double>();
    test_has_nothrow_assign<int*>();
    test_has_nothrow_assign<const int*>();
    test_has_nothrow_assign<NotEmpty>();
    test_has_nothrow_assign<bit_zero>();

    test_has_not_nothrow_assign<void>();
    test_has_not_nothrow_assign<A>();
// TODO: enable the test for GCC once https://gcc.gnu.org/bugzilla/show_bug.cgi?id=106611 is resolved
#if TEST_STD_VER >= 11 && !defined(TEST_COMPILER_GCC)
    test_has_not_nothrow_assign<TrivialNotNoexcept>();
#endif

  return 0;
}
