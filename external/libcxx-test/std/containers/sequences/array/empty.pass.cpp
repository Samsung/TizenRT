//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <array>

// class array

// constexpr bool empty() const noexcept;

#include <array>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

TEST_CONSTEXPR_CXX14 bool tests()
{
    {
        typedef std::array<int, 2> C;
        C c = {};
        ASSERT_NOEXCEPT(c.empty());
        TC_ASSERT_EXPR(!c.empty());
    }
    {
        typedef std::array<int, 0> C;
        C c = {};
        ASSERT_NOEXCEPT(c.empty());
        TC_ASSERT_EXPR(c.empty());
    }

    return true;
}

int tc_containers_sequences_array_empty(void) {
    tests();
#if TEST_STD_VER >= 14
    static_assert(tests(), "");
#endif

#if TEST_STD_VER >= 11
    // Sanity check for constexpr in C++11
    {
        constexpr std::array<int, 3> array = {};
        static_assert(!array.empty(), "");
    }
#endif

    return 0;
}
