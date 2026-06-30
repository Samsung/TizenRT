//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// Class bad_function_call

// bad_function_call();

#include <functional>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_func_wrap_func_wrap_badcall_func_wrap_badcall_const_bad_function_call_ctor(void) {
    std::bad_function_call ex;

  return 0;
}
