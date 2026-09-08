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

// template <class T1, class T2>
// struct pair
// {
//     typedef T1 first_type;
//     typedef T2 second_type;

#include <utility>
#include <type_traits>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_utilities_pairs_pair_types(void)
{
    typedef std::pair<float, short*> P;
    static_assert((std::is_same<P::first_type, float>::value), "");
    static_assert((std::is_same<P::second_type, short*>::value), "");
    TC_SUCCESS_RESULT();
    return 0;
}
