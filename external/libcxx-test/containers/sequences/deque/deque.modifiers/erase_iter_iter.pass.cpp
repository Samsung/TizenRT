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
//
// REQUIRES: long_tests

// <deque>

// iterator erase(const_iterator f, const_iterator l)

#include "asan_testing.h"
#include <deque>
#include <algorithm>
#include <iterator>
#include <cassert>
#include <cstddef>

#include "min_allocator.h"
#include "test_macros.h"
#include "libcxx_tc_common.h"

#ifndef TEST_HAS_NO_EXCEPTIONS
struct Throws {
    Throws() : v_(0) {}
    Throws(int v) : v_(v) {}
    Throws(const Throws  &rhs) : v_(rhs.v_) { if (sThrows) throw 1; }
    Throws(      Throws &&rhs) : v_(rhs.v_) { if (sThrows) throw 1; }
    Throws& operator=(const Throws  &rhs) { v_ = rhs.v_; return *this; }
    Throws& operator=(      Throws &&rhs) { v_ = rhs.v_; return *this; }
    int v_;

    static bool sThrows;
    };

bool Throws::sThrows = false;
#endif


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
void
test(int P, C& c1, int size)
{
    typedef typename C::iterator I;
    TC_ASSERT_EXPR(static_cast<std::size_t>(P + size) <= c1.size());
    std::size_t c1_osize = c1.size();
    I i = c1.erase(c1.cbegin() + P, c1.cbegin() + (P + size));
    TC_ASSERT_EXPR(i == c1.begin() + P);
    TC_ASSERT_EXPR(c1.size() == c1_osize - size);
    TC_ASSERT_EXPR(static_cast<std::size_t>(std::distance(c1.begin(), c1.end())) == c1.size());
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(c1));
    i = c1.begin();
    int j = 0;
    for (; j < P; ++j, ++i)
        TC_ASSERT_EXPR(*i == j);
    for (j += size; static_cast<std::size_t>(j) < c1_osize; ++j, ++i)
        TC_ASSERT_EXPR(*i == j);
}

template <class C>
void
testN(int start, int N)
{
    int pstep = std::max(N / std::max(std::min(N, 10), 1), 1);
    for (int p = 0; p <= N; p += pstep)
    {
        int sstep = std::max((N - p) / std::max(std::min(N - p, 10), 1), 1);
        for (int s = 0; s <= N - p; s += sstep)
        {
            C c1 = make<C>(N, start);
            test(p, c1, s);
        }
    }
}

int tc_containers_sequences_deque_deque_modifiers_erase_iter_iter(void) {
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
#ifndef TEST_HAS_NO_EXCEPTIONS
// Test for LWG2953:
// Throws: Nothing unless an exception is thrown by the assignment operator of T.
// (which includes move assignment)
    {
    Throws arr[] = {1, 2, 3};
    std::deque<Throws> v(arr, arr+3);
    Throws::sThrows = true;
    v.erase(v.begin(), --v.end());
    TC_ASSERT_EXPR(v.size() == 1);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(v));
    v.erase(v.begin(), v.end());
    TC_ASSERT_EXPR(v.size() == 0);
    LIBCPP_ASSERT(is_double_ended_contiguous_container_asan_correct(v));
    }
#endif

  return 0;
}
