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

// function& operator=(nullptr_t);

#include <functional>
#include <cassert>

#include "count_new.h"

#include "test_macros.h"
#include "libcxx_tc_common.h"

class A
{
    int data_[10];
public:
    static int count;

    A()
    {
        ++count;
        for (int i = 0; i < 10; ++i)
            data_[i] = i;
    }

    A(const A&) {++count;}

    ~A() {--count;}

    int operator()(int i) const
    {
        for (int j = 0; j < 10; ++j)
            i += data_[j];
        return i;
    }
};

int A::count = 0;

int g(int) {return 0;}

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_nullptr_t_assign(void) {
    globalMemCounter.reset();
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
    std::function<int(int)> f = A();
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
    RTTI_ASSERT(f.target<A>());
    f = nullptr;
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<A>() == 0);
    }
    {
    std::function<int(int)> f = g;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<int(*)(int)>());
    RTTI_ASSERT(f.target<A>() == 0);
    f = nullptr;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<int(*)(int)>() == 0);
    }

  return 0;
}
