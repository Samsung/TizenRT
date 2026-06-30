//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14, c++17

// <functional>

// Test that std::compare_three_way is defined in <functional>,
// not only in <compare>.

#include <functional>
#include <cassert>
#include "libcxx_tc_common.h"

int tc_utilities_function_objects_comparisons_compare_three_way_functional(void) {
    TC_ASSERT_EXPR(std::compare_three_way()(1, 2) < 0);
    TC_ASSERT_EXPR(std::compare_three_way()(1, 1) == 0);
    TC_ASSERT_EXPR(std::compare_three_way()(2, 1) > 0);

    return 0;
}
