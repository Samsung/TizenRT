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

// explicit operator bool() const

#include <functional>
#include <cassert>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int g(int) {return 0;}

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_cap_operator_bool(void) {
    static_assert(std::is_constructible<bool, std::function<void()> >::value, "");
    static_assert(!std::is_convertible<std::function<void()>, bool>::value, "");

    {
    std::function<int(int)> f;
    TC_ASSERT_EXPR(!f);
    f = g;
    TC_ASSERT_EXPR(f);
    }

  return 0;
}
