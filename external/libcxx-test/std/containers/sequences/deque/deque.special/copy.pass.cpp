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
// <deque>

// Optimization for deque::iterators

// template <class InputIterator, class OutputIterator>
//   OutputIterator
//   copy(InputIterator first, InputIterator last, OutputIterator result);

#include "asan_testing.h"
#include <deque>
#include <cassert>

#include "test_macros.h"
#include "test_iterators.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

template <class C>
C
make(int size, int start = 0 )
{
    const int b = 4096 / sizeof(int);
    int init = 0;
    if (start > 0)
    {
        init = (start+1) / b + ((start+1) % b != 0);
        init *= b;
        --init;
    }
    C c(init, 0);
    for (int i = 0; i < init-start; ++i)
        c.pop_back();
    for (int i = 0; i < size; ++i)
        c.push_back(i);
    for (int i = 0; i < start; ++i)
        c.pop_front();
    return c;
}

template <class C>
void testN(int start, int N)
{
    typedef typename C::iterator I;
    typedef typename C::const_iterator CI;
    typedef random_access_iterator<I> RAI;
    typedef random_access_iterator<CI> RACI;
    typedef cpp17_input_iterator<CI> ICI;
    C c1 = make<C>(N, start);
    C c2 = make<C>(N);
    TC_ASSERT_EXPR(std::copy(c1.cbegin(), c1.cend(), c2.begin()) == c2.end());
    TC_ASSERT_EXPR(c1 == c2);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c1));
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c2));
    TC_ASSERT_EXPR(std::copy(c2.cbegin(), c2.cend(), c1.begin()) == c1.end());
    TC_ASSERT_EXPR(c1 == c2);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c1));
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c2));
    TC_ASSERT_EXPR(std::copy(c1.cbegin(), c1.cend(), RAI(c2.begin())) == RAI(c2.end()));
    TC_ASSERT_EXPR(c1 == c2);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c1));
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c2));
    TC_ASSERT_EXPR(std::copy(c2.cbegin(), c2.cend(), RAI(c1.begin())) == RAI(c1.end()));
    TC_ASSERT_EXPR(c1 == c2);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c1));
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c2));
    TC_ASSERT_EXPR(std::copy(RACI(c1.cbegin()), RACI(c1.cend()), c2.begin()) == c2.end());
    TC_ASSERT_EXPR(c1 == c2);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c1));
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c2));
    TC_ASSERT_EXPR(std::copy(ICI(c2.cbegin()), ICI(c2.cend()), c1.begin()) == c1.end());
    TC_ASSERT_EXPR(c1 == c2);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c1));
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c2));
}

int tc_containers_sequences_deque_deque_special_copy(void) {
    {
    int rng[] = {0, 1, 2, 3, 1023, 1024, 1025, 2047, 2048, 2049};
    const int N = sizeof(rng)/sizeof(rng[0]);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            testN<std::deque<int> >(rng[i], rng[j]);
    }
#if TEST_STD_VER >= 11
    {
    int rng[] = {0, 1, 2, 3, 1023, 1024, 1025, 2047, 2048, 2049};
    const int N = sizeof(rng)/sizeof(rng[0]);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            testN<std::deque<int, min_allocator<int>> >(rng[i], rng[j]);
    }
#endif

  return 0;
}
