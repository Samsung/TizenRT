//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// INVOKE (f, t1, t2, ..., tN)

// The tests for INVOKE (f, t1, t2, ..., tN) live in the "test/libcxx" tree
// since they require calling the implementation specific "__invoke" and
// "__invoke_constexpr" functions.
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_func_require_INVOKE_tested_elsewhere(void) {
  return 0;
}
