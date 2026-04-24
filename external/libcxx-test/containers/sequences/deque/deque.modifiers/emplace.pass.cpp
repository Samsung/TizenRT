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

// template <class... Args> iterator emplace(const_iterator p, Args&&... args);

// UNSUPPORTED: c++03

#include "asan_testing.h"
#include <deque>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "../../../Emplaceable.h"
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
    C c(init);
    for (int i = 0; i < init-start; ++i)
        c.pop_back();
    for (int i = 0; i < size; ++i)
        c.push_back(Emplaceable());
    for (int i = 0; i < start; ++i)
        c.pop_front();
    return c;
}

template <class C>
void
test(int P, C& c1)
{
    typedef typename C::const_iterator CI;
    std::size_t c1_osize = c1.size();
    CI i = c1.emplace(c1.begin() + P, Emplaceable(1, 2.5));
    TC_ASSERT_EXPR(i == c1.begin() + P);
    TC_ASSERT_EXPR(c1.size() == c1_osize + 1);
    TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.begin(), c1.end())) == c1.size());
    TC_ASSERT_EXPR(*i == Emplaceable(1, 2.5));
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c1));
}

template <class C>
void
testN(int start, int N)
{
    for (int i = 0; i <= 3; ++i)
    {
        if (0 <= i && i <= N)
        {
            C c1 = make<C>(N, start);
            test(i, c1);
        }
    }
    for (int i = N/2-1; i <= N/2+1; ++i)
    {
        if (0 <= i && i <= N)
        {
            C c1 = make<C>(N, start);
            test(i, c1);
        }
    }
    for (int i = N - 3; i <= N; ++i)
    {
        if (0 <= i && i <= N)
        {
            C c1 = make<C>(N, start);
            test(i, c1);
        }
    }
}


int tc_containers_sequences_deque_deque_modifiers_emplace(void) {
    {
    int rng[] = {0, 1, 2, 3, 1023, 1024, 1025, 2047, 2048, 2049};
    const int N = sizeof(rng)/sizeof(rng[0]);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            testN<std::deque<Emplaceable> >(rng[i], rng[j]);
    }
    {
    int rng[] = {0, 1, 2, 3, 1023, 1024, 1025, 2047, 2048, 2049};
    const int N = sizeof(rng)/sizeof(rng[0]);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            testN<std::deque<Emplaceable, min_allocator<Emplaceable>> >(rng[i], rng[j]);
    }
    {
    int rng[] = {0, 1, 2, 3, 1023, 1024, 1025, 2047, 2048, 2049};
    const int N = sizeof(rng)/sizeof(rng[0]);
    for (int i = 0; i < N; ++i)
        for (int j = 0; j < N; ++j)
            testN<std::deque<Emplaceable, safe_allocator<Emplaceable>> >(rng[i], rng[j]);
    }

  return 0;
}
