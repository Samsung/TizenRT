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

// <forward_list>

// static int reverse();

#include <forward_list>
#include <iterator>
#include <algorithm>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


template <class C>
static int test(int N)
{
    C c;
    for (int i = 0; i < N; ++i)
        c.push_front(i);
    c.reverse();
    TC_ASSERT_EXPR(distance(c.begin(), c.end()) == N);
    typename C::const_iterator j = c.begin();
    for (int i = 0; i < N; ++i, ++j)
        TC_ASSERT_EXPR(*j == i);
    return 0;
}

int tc_libcxx_containers_forwardlist_ops_reverse(void)
{
    for (int i = 0; i < 10; ++i)
        test<std::forward_list<int> >(i);
    TC_SUCCESS_RESULT();
    return 0;
}
