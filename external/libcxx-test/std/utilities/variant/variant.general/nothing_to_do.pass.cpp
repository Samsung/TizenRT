//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "libcxx_tc_common.h"

namespace {

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test_nothing() {
  return true;
}
}

int tc_utilities_variant_general_nothing_to_do(void) {
    test_nothing();
#if TEST_STD_VER > 17
    static_assert(test_nothing());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif /* TEST_STD_VER >= 17 */
