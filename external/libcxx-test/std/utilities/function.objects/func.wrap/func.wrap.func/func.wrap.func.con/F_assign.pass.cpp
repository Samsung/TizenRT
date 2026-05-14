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

// template<class F>
//   requires CopyConstructible<F> && Callable<F, ArgTypes..>
//         && Convertible<Callable<F, ArgTypes...>::result_type
//   operator=(F f);

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

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_F_assign(void) {
    globalMemCounter.reset();
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
    std::function<int(int)> f;
    f = A();
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
    RTTI_ASSERT(f.target<A>());
    RTTI_ASSERT(f.target<int(*)(int)>() == 0);
    }
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
    std::function<int(int)> f;
    f = g;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<int(*)(int)>());
    RTTI_ASSERT(f.target<A>() == 0);
    }
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
    std::function<int(int)> f;
    f = (int (*)(int))0;
    TC_ASSERT_EXPR(!f);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<int(*)(int)>() == 0);
    RTTI_ASSERT(f.target<A>() == 0);
    }
    {
    std::function<int(const A*, int)> f;
    f = &A::foo;
    TC_ASSERT_EXPR(f);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<int (A::*)(int) const>() != 0);
    }
    {
    std::function<void(int)> f;
    f = &g;
    TC_ASSERT_EXPR(f);
    RTTI_ASSERT(f.target<int(*)(int)>() != 0);
    f(1);
    }
#if TEST_STD_VER >= 11
    {
        using Fn = std::function<void(int, int, int)>;
        static_assert(std::is_assignable<Fn&, LValueCallable&>::value, "");
        static_assert(std::is_assignable<Fn&, LValueCallable>::value, "");
        static_assert(!std::is_assignable<Fn&, RValueCallable&>::value, "");
        static_assert(!std::is_assignable<Fn&, RValueCallable>::value, "");
    }
    {
        using Fn = std::function<void(int, int, int)>;
        static_assert(std::is_assignable<Fn&, Fn&&>::value, "");
    }
    {
        using F1 = std::function<void(int, int)>;
        using F2 = std::function<void(int, int, int)>;
        static_assert(!std::is_assignable<F1&, F2&&>::value, "");
    }
    {
        using F1 = std::function<int(int, int)>;
        using F2 = std::function<A  (int, int)>;
        static_assert(!std::is_assignable<F1&, F2&&>::value, "");
        static_assert(!std::is_assignable<F2&, F1&&>::value, "");
    }
#endif

  return 0;
}
