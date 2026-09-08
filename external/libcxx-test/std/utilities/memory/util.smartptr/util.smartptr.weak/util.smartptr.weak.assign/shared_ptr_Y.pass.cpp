//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// weak_ptr

// template<class Y> weak_ptr& operator=(const shared_ptr<Y>& r);

#include <memory>
#include <type_traits>
#include <cassert>

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

int tc_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_assign_shared_ptr_Y(void) {
    {
        const std::shared_ptr<A> pA(new A);
        TC_ASSERT_EXPR(pA.use_count() == 1);
        {
            std::weak_ptr<B> pB;
            pB = pA;
            TC_ASSERT_EXPR(B::count == 1);
            TC_ASSERT_EXPR(A::count == 1);
            TC_ASSERT_EXPR(pB.use_count() == 1);
            TC_ASSERT_EXPR(pA.use_count() == 1);
        }
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(A::count == 1);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);

#if TEST_STD_VER > 14
    {
        const std::shared_ptr<A[]> p1(new A[8]);
        TC_ASSERT_EXPR(p1.use_count() == 1);
        {
            std::weak_ptr<const A[]> p2;
            p2 = p1;
            TC_ASSERT_EXPR(A::count == 8);
            TC_ASSERT_EXPR(p2.use_count() == 1);
            TC_ASSERT_EXPR(p1.use_count() == 1);
        }
        TC_ASSERT_EXPR(p1.use_count() == 1);
        TC_ASSERT_EXPR(A::count == 8);
    }
    TC_ASSERT_EXPR(A::count == 0);
#endif

  return 0;
}
