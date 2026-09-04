//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <utility>

// template <class T1, class T2> pair<V1, V2> make_pair(T1&&, T2&&);

#include <utility>
#include <memory>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

int tc_libcxx_utilities_pairs_spec_make_pair(void)
{
    {
        typedef std::pair<int, short> P1;
        P1 p1 = std::make_pair(3, static_cast<short>(4));
        TC_ASSERT_EXPR(p1.first == 3);
        TC_ASSERT_EXPR(p1.second == 4);
    }

#if TEST_STD_VER >= 11
    {
        typedef std::pair<std::unique_ptr<int>, short> P1;
        P1 p1 = std::make_pair(std::unique_ptr<int>(new int(3)), static_cast<short>(4));
        TC_ASSERT_EXPR(*p1.first == 3);
        TC_ASSERT_EXPR(p1.second == 4);
    }
    {
        typedef std::pair<std::unique_ptr<int>, short> P1;
        P1 p1 = std::make_pair(std::unique_ptr<int>(), static_cast<short>(4));
        TC_ASSERT_EXPR(p1.first == nullptr);
        TC_ASSERT_EXPR(p1.second == 4);
    }
#endif
#if TEST_STD_VER >= 14
    {
        typedef std::pair<int, short> P1;
        constexpr P1 p1 = std::make_pair(3, static_cast<short>(4));
        static_assert(p1.first == 3, "");
        static_assert(p1.second == 4, "");
    }
#endif

    TC_SUCCESS_RESULT();
    return 0;
}
