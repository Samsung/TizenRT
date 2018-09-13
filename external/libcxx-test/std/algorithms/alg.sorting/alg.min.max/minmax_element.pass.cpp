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

// <algorithm>

// template<ForwardIterator Iter>
//   requires LessThanComparable<Iter::value_type>
//   pair<Iter, Iter>
//   minmax_element(Iter first, Iter last);

#include <algorithm>
#include <random>
#include <cassert>

#include "test_iterators.h"
#include "libcxx_tc_common.h"

static std::mt19937 randomness;

template <class Iter>
static int
test(Iter first, Iter last)
{
    std::pair<Iter, Iter> p = std::minmax_element(first, last);
    if (first != last)
    {
        for (Iter j = first; j != last; ++j)
        {
            TC_ASSERT_EXPR(!(*j < *p.first));
            TC_ASSERT_EXPR(!(*p.second < *j));
        }
    }
    else
    {
        TC_ASSERT_EXPR(p.first == last);
        TC_ASSERT_EXPR(p.second == last);
    }

    return 0;
}

template <class Iter>
static int
test(int N)
{
    int* a = new int[N];
    for (int i = 0; i < N; ++i)
        a[i] = i;
    std::shuffle(a, a+N, randomness);
    TC_ASSERT_FUNC((test(Iter(a), Iter(a+N))));
    delete [] a;
    return 0;
}

template <class Iter>
static int
test()
{
    TC_ASSERT_FUNC((test<Iter>(0)));
    TC_ASSERT_FUNC((test<Iter>(1)));
    TC_ASSERT_FUNC((test<Iter>(2)));
    TC_ASSERT_FUNC((test<Iter>(3)));
    TC_ASSERT_FUNC((test<Iter>(10)));
    TC_ASSERT_FUNC((test<Iter>(1000)));
    {
    const int N = 100;
    int* a = new int[N];
    for (int i = 0; i < N; ++i)
        a[i] = 5;
    std::shuffle(a, a+N, randomness);
    std::pair<Iter, Iter> p = std::minmax_element(Iter(a), Iter(a+N));
    TC_ASSERT_EXPR(base(p.first) == a);
    TC_ASSERT_EXPR(base(p.second) == a+N-1);
    delete [] a;
    }
    return 0;
}

#if TEST_STD_VER >= 14
constexpr int il[] = { 2, 4, 6, 8, 7, 5, 3, 1 };
#endif

static void constexpr_test()
{
#if TEST_STD_VER >= 14
    constexpr auto p = std::minmax_element(il, il+8);
    static_assert ( *(p.first)  == 1, "" );
    static_assert ( *(p.second) == 8, "" );
#endif
}

int tc_libcxx_algorithms_alg_min_max_minmax_element(void)
{
    TC_ASSERT_FUNC((test<forward_iterator<const int*> >()));
    TC_ASSERT_FUNC((test<bidirectional_iterator<const int*> >()));
    TC_ASSERT_FUNC((test<random_access_iterator<const int*> >()));
    TC_ASSERT_FUNC((test<const int*>()));

   constexpr_test();
   TC_SUCCESS_RESULT();
   return 0;
}
