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

// template <class T1, class T2> struct pair

// tuple_size<pair<T1, T2> >::value

#include <utility>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_utilities_pair_astuple_tuple_size(void)
{
    {
        typedef std::pair<int, short> P1;
        static_assert((std::tuple_size<P1>::value == 2), "");
    }
    {
        typedef std::pair<int, short> const P1;
        static_assert((std::tuple_size<P1>::value == 2), "");
    }
    {
        typedef std::pair<int, short> volatile P1;
        static_assert((std::tuple_size<P1>::value == 2), "");
    }
    {
        typedef std::pair<int, short> const volatile P1;
        static_assert((std::tuple_size<P1>::value == 2), "");
    }
    TC_SUCCESS_RESULT();
    return 0;
}
