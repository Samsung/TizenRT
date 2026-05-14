//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <functional>

// class function<R(ArgTypes...)>

// Make sure we can use std::function with a type that has a hostile overload
// of operator&().

#include <functional>
#include <cassert>

#include "operator_hijacker.h"
#include "libcxx_tc_common.h"

struct TrapAddressof : operator_hijacker {
    int operator()() const { return 1; }
};

int tc_utilities_function_objects_func_wrap_func_wrap_func_addressof(void) {
    std::function<int()> f = TrapAddressof();
    TC_ASSERT_EXPR(f() == 1);
    return 0;
}
