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

// UNSUPPORTED: c++98, c++03

// <utility>

// template <class T1, class T2> struct pair

// template <class... Args1, class... Args2>
//     pair(piecewise_construct_t, tuple<Args1...> first_args,
//                                 tuple<Args2...> second_args);

#include <utility>
#include <tuple>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_utilities_pairs_pair_piecewise(void)
{
    {
        typedef std::pair<int, int*> P1;
        typedef std::pair<int*, int> P2;
        typedef std::pair<P1, P2> P3;
        P3 p3(std::piecewise_construct, std::tuple<int, int*>(3, nullptr),
                                        std::tuple<int*, int>(nullptr, 4));
        TC_ASSERT_EXPR(p3.first == P1(3, nullptr));
        TC_ASSERT_EXPR(p3.second == P2(nullptr, 4));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
