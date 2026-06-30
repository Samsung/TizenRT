//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// Regression test for https://llvm.org/PR27375

// UNSUPPORTED: c++03

#include <tuple>
#include "libcxx_tc_common.h"

int tc_utilities_tuple_tuple_tuple_PR27375(void) {
    std::tuple<int&>(std::tuple<int&&>(42));

    return 0;
}
