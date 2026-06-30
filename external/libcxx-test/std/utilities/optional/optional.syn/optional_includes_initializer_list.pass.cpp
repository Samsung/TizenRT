//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14
// <optional>

// #include <initializer_list>

#include <optional>

#include "test_macros.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER >= 17
namespace {
TEST_CONSTEXPR_CXX20 bool test() {
    using std::optional;

    std::initializer_list<int> list;
    (void)list;

    return true;
}
} // namespace

int tc_utilities_optional_optional_syn_optional_includes_initializer_list_pass(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif /* TEST_STD_VER >= 17 */
