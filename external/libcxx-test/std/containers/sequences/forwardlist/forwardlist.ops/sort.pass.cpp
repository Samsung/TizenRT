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

// static int sort();

#include <forward_list>
#include <iterator>
#include <algorithm>
#include <vector>
#include <random>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


static std::mt19937 randomness;

template <class C>
static int test(int N)
{
    typedef typename C::value_type T;
    typedef std::vector<T> V;
    V v;
    for (int i = 0; i < N; ++i)
        v.push_back(i);
    std::shuffle(v.begin(), v.end(), randomness);
    C c(v.begin(), v.end());
    c.sort();
    TC_ASSERT_EXPR(distance(c.begin(), c.end()) == N);
    typename C::const_iterator j = c.begin();
    for (int i = 0; i < N; ++i, ++j)
        TC_ASSERT_EXPR(*j == i);
    return 0;
}

int tc_libcxx_containers_forwardlist_ops_sort(void)
{
    for (int i = 0; i < 40; ++i)
        test<std::forward_list<int> >(i);
    TC_SUCCESS_RESULT();
    return 0;
}
