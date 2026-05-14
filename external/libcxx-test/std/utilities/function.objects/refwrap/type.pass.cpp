//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <functional>

// reference_wrapper

// check for member typedef type

#include <functional>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

class C {};

int tc_utilities_function_objects_refwrap_type(void) {
    static_assert((std::is_same<std::reference_wrapper<C>::type,
                                                       C>::value), "");
    static_assert((std::is_same<std::reference_wrapper<void ()>::type,
                                                       void ()>::value), "");
    static_assert((std::is_same<std::reference_wrapper<int* (double*)>::type,
                                                       int* (double*)>::value), "");
    static_assert((std::is_same<std::reference_wrapper<void(*)()>::type,
                                                       void(*)()>::value), "");
    static_assert((std::is_same<std::reference_wrapper<int*(*)(double*)>::type,
                                                       int*(*)(double*)>::value), "");
    static_assert((std::is_same<std::reference_wrapper<int*(C::*)(double*)>::type,
                                                       int*(C::*)(double*)>::value), "");
    static_assert((std::is_same<std::reference_wrapper<int (C::*)(double*) const volatile>::type,
                                                       int (C::*)(double*) const volatile>::value), "");

  return 0;
}
