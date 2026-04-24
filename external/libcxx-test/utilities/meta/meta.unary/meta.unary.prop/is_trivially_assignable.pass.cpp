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

// is_trivially_assignable

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, class U>
void test_is_trivially_assignable()
{
    static_assert(( std::is_trivially_assignable<T, U>::value), "");
#if TEST_STD_VER > 14
    static_assert(( std::is_trivially_assignable_v<T, U>), "");
#endif
}

template <class T, class U>
void test_is_not_trivially_assignable()
{
    static_assert((!std::is_trivially_assignable<T, U>::value), "");
#if TEST_STD_VER > 14
    static_assert((!std::is_trivially_assignable_v<T, U>), "");
#endif
}

struct A
{
};

struct B
{
    void operator=(A);
};

struct C
{
    void operator=(C&);  // not const
};

int tc_utilities_meta_meta_unary_meta_unary_prop_is_trivially_assignable(void) {
    test_is_trivially_assignable<int&, int&> ();
    test_is_trivially_assignable<int&, int> ();
    test_is_trivially_assignable<int&, double> ();

    test_is_not_trivially_assignable<int, int&> ();
    test_is_not_trivially_assignable<int, int> ();
    test_is_not_trivially_assignable<B, A> ();
    test_is_not_trivially_assignable<A, B> ();
    test_is_not_trivially_assignable<C&, C&> ();

  return 0;
}
