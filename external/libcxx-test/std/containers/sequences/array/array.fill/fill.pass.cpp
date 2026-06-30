//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <array>

// void fill(const T& u);

#include <array>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX20 bool tests()
{
    {
        typedef double T;
        typedef std::array<T, 3> C;
        C c = {1, 2, 3.5};
        c.fill(5.5);
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c[0] == 5.5);
        TC_ASSERT_EXPR(c[1] == 5.5);
        TC_ASSERT_EXPR(c[2] == 5.5);
    }

    {
        typedef double T;
        typedef std::array<T, 0> C;
        C c = {};
        c.fill(5.5);
        TC_ASSERT_EXPR(c.size() == 0);
    }
    return true;
}

int tc_containers_sequences_array_array_fill_fill(void) {
    tests();
#if TEST_STD_VER >= 20
    static_assert(tests(), "");
#endif
    return 0;
}
