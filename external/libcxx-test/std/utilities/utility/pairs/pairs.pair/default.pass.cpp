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

// template <class T1, class T2> struct pair

// constexpr pair();

// This test doesn't pass due to a constexpr bug in GCC 4.9 that fails
// to initialize any type without a user provided constructor in a constant
// expression (e.g. float).
// XFAIL: gcc-4.9

// NOTE: The SFINAE on the default constructor is tested in
//       default-sfinae.pass.cpp


#include <utility>
#include <type_traits>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "archetypes.hpp"

int tc_libcxx_utilities_pairs_pair_default(void)
{
    {
        typedef std::pair<float, short*> P;
        P p;
        TC_ASSERT_EXPR(p.first == 0.0f);
        TC_ASSERT_EXPR(p.second == nullptr);
    }
#if TEST_STD_VER > 11
    {
        typedef std::pair<float, short*> P;
        constexpr P p;
        static_assert(p.first == 0.0f, "");
        static_assert(p.second == nullptr, "");
    }
    {
        using NoDefault = ImplicitTypes::NoDefault;
        using P = std::pair<int, NoDefault>;
        static_assert(!std::is_default_constructible<P>::value, "");
        using P2 = std::pair<NoDefault, int>;
        static_assert(!std::is_default_constructible<P2>::value, "");
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
