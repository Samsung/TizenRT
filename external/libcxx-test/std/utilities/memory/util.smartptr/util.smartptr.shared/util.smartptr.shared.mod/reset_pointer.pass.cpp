//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// shared_ptr

// template<class Y> void reset(Y* p);

#include <memory>
#include <cassert>

#include "reset_helper.h"
#include "test_macros.h"
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

struct Derived : A {};

static_assert( HasReset<std::shared_ptr<int>,  int*>::value, "");
static_assert( HasReset<std::shared_ptr<A>,  Derived*>::value, "");
static_assert(!HasReset<std::shared_ptr<A>,  int*>::value, "");

#if TEST_STD_VER >= 17
static_assert( HasReset<std::shared_ptr<int[]>,  int*>::value, "");
static_assert(!HasReset<std::shared_ptr<int[]>,  int(*)[]>::value, "");
static_assert( HasReset<std::shared_ptr<int[5]>, int*>::value, "");
static_assert(!HasReset<std::shared_ptr<int[5]>, int(*)[5]>::value, "");
#endif

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset_pointer(void) {
    {
        std::shared_ptr<B> p(new B);
        A* ptr = new A;
        p.reset(ptr);
        TC_ASSERT_EXPR(A::count == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(p.use_count() == 1);
        TC_ASSERT_EXPR(p.get() == ptr);
    }
    TC_ASSERT_EXPR(A::count == 0);
    {
        std::shared_ptr<B> p;
        A* ptr = new A;
        p.reset(ptr);
        TC_ASSERT_EXPR(A::count == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(p.use_count() == 1);
        TC_ASSERT_EXPR(p.get() == ptr);
    }
    TC_ASSERT_EXPR(A::count == 0);

#if TEST_STD_VER > 14
    {
        std::shared_ptr<const A[]> p;
        A* ptr = new A[8];
        p.reset(ptr);
        TC_ASSERT_EXPR(A::count == 8);
        TC_ASSERT_EXPR(p.use_count() == 1);
        TC_ASSERT_EXPR(p.get() == ptr);
    }
    TC_ASSERT_EXPR(A::count == 0);
#endif

  return 0;
}
