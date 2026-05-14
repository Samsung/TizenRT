//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <functional>

// not2

// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_ENABLE_CXX20_REMOVED_BINDER_TYPEDEFS
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_ENABLE_CXX20_REMOVED_NEGATORS
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_negators_not2(void) {
    typedef std::logical_and<int> F;
    TC_ASSERT_EXPR(!std::not2(F())(36, 36));
    TC_ASSERT_EXPR( std::not2(F())(36, 0));
    TC_ASSERT_EXPR( std::not2(F())(0, 36));
    TC_ASSERT_EXPR( std::not2(F())(0, 0));

  return 0;
}
