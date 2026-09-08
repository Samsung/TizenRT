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

// template<size_t I, class T1, class T2>
//     typename tuple_element<I, std::pair<T1, T2> >::type&&
//     get(pair<T1, T2>&&);

#include <utility>
#include <memory>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_utilities_pair_astuple_get_rv(void)
{
    {
        typedef std::pair<std::unique_ptr<int>, short> P;
        P p(std::unique_ptr<int>(new int(3)), static_cast<short>(4));
        std::unique_ptr<int> ptr = std::get<0>(std::move(p));
        TC_ASSERT_EXPR(*ptr == 3);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
