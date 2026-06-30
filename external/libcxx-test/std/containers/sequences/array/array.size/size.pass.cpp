//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <array>

// template <class T, size_t N> constexpr size_type array<T,N>::size();

#include <array>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_array_array_size_size(void) {
    {
        typedef double T;
        typedef std::array<T, 3> C;
        C c = {1, 2, 3.5};
        TC_ASSERT_EXPR(c.size() == 3);
        TC_ASSERT_EXPR(c.max_size() == 3);
        TC_ASSERT_EXPR(!c.empty());
    }
    {
        typedef double T;
        typedef std::array<T, 0> C;
        C c = {};
        TC_ASSERT_EXPR(c.size() == 0);
        TC_ASSERT_EXPR(c.max_size() == 0);
        TC_ASSERT_EXPR(c.empty());
    }
#if TEST_STD_VER >= 11
    {
        typedef double T;
        typedef std::array<T, 3> C;
        constexpr C c = {1, 2, 3.5};
        static_assert(c.size() == 3, "");
        static_assert(c.max_size() == 3, "");
        static_assert(!c.empty(), "");
    }
    {
        typedef double T;
        typedef std::array<T, 0> C;
        constexpr C c = {};
        static_assert(c.size() == 0, "");
        static_assert(c.max_size() == 0, "");
        static_assert(c.empty(), "");
    }
#endif

    return 0;
}
