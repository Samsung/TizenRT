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

// is_assignable

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A
{
};

struct B
{
    void operator=(A);
};

template <class T, class U>
void test_is_assignable()
{
    static_assert(( std::is_assignable<T, U>::value), "");
#if TEST_STD_VER > 14
    static_assert(  std::is_assignable_v<T, U>, "");
#endif
}

template <class T, class U>
void test_is_not_assignable()
{
    static_assert((!std::is_assignable<T, U>::value), "");
#if TEST_STD_VER > 14
    static_assert( !std::is_assignable_v<T, U>, "");
#endif
}

struct D;

#if TEST_STD_VER >= 11
struct C
{
    template <class U>
    D operator,(U&&);
};

struct E
{
    C operator=(int);
};
#endif

template <typename T>
struct X { T t; };

int tc_utilities_meta_meta_unary_meta_unary_prop_is_assignable(void) {
    test_is_assignable<int&, int&> ();
    test_is_assignable<int&, int> ();
    test_is_assignable<int&, double> ();
    test_is_assignable<B, A> ();
    test_is_assignable<void*&, void*> ();

#if TEST_STD_VER >= 11
    test_is_assignable<E, int> ();

    test_is_not_assignable<int, int&> ();
    test_is_not_assignable<int, int> ();
#endif
    test_is_not_assignable<A, B> ();
    test_is_not_assignable<void, const void> ();
    test_is_not_assignable<const void, const void> ();
    test_is_not_assignable<int(), int> ();

//  pointer to incomplete template type
    test_is_assignable<X<D>*&, X<D>*> ();

  return 0;
}
