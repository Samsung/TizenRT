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

// add_rvalue_reference
// If T names a referenceable type then the member typedef type
//   shall name T&&; otherwise, type shall name T.

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, class U>
void test_add_rvalue_reference()
{
    ASSERT_SAME_TYPE(U, typename std::add_rvalue_reference<T>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(U, std::add_rvalue_reference_t<T>);
#endif
}

template <class F>
void test_function0()
{
    ASSERT_SAME_TYPE(F&&, typename std::add_rvalue_reference<F>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(F&&, std::add_rvalue_reference_t<F>);
#endif
}

template <class F>
void test_function1()
{
    ASSERT_SAME_TYPE(F, typename std::add_rvalue_reference<F>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(F, std::add_rvalue_reference_t<F>);
#endif
}

struct Foo {};

int tc_utilities_meta_meta_trans_meta_trans_ref_add_rvalue_ref(void) {
    test_add_rvalue_reference<void, void>();
    test_add_rvalue_reference<int, int&&>();
    test_add_rvalue_reference<int[3], int(&&)[3]>();
    test_add_rvalue_reference<int&, int&>();
    test_add_rvalue_reference<const int&, const int&>();
    test_add_rvalue_reference<int*, int*&&>();
    test_add_rvalue_reference<const int*, const int*&&>();
    test_add_rvalue_reference<Foo, Foo&&>();

//  LWG 2101 specifically talks about add_rvalue_reference and functions.
//  The term of art is "a referenceable type", which a cv- or ref-qualified function is not.
    test_function0<void()>();
    test_function1<void() const>();
    test_function1<void() &>();
    test_function1<void() &&>();
    test_function1<void() const &>();
    test_function1<void() const &&>();

//  But a cv- or ref-qualified member function *is* "a referenceable type"
    test_function0<void (Foo::*)()>();
    test_function0<void (Foo::*)() const>();
    test_function0<void (Foo::*)() &>();
    test_function0<void (Foo::*)() &&>();
    test_function0<void (Foo::*)() const &>();
    test_function0<void (Foo::*)() const &&>();

  return 0;
}
