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

// template<ForwardIterator Iter, StrictWeakOrder<auto, Iter::value_type> Compare>
//   requires CopyConstructible<Compare>
//   Iter
//   max_element(Iter first, Iter last, Compare comp);

#include <algorithm>
#include <functional>
#include <random>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"
#include "test_iterators.h"

static std::mt19937 randomness;

template <class Iter>
static int
test(Iter first, Iter last)
{
    Iter i = std::max_element(first, last, std::greater<int>());
    if (first != last)
    {
        for (Iter j = first; j != last; ++j)
            TC_ASSERT_EXPR(!std::greater<int>()(*i, *j));
    }
    else
        TC_ASSERT_EXPR(i == last);
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
    return 0;
}

template <class Iter, class Pred>
static int test_eq0(Iter first, Iter last, Pred p)
{
    TC_ASSERT_EXPR(first == std::max_element(first, last, p));
    return 0;
}

static int test_eq()
{
    const int N = 10;
    int* a = new int[N];
    for (int i = 0; i < N; ++i)
        a[i] = 10; // all the same
    TC_ASSERT_FUNC((test_eq0(a, a+N, std::less<int>())));
    TC_ASSERT_FUNC((test_eq0(a, a+N, std::greater<int>())));
    delete [] a;
    return 0;
}

#if TEST_STD_VER >= 14
constexpr int il[] = { 2, 4, 6, 8, 7, 5, 3, 1 };
struct less { constexpr bool operator ()( const int &x, const int &y) const { return x < y; }};
#endif

void constexpr_test()
{
#if TEST_STD_VER >= 14
    constexpr auto p = std::max_element(il, il+8, less());
    static_assert ( *p == 8, "" );
#endif
}

int tc_libcxx_algorithms_alg_min_max_max_element_comp(void)
{
    TC_ASSERT_FUNC((test<forward_iterator<const int*> >()));
    TC_ASSERT_FUNC((test<bidirectional_iterator<const int*> >()));
    TC_ASSERT_FUNC((test<random_access_iterator<const int*> >()));
    TC_ASSERT_FUNC((test<const int*>()));
    TC_ASSERT_FUNC((test_eq()));

    constexpr_test();

    TC_SUCCESS_RESULT();
    return 0;
}
