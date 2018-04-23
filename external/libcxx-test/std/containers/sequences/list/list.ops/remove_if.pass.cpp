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

// <list>

// template <class Pred> static int remove_if(Pred pred);

#include <list>
#include <cassert>
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
