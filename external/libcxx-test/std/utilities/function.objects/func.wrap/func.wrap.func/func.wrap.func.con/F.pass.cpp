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

// function(F);

#include <functional>
#include <cassert>

#include "test_macros.h"
#include "count_new.h"
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

    int foo(int) const {return 1;}
};

int A::count = 0;

int g(int) {return 0;}

#if TEST_STD_VER >= 11
struct RValueCallable {
    template <class ...Args>
    void operator()(Args&&...) && {}
};
struct LValueCallable {
    template <class ...Args>
    void operator()(Args&&...) & {}
};
#endif

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_F(void) {
    globalMemCounter.reset();
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
    std::function<int(int)> f = A();
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
    RTTI_ASSERT(f.target<A>());
    RTTI_ASSERT(f.target<int(*)(int)>() == 0);
    }
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
    std::function<int(int)> f = g;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<int(*)(int)>());
    RTTI_ASSERT(f.target<A>() == 0);
    }
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
    std::function<int(int)> f = (int (*)(int))0;
    TC_ASSERT_EXPR(!f);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<int(*)(int)>() == 0);
    RTTI_ASSERT(f.target<A>() == 0);
    }
    {
    std::function<int(const A*, int)> f = &A::foo;
    TC_ASSERT_EXPR(f);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<int (A::*)(int) const>() != 0);
    }
    {
      std::function<void(int)> f(&g);
      TC_ASSERT_EXPR(f);
      RTTI_ASSERT(f.target<int(*)(int)>() != 0);
      f(1);
    }
    {
        std::function <void()> f(static_cast<void (*)()>(0));
        TC_ASSERT_EXPR(!f);
    }
#if TEST_STD_VER >= 11
    {
        using Fn = std::function<void(int, int, int)>;
        static_assert(std::is_constructible<Fn, LValueCallable&>::value, "");
        static_assert(std::is_constructible<Fn, LValueCallable>::value, "");
        static_assert(!std::is_constructible<Fn, RValueCallable&>::value, "");
        static_assert(!std::is_constructible<Fn, RValueCallable>::value, "");
    }
#endif

  return 0;
}
