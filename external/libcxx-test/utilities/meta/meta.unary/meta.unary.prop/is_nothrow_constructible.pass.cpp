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

// template <class T, class... Args>
//   struct is_nothrow_constructible;

#include <type_traits>
#include "test_macros.h"

#include "common.h"
#include "libcxx_tc_common.h"

template <class T>
void test_is_nothrow_constructible()
{
    static_assert(( std::is_nothrow_constructible<T>::value), "");
#if TEST_STD_VER > 14
    static_assert(( std::is_nothrow_constructible_v<T>), "");
#endif
}

template <class T, class A0>
void test_is_nothrow_constructible()
{
    static_assert(( std::is_nothrow_constructible<T, A0>::value), "");
#if TEST_STD_VER > 14
    static_assert(( std::is_nothrow_constructible_v<T, A0>), "");
#endif
}

template <class T>
void test_is_not_nothrow_constructible()
{
    static_assert((!std::is_nothrow_constructible<T>::value), "");
#if TEST_STD_VER > 14
    static_assert((!std::is_nothrow_constructible_v<T>), "");
#endif
}

template <class T, class A0>
void test_is_not_nothrow_constructible()
{
    static_assert((!std::is_nothrow_constructible<T, A0>::value), "");
#if TEST_STD_VER > 14
    static_assert((!std::is_nothrow_constructible_v<T, A0>), "");
#endif
}

template <class T, class A0, class A1>
void test_is_not_nothrow_constructible()
{
    static_assert((!std::is_nothrow_constructible<T, A0, A1>::value), "");
#if TEST_STD_VER > 14
    static_assert((!std::is_nothrow_constructible_v<T, A0, A1>), "");
#endif
}

struct C
{
    C(C&);  // not const
    void operator=(C&);  // not const
};

#if TEST_STD_VER >= 11
struct Tuple {
    Tuple(Empty&&) noexcept {}
};
#endif

int tc_utilities_meta_meta_unary_meta_unary_prop_is_nothrow_constructible(void) {
    test_is_nothrow_constructible<int> ();
    test_is_nothrow_constructible<int, const int&> ();
    test_is_nothrow_constructible<Empty> ();
    test_is_nothrow_constructible<Empty, const Empty&> ();

    test_is_not_nothrow_constructible<A, int> ();
    test_is_not_nothrow_constructible<A, int, double> ();
    test_is_not_nothrow_constructible<A> ();
    test_is_not_nothrow_constructible<C> ();
#if TEST_STD_VER >= 11
    test_is_nothrow_constructible<Tuple &&, Empty> (); // See bug #19616.

    static_assert(!std::is_constructible<Tuple&, Empty>::value, "");
    test_is_not_nothrow_constructible<Tuple &, Empty> (); // See bug #19616.
#endif

  return 0;
}
