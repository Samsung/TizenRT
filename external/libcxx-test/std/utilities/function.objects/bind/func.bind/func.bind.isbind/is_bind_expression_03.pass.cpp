//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
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
