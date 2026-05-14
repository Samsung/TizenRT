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

// add_pointer
// If T names a referenceable type or a (possibly cv-qualified) void type then
//    the member typedef type shall name the same type as remove_reference_t<T>*;
//    otherwise, type shall name T.

#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T, class U>
void test_add_pointer()
{
    ASSERT_SAME_TYPE(U, typename std::add_pointer<T>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(U, std::add_pointer_t<T>);
#endif
}

template <class F>
void test_function0()
{
    ASSERT_SAME_TYPE(F*, typename std::add_pointer<F>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(F*, std::add_pointer_t<F>);
#endif
}

template <class F>
void test_function1()
{
    ASSERT_SAME_TYPE(F, typename std::add_pointer<F>::type);
#if TEST_STD_VER > 11
    ASSERT_SAME_TYPE(F, std::add_pointer_t<F>);
#endif
}

struct Foo {};

int tc_utilities_meta_meta_trans_meta_trans_ptr_add_pointer(void) {
    test_add_pointer<void, void*>();
    test_add_pointer<int, int*>();
    test_add_pointer<int[3], int(*)[3]>();
    test_add_pointer<int&, int*>();
    test_add_pointer<const int&, const int*>();
    test_add_pointer<int*, int**>();
    test_add_pointer<const int*, const int**>();
    test_add_pointer<Foo, Foo*>();

//  LWG 2101 specifically talks about add_pointer and functions.
//  The term of art is "a referenceable type", which a cv- or ref-qualified function is not.
    test_function0<void()>();
#if TEST_STD_VER >= 11
    test_function1<void() const>();
    test_function1<void() &>();
    test_function1<void() &&>();
    test_function1<void() const &>();
    test_function1<void() const &&>();
#endif

//  But a cv- or ref-qualified member function *is* "a referenceable type"
    test_function0<void (Foo::*)()>();
#if TEST_STD_VER >= 11
    test_function0<void (Foo::*)() const>();
    test_function0<void (Foo::*)() &>();
    test_function0<void (Foo::*)() &&>();
    test_function0<void (Foo::*)() const &>();
    test_function0<void (Foo::*)() const &&>();
#endif

  return 0;
}
