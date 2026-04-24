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

// template <class ForwardIterator, class Size, class T>
//   ForwardIterator
//   uninitialized_fill_n(ForwardIterator first, Size n, const T& x);

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

int tc_utilities_memory_specialized_algorithms_uninitialized_fill_n_uninitialized_fill_n(void) {
    {
    const int N = 5;
    char pool[sizeof(B)*N] = {0};
    B* bp = (B*)pool;
    TC_ASSERT_EXPR(B::population_ == 0);
#ifndef TEST_HAS_NO_EXCEPTIONS
    try
    {
        std::uninitialized_fill_n(bp, 5, B());
        TC_ASSERT_EXPR(false);
    }
    catch (...)
    {
        TC_ASSERT_EXPR(B::population_ == 0);
    }
#endif
    B::count_ = 0;
    B* r = std::uninitialized_fill_n(bp, 2, B());
    TC_ASSERT_EXPR(r == bp + 2);
    for (int i = 0; i < 2; ++i)
        TC_ASSERT_EXPR(bp[i].data_ == 1);
    TC_ASSERT_EXPR(B::population_ == 2);
    }
    {
    {
    const int N = 5;
    char pool[N*sizeof(Nasty)] = {0};
    Nasty* bp = (Nasty*)pool;

    Nasty::counter_ = 23;
    std::uninitialized_fill_n(bp, N, Nasty());
    for (int i = 0; i < N; ++i)
        TC_ASSERT_EXPR(bp[i].i_ == 23);
    }

    }

  return 0;
}
