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
// UNSUPPORTED: c++03

// <functional>

// class function<R(ArgTypes...)>

// void swap(function& other);

#include <functional>
#include <cassert>

#include "count_new.h"

#include "test_macros.h"
#include "libcxx_tc_common.h"

class A {
  int data_[10];

public:
  static int count;

  explicit A(int j) {
    ++count;
    data_[0] = j;
  }

  A(const A &a) {
    ++count;
    for (int i = 0; i < 10; ++i)
      data_[i] = a.data_[i];
  }

  ~A() { --count; }

  int operator()(int i) const {
    for (int j = 0; j < 10; ++j)
      i += data_[j];
    return i;
  }

  int operator()() const { return -1; }
  int operator()(int, int) const { return -2; }
  int operator()(int, int, int) const { return -3; }

  int id() const { return data_[0]; }
};

int A::count = 0;

int g0() { return 0; }
int g(int) { return 0; }
int h(int) { return 1; }
int g2(int, int) { return 2; }
int g3(int, int, int) { return 3; }

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_mod_swap(void) {
  globalMemCounter.reset();
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
  {
    std::function<int(int)> f1 = A(1);
    std::function<int(int)> f2 = A(2);
    TC_ASSERT_EXPR(A::count == 2);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(2));
    RTTI_ASSERT(f1.target<A>()->id() == 1);
    RTTI_ASSERT(f2.target<A>()->id() == 2);
    f1.swap(f2);
    TC_ASSERT_EXPR(A::count == 2);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(2));
    RTTI_ASSERT(f1.target<A>()->id() == 2);
    RTTI_ASSERT(f2.target<A>()->id() == 1);
  }
  TC_ASSERT_EXPR(A::count == 0);
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
  {
    std::function<int(int)> f1 = A(1);
    std::function<int(int)> f2 = g;
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
    RTTI_ASSERT(f1.target<A>()->id() == 1);
    RTTI_ASSERT(*f2.target<int (*)(int)>() == g);
    f1.swap(f2);
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
    RTTI_ASSERT(*f1.target<int (*)(int)>() == g);
    RTTI_ASSERT(f2.target<A>()->id() == 1);
  }
  TC_ASSERT_EXPR(A::count == 0);
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
  {
    std::function<int(int)> f1 = g;
    std::function<int(int)> f2 = A(1);
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
    RTTI_ASSERT(*f1.target<int (*)(int)>() == g);
    RTTI_ASSERT(f2.target<A>()->id() == 1);
    f1.swap(f2);
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
    RTTI_ASSERT(f1.target<A>()->id() == 1);
    RTTI_ASSERT(*f2.target<int (*)(int)>() == g);
  }
  TC_ASSERT_EXPR(A::count == 0);
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
  {
    std::function<int(int)> f1 = g;
    std::function<int(int)> f2 = h;
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(*f1.target<int (*)(int)>() == g);
    RTTI_ASSERT(*f2.target<int (*)(int)>() == h);
    f1.swap(f2);
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(*f1.target<int (*)(int)>() == h);
    RTTI_ASSERT(*f2.target<int (*)(int)>() == g);
  }
  TC_ASSERT_EXPR(A::count == 0);
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
  {
    std::function<int(int)> f1 = A(1);
    TC_ASSERT_EXPR(A::count == 1);
    {
      DisableAllocationGuard guard;
      ((void)guard);
      f1.swap(f1);
    }
    TC_ASSERT_EXPR(A::count == 1);
    RTTI_ASSERT(f1.target<A>()->id() == 1);
  }
  TC_ASSERT_EXPR(A::count == 0);
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
  {
    std::function<int()> f1 = g0;
    DisableAllocationGuard guard;
    ((void)guard);
    f1.swap(f1);
    RTTI_ASSERT(*f1.target<int (*)()>() == g0);
  }
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
  {
    std::function<int(int, int)> f1 = g2;
    DisableAllocationGuard guard;
    ((void)guard);
    f1.swap(f1);
    RTTI_ASSERT(*f1.target<int (*)(int, int)>() == g2);
  }
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
  {
    std::function<int(int, int, int)> f1 = g3;
    DisableAllocationGuard guard;
    ((void)guard);
    f1.swap(f1);
    RTTI_ASSERT(*f1.target<int (*)(int, int, int)>() == g3);
  }
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
  {
    std::function<int()> f1 = A(1);
    TC_ASSERT_EXPR(A::count == 1);
    DisableAllocationGuard guard;
    ((void)guard);
    f1.swap(f1);
    TC_ASSERT_EXPR(A::count == 1);
    RTTI_ASSERT(f1.target<A>()->id() == 1);
  }
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
  TC_ASSERT_EXPR(A::count == 0);
  {
    std::function<int(int, int)> f1 = A(2);
    TC_ASSERT_EXPR(A::count == 1);
    DisableAllocationGuard guard;
    ((void)guard);
    f1.swap(f1);
    TC_ASSERT_EXPR(A::count == 1);
    RTTI_ASSERT(f1.target<A>()->id() == 2);
  }
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
  TC_ASSERT_EXPR(A::count == 0);
  {
    std::function<int(int, int, int)> f1 = A(3);
    TC_ASSERT_EXPR(A::count == 1);
    DisableAllocationGuard guard;
    ((void)guard);
    f1.swap(f1);
    TC_ASSERT_EXPR(A::count == 1);
    RTTI_ASSERT(f1.target<A>()->id() == 3);
  }
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
  TC_ASSERT_EXPR(A::count == 0);

  return 0;
}
