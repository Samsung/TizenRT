//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14
// XFAIL: availability-bad_optional_access-missing

// <optional>

// class bad_optional_access is default constructible

#include <optional>
#include <type_traits>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_utilities_optional_optional_bad_optional_access_default(void) {
    using std::bad_optional_access;
    bad_optional_access ex;

  return 0;
}
