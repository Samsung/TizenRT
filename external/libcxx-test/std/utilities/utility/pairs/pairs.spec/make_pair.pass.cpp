/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
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
        P1 p1 = std::make_pair(nullptr, static_cast<short>(4));
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
