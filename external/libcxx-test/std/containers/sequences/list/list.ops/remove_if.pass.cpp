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

// <list>

// template <class Pred> static int remove_if(Pred pred);

#include <list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include <functional>

#include "counting_predicates.hpp"

bool even(int i)
{
    return i % 2 == 0;
    return 0;
}

static bool g(int i)
{
    return i < 3;
    return 0;
}

typedef unary_counting_predicate<bool(*)(int), int> Predicate;

int tc_libcxx_containers_list_ops_remove_if(void)
{
    {
    int a1[] = {1, 2, 3, 4};
    int a2[] = {3, 4};
    std::list<int> c(a1, a1+4);
    Predicate cp(g);
    c.remove_if(std::ref(cp));
    TC_ASSERT_EXPR(c == std::list<int>(a2, a2+2));
    TC_ASSERT_EXPR(cp.count() == 4);
    }
    {
    int a1[] = {1, 2, 3, 4};
    int a2[] = {1, 3};
    std::list<int> c(a1, a1+4);
    Predicate cp(even);
    c.remove_if(std::ref(cp));
    TC_ASSERT_EXPR(c == std::list<int>(a2, a2+2));
    TC_ASSERT_EXPR(cp.count() == 4);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
