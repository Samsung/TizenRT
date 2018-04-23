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

// static int splice_after(const_iterator p, forward_list&& x, const_iterator i);

#include <forward_list>
#include <cassert>
#include "libcxx_tc_common.h"
#include <iterator>
#include <cstddef>

#include "test_macros.h"

typedef int T;
const T t1[] = {0, 1, 2, 3, 4, 5, 6, 7};
const T t2[] = {10, 11, 12};
const std::ptrdiff_t size_t1 = std::end(t1) - std::begin(t1);
const std::ptrdiff_t size_t2 = std::end(t2) - std::begin(t2);

template <class C>
static int
testd(const C& c, int p, int f)
{
    typename C::const_iterator i = c.begin();
    int n1 = 0;
    for (; n1 < p; ++n1, ++i)
        TC_ASSERT_EXPR(*i == t1[n1]);
    for (int n2 = f; n2 < f+1; ++n2, ++i)
        TC_ASSERT_EXPR(*i == t2[n2]);
    for (; n1 < size_t1; ++n1, ++i)
        TC_ASSERT_EXPR(*i == t1[n1]);
    TC_ASSERT_EXPR(distance(c.begin(), c.end()) == size_t1 + 1);
    return 0;
}

template <class C>
static int
tests(const C& c, int p, int f)
{
    typename C::const_iterator i = c.begin();
    int n = 0;
    if (p == f || p == f+1)
    {
        for (n = 0; n < size_t1; ++n, ++i)
            TC_ASSERT_EXPR(*i == t1[n]);
    }
    else if (p < f)
    {
        for (n = 0; n < p; ++n, ++i)
            TC_ASSERT_EXPR(*i == t1[n]);
        for (n = f; n < f+1; ++n, ++i)
            TC_ASSERT_EXPR(*i == t1[n]);
        for (n = p; n < f; ++n, ++i)
            TC_ASSERT_EXPR(*i == t1[n]);
        for (n = f+1; n < size_t1; ++n, ++i)
            TC_ASSERT_EXPR(*i == t1[n]);
    }
    else // p > f+1
    {
        for (n = 0; n < f; ++n, ++i)
            TC_ASSERT_EXPR(*i == t1[n]);
        for (n = f+1; n < p; ++n, ++i)
            TC_ASSERT_EXPR(*i == t1[n]);
        for (n = f; n < f+1; ++n, ++i)
            TC_ASSERT_EXPR(*i == t1[n]);
        for (n = p; n < size_t1; ++n, ++i)
            TC_ASSERT_EXPR(*i == t1[n]);
    }
    TC_ASSERT_EXPR(distance(c.begin(), c.end()) == size_t1);
    return 0;
}

int tc_libcxx_containers_forwardlist_ops_splice_after_one(void)
{
    {
    // splicing different containers
    typedef std::forward_list<T> C;
    for (int f = 0; f <= size_t2-1; ++f)
    {
        for (int p = 0; p <= size_t1; ++p)
        {
            C c1(std::begin(t1), std::end(t1));
            C c2(std::begin(t2), std::end(t2));

            c1.splice_after(next(c1.cbefore_begin(), p), std::move(c2),
                  next(c2.cbefore_begin(), f));
            testd(c1, p, f);
        }
    }

    // splicing within same container
    for (int f = 0; f <= size_t1-1; ++f)
    {
        for (int p = 0; p <= size_t1; ++p)
        {
            C c1(std::begin(t1), std::end(t1));

            c1.splice_after(next(c1.cbefore_begin(), p), std::move(c1),
                  next(c1.cbefore_begin(), f));
            tests(c1, p, f);
        }
    }
    }
    TC_SUCCESS_RESULT();
    return 0;
}
