//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <functional>
// REQUIRES: c++11 || c++14

// class function<R(ArgTypes...)>

// template<class A> function(allocator_arg_t, const A&);
//
// This signature was removed in C++17

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_alloc(void) {
    {
    std::function<int(int)> f(std::allocator_arg, bare_allocator<int>());
    TC_ASSERT_EXPR(!f);
    }

  return 0;
}
