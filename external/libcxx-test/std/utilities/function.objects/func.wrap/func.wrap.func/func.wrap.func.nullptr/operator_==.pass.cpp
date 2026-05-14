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

// template <MoveConstructible R, MoveConstructible ... ArgTypes>
//   bool operator==(const function<R(ArgTypes...)>&, nullptr_t);
//
// template <MoveConstructible R, MoveConstructible ... ArgTypes>
//   bool operator==(nullptr_t, const function<R(ArgTypes...)>&);
//
// template <MoveConstructible R, MoveConstructible ... ArgTypes>
//   bool operator!=(const function<R(ArgTypes...)>&, nullptr_t);
//
// template <MoveConstructible  R, MoveConstructible ... ArgTypes>
//   bool operator!=(nullptr_t, const function<R(ArgTypes...)>&);

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int g(int) {return 0;}

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_nullptr_operator_eq_eq(void) {
    {
    std::function<int(int)> f;
    TC_ASSERT_EXPR(f == nullptr);
    TC_ASSERT_EXPR(nullptr == f);
    f = g;
    TC_ASSERT_EXPR(f != nullptr);
    TC_ASSERT_EXPR(nullptr != f);
    }

  return 0;
}
