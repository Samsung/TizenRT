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

// <forward_list>

// template <class Predicate> static int remove_if(Predicate pred);

#include <forward_list>
#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"
#include <cstddef>

#include "counting_predicates.hpp"


bool g(int i)
{
    return i < 3;
    return 0;
}

int tc_libcxx_containers_forwardlist_ops_remove_if(void)
{
    {
        typedef int T;
        typedef unary_counting_predicate<bool(*)(T), T> Predicate;
        typedef std::forward_list<T> C;
        const T t1[] = {0, 5, 5, 0, 0, 0, 5};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        Predicate cp(g);
        c1.remove_if(std::ref(cp));
        TC_ASSERT_EXPR(c1 == c2);
        TC_ASSERT_EXPR(cp.count() == static_cast<std::size_t>(std::distance(std::begin(t1), std::end(t1))));
    }
    {
        typedef int T;
        typedef unary_counting_predicate<bool(*)(T), T> Predicate;
        typedef std::forward_list<T> C;
        const T t1[] = {0, 0, 0, 0};
        C c1(std::begin(t1), std::end(t1));
        C c2;
        Predicate cp(g);
        c1.remove_if(std::ref(cp));
        TC_ASSERT_EXPR(c1 == c2);
        TC_ASSERT_EXPR(cp.count() == static_cast<std::size_t>(std::distance(std::begin(t1), std::end(t1))));
    }
    {
        typedef int T;
        typedef unary_counting_predicate<bool(*)(T), T> Predicate;
        typedef std::forward_list<T> C;
        const T t1[] = {5, 5, 5};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        Predicate cp(g);
        c1.remove_if(std::ref(cp));
        TC_ASSERT_EXPR(c1 == c2);
        TC_ASSERT_EXPR(cp.count() == static_cast<std::size_t>(std::distance(std::begin(t1), std::end(t1))));
    }
    {
        typedef int T;
        typedef unary_counting_predicate<bool(*)(T), T> Predicate;
        typedef std::forward_list<T> C;
        C c1;
        C c2;
        Predicate cp(g);
        c1.remove_if(std::ref(cp));
        TC_ASSERT_EXPR(c1 == c2);
        TC_ASSERT_EXPR(cp.count() == 0);
    }
    {
        typedef int T;
        typedef unary_counting_predicate<bool(*)(T), T> Predicate;
        typedef std::forward_list<T> C;
        const T t1[] = {5, 5, 5, 0};
        const T t2[] = {5, 5, 5};
        C c1(std::begin(t1), std::end(t1));
        C c2(std::begin(t2), std::end(t2));
        Predicate cp(g);
        c1.remove_if(std::ref(cp));
        TC_ASSERT_EXPR(c1 == c2);
        TC_ASSERT_EXPR(cp.count() == static_cast<std::size_t>(std::distance(std::begin(t1), std::end(t1))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
