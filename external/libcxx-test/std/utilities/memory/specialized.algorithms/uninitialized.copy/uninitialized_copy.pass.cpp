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
// <memory>

// template <class InputIterator, class ForwardIterator>
//   ForwardIterator
//   uninitialized_copy(InputIterator first, InputIterator last,
//                      ForwardIterator result);

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct B
{
    static int count_;
    static int population_;
    int data_;
    explicit B() : data_(1) { ++population_; }
    B(const B &b) {
      ++count_;
      if (count_ == 3)
        TEST_THROW(1);
      data_ = b.data_;
      ++population_;
    }
    ~B() {data_ = 0; --population_; }
};

int B::count_ = 0;
int B::population_ = 0;

struct Nasty
{
    Nasty() : i_ ( counter_++ ) {}
    Nasty * operator &() const { return nullptr; }
    int i_;
    static int counter_;
};

int Nasty::counter_ = 0;

int tc_utilities_memory_specialized_algorithms_uninitialized_copy_uninitialized_copy(void) {
    {
    const int N = 5;
    char pool[sizeof(B)*N] = {0};
    B* bp = (B*)pool;
    B b[N];
    TC_ASSERT_EXPR(B::population_ == N);
#ifndef TEST_HAS_NO_EXCEPTIONS
    try
    {
        std::uninitialized_copy(b, b+N, bp);
        TC_ASSERT_EXPR(false);
    }
    catch (...)
    {
        TC_ASSERT_EXPR(B::population_ == N);
    }
#endif
    B::count_ = 0;
    std::uninitialized_copy(b, b+2, bp);
    for (int i = 0; i < 2; ++i)
        TC_ASSERT_EXPR(bp[i].data_ == 1);
    TC_ASSERT_EXPR(B::population_ == N + 2);
    }

    {
    const int N = 5;
    char pool[sizeof(Nasty)*N] = {0};
    Nasty * p = (Nasty *) pool;
    Nasty arr[N];
    std::uninitialized_copy(arr, arr+N, p);
    for (int i = 0; i < N; ++i) {
        TC_ASSERT_EXPR(arr[i].i_ == i);
        TC_ASSERT_EXPR(  p[i].i_ == i);
        }
    }


  return 0;
}
