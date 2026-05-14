//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// template<class Y> explicit shared_ptr(auto_ptr<Y>&& r);

// REQUIRES: c++03 || c++11 || c++14
// ADDITIONAL_COMPILE_FLAGS: -D_LIBCPP_DISABLE_DEPRECATION_WARNINGS

#include <memory>
#include <new>
#include <cstdlib>
#include <cassert>

#include "test_macros.h"
#include "count_new.h"
#include "libcxx_tc_common.h"

struct B
{
    static int count;

    B() {++count;}
    B(const B&) {++count;}
    virtual ~B() {--count;}
};

int B::count = 0;

struct A
    : public B
{
    static int count;

    A() {++count;}
    A(const A& other) : B(other) {++count;}
    ~A() {--count;}
};

int A::count = 0;

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_auto_ptr(void) {
    globalMemCounter.reset();
    {
        std::auto_ptr<A> ptr(new A);
        A* raw_ptr = ptr.get();
        std::shared_ptr<B> p(std::move(ptr));
        TC_ASSERT_EXPR(A::count == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(p.use_count() == 1);
        TC_ASSERT_EXPR(p.get() == raw_ptr);
        TC_ASSERT_EXPR(ptr.get() == 0);
    }
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));

    globalMemCounter.reset();
    {
        std::auto_ptr<A const> ptr(new A);
        A const* raw_ptr = ptr.get();
        std::shared_ptr<B const> p(std::move(ptr));
        TC_ASSERT_EXPR(A::count == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(p.use_count() == 1);
        TC_ASSERT_EXPR(p.get() == raw_ptr);
        TC_ASSERT_EXPR(ptr.get() == 0);
    }
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));

#if !defined(TEST_HAS_NO_EXCEPTIONS) && !defined(DISABLE_NEW_COUNT)
    {
        std::auto_ptr<A> ptr(new A);
        A* raw_ptr = ptr.get();
        globalMemCounter.throw_after = 0;
        try
        {
            std::shared_ptr<B> p(std::move(ptr));
            TC_ASSERT_EXPR(false);
        }
        catch (...)
        {
            TC_ASSERT_EXPR(A::count == 1);
            TC_ASSERT_EXPR(B::count == 1);
            TC_ASSERT_EXPR(ptr.get() == raw_ptr);
        }
    }
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));
#endif // !defined(TEST_HAS_NO_EXCEPTIONS) && !defined(DISABLE_NEW_COUNT)

  return 0;
}
