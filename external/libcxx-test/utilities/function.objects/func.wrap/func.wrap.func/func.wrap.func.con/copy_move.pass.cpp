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
// FIXME: In MSVC mode, even "std::function<int(int)> f(aref);" causes
// allocations.
// XFAIL: target=x86_64-pc-windows-msvc && stdlib=libc++ && libcpp-abi-version=1

// UNSUPPORTED: c++03

// <functional>

// class function<R(ArgTypes...)>

// function(const function&  f);
// function(function&& f); // noexcept in C++20

#include <functional>
#include <memory>
#include <cstdlib>
#include <cassert>
#include <utility>

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
};

int A::count = 0;

int g(int) {return 0;}

int tc_utilities_function_objects_func_wrap_func_wrap_func_func_wrap_func_con_copy_move(void) {
    globalMemCounter.reset();
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
    std::function<int(int)> f = A();
    TC_ASSERT_EXPR(A::count == 1);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
    RTTI_ASSERT(f.target<A>());
    RTTI_ASSERT(f.target<int(*)(int)>() == 0);
    std::function<int(int)> f2 = f;
    TC_ASSERT_EXPR(A::count == 2);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(2));
    RTTI_ASSERT(f2.target<A>());
    RTTI_ASSERT(f2.target<int(*)(int)>() == 0);
    }
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
    std::function<int(int)> f = g;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<int(*)(int)>());
    RTTI_ASSERT(f.target<A>() == 0);
    std::function<int(int)> f2 = f;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f2.target<int(*)(int)>());
    RTTI_ASSERT(f2.target<A>() == 0);
    }
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
    std::function<int(int)> f;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<int(*)(int)>() == 0);
    RTTI_ASSERT(f.target<A>() == 0);
    std::function<int(int)> f2 = f;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f2.target<int(*)(int)>() == 0);
    RTTI_ASSERT(f2.target<A>() == 0);
    }
    {
    std::function<int(int)> f;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(f.target<int(*)(int)>() == 0);
    RTTI_ASSERT(f.target<A>() == 0);
    TC_ASSERT_EXPR(!f);
    std::function<long(int)> g = f;
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    RTTI_ASSERT(g.target<long(*)(int)>() == 0);
    RTTI_ASSERT(g.target<A>() == 0);
    TC_ASSERT_EXPR(!g);
    }
#if TEST_STD_VER >= 11
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    { // Test rvalue references
        std::function<int(int)> f = A();
        TC_ASSERT_EXPR(A::count == 1);
        TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
        RTTI_ASSERT(f.target<A>());
        RTTI_ASSERT(f.target<int(*)(int)>() == 0);
        LIBCPP_ASSERT_NOEXCEPT(std::function<int(int)>(std::move(f)));
#if TEST_STD_VER > 17
        ASSERT_NOEXCEPT(std::function<int(int)>(std::move(f)));
#endif
        std::function<int(int)> f2 = std::move(f);
        TC_ASSERT_EXPR(A::count == 1);
        TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(1));
        RTTI_ASSERT(f2.target<A>());
        RTTI_ASSERT(f2.target<int(*)(int)>() == 0);
        RTTI_ASSERT(f.target<A>() == 0);
        RTTI_ASSERT(f.target<int(*)(int)>() == 0);
    }
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
    {
        // Test that moving a function constructed from a reference wrapper
        // is done without allocating.
        DisableAllocationGuard g;
        using Ref = std::reference_wrapper<A>;
        A a;
        Ref aref(a);
        std::function<int(int)> f(aref);
        TC_ASSERT_EXPR(A::count == 1);
        RTTI_ASSERT(f.target<A>() == nullptr);
        RTTI_ASSERT(f.target<Ref>());
        LIBCPP_ASSERT_NOEXCEPT(std::function<int(int)>(std::move(f)));
#if TEST_STD_VER > 17
        ASSERT_NOEXCEPT(std::function<int(int)>(std::move(f)));
#endif
        std::function<int(int)> f2(std::move(f));
        TC_ASSERT_EXPR(A::count == 1);
        RTTI_ASSERT(f2.target<A>() == nullptr);
        RTTI_ASSERT(f2.target<Ref>());
#if defined(_LIBCPP_VERSION)
        RTTI_ASSERT(f.target<Ref>()); // f is unchanged because the target is small
#endif
    }
    {
        // Test that moving a function constructed from a function pointer
        // is done without allocating
        DisableAllocationGuard guard;
        using Ptr = int(*)(int);
        Ptr p = g;
        std::function<int(int)> f(p);
        RTTI_ASSERT(f.target<A>() == nullptr);
        RTTI_ASSERT(f.target<Ptr>());
        LIBCPP_ASSERT_NOEXCEPT(std::function<int(int)>(std::move(f)));
#if TEST_STD_VER > 17
        ASSERT_NOEXCEPT(std::function<int(int)>(std::move(f)));
#endif
        std::function<int(int)> f2(std::move(f));
        RTTI_ASSERT(f2.target<A>() == nullptr);
        RTTI_ASSERT(f2.target<Ptr>());
#if defined(_LIBCPP_VERSION)
        RTTI_ASSERT(f.target<Ptr>()); // f is unchanged because the target is small
#endif
    }
#endif // TEST_STD_VER >= 11

  return 0;
}
