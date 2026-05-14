//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <functional>

// class function<R(ArgTypes...)>

// function& operator=(const function& f);

#include <cassert>
#include <functional>
#include <utility>

#include "test_macros.h"
#include "count_new.h"
#include "libcxx_tc_common.h"

class A {
  int data_[10];

public:
  static int count;

  A() {
    ++count;
    for (int i = 0; i < 10; ++i)
      data_[i] = i;
  }

  A(const A &) { ++count; }

  ~A() { --count; }

  int operator()(int i) const {
    for (int j = 0; j < 10; ++j)
      i += data_[j];
    return i;
  }
};

int A::count = 0;

int g0() { return 0; }
int g(int) { return 0; }
int g2(int, int) { return 2; }
int g3(int, int, int) { return 3; }

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_copy_assign(void) {
  globalMemCounter.reset();
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
  {
    std::function<int(int)> f = A();
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
    RTTI_ASSERT(f.target<A>());
    RTTI_ASSERT(f.target<int (*)(int)>() == 0);
    std::function<int(int)> f2;
    f2 = f;
    TC_ASSERT_EXPR(A::count == 2);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(2));
    RTTI_ASSERT(f2.target<A>());
    RTTI_ASSERT(f2.target<int (*)(int)>() == 0);
  }
  TC_ASSERT_EXPR(A::count == 0);
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
  {
    std::function<int(int)> f = g;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<int (*)(int)>());
    RTTI_ASSERT(f.target<A>() == 0);
    std::function<int(int)> f2;
    f2 = f;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f2.target<int (*)(int)>());
    RTTI_ASSERT(f2.target<A>() == 0);
  }
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
  {
    std::function<int(int)> f;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<int (*)(int)>() == 0);
    RTTI_ASSERT(f.target<A>() == 0);
    std::function<int(int)> f2;
    f2 = f;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f2.target<int (*)(int)>() == 0);
    RTTI_ASSERT(f2.target<A>() == 0);
  }
  {
    typedef std::function<int()> Func;
    Func f = g0;
    Func& fr = (f = (Func &)f);
    TC_ASSERT_EXPR(&fr == &f);
    RTTI_ASSERT(*f.target<int(*)()>() == g0);
  }
  {
    typedef std::function<int(int)> Func;
    Func f = g;
    Func& fr = (f = (Func &)f);
    TC_ASSERT_EXPR(&fr == &f);
    RTTI_ASSERT(*f.target<int(*)(int)>() == g);
  }
  {
    typedef std::function<int(int, int)> Func;
    Func f = g2;
    Func& fr = (f = (Func &)f);
    TC_ASSERT_EXPR(&fr == &f);
    RTTI_ASSERT(*f.target<int(*)(int, int)>() == g2);
  }
  {
    typedef std::function<int(int, int, int)> Func;
    Func f = g3;
    Func& fr = (f = (Func &)f);
    TC_ASSERT_EXPR(&fr == &f);
    RTTI_ASSERT(*f.target<int(*)(int, int, int)>() == g3);
  }
#if TEST_STD_VER >= 11
  TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
  {
    std::function<int(int)> f = A();
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
    RTTI_ASSERT(f.target<A>());
    RTTI_ASSERT(f.target<int (*)(int)>() == 0);
    std::function<int(int)> f2;
    f2 = std::move(f);
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
    RTTI_ASSERT(f2.target<A>());
    RTTI_ASSERT(f2.target<int (*)(int)>() == 0);
    RTTI_ASSERT(f.target<A>() == 0);
    RTTI_ASSERT(f.target<int (*)(int)>() == 0);
  }
#endif

  return 0;
}
