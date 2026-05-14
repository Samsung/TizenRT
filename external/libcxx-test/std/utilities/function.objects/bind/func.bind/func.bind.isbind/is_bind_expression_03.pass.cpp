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
// <functional>

//-----------------------------------------------------------------------------
// TESTING template<class T> struct is_bind_expression
//
// bind is not implemented in C++03 so nothing is a bind expression. However
// for compatibility reasons the trait is_bind_expression should be available
// in C++03 and it should always return false.

#include <functional>

#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class T>
void test() {
    static_assert(!std::is_bind_expression<T>::value, "");
}

struct C {};

int tc_utilities_function_objects_bind_func_bind_func_bind_isbind_is_bind_expression_03(void) {
    test<int>();
    test<void>();
    test<C>();
    test<C&>();
    test<C const&>();
    test<C*>();
    test<void()>();
    test<int(*)()>();
    test<int (C::*)()>();
    test<decltype(std::placeholders::_2)>();

  return 0;
}
