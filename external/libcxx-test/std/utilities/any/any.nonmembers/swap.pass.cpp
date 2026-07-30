//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03, c++11, c++14

// XFAIL: availability-bad_any_cast-missing && !no-exceptions

// <any>

// void swap(any &, any &) noexcept

// swap(...) just wraps any::swap(...). That function is tested elsewhere.

#include <any>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

#if TEST_STD_VER >= 17
TEST_CONSTEXPR_CXX20 bool test_swap_any_nonmembers() {

    { // test noexcept
        std::any a;
        static_assert(noexcept(swap(a, a)), "swap(any&, any&) must be noexcept");
    }
    {
        std::any a1 = 1;
        std::any a2 = 2;

        swap(a1, a2);

        TC_ASSERT_EXPR(std::any_cast<int>(a1) == 2);
        TC_ASSERT_EXPR(std::any_cast<int>(a2) == 1);
    }

  return true;
}

int tc_any_swap_nonmember(void) {
    test_swap_any_nonmembers();
#if TEST_STD_VER > 17
    static_assert(test_swap_any_nonmembers());
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
#endif /* TEST_STD_VER >= 17 */
