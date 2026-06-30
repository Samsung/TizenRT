//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// weak_ptr

// template<class Y> weak_ptr(const weak_ptr<Y>& r);
// template<class Y> weak_ptr(weak_ptr<Y>&& r);

#include <memory>
#include <type_traits>
#include <utility>
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

struct C
{
    static int count;

    C() {++count;}
    C(const C&) {++count;}
    virtual ~C() {--count;}
};

int C::count = 0;

template <class T>
std::weak_ptr<T> source (std::shared_ptr<T> p) { return std::weak_ptr<T>(p); }

int tc_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_const_weak_ptr_Y(void) {
    static_assert(( std::is_convertible<std::weak_ptr<A>, std::weak_ptr<B> >::value), "");
    static_assert((!std::is_convertible<std::weak_ptr<B>, std::weak_ptr<A> >::value), "");
    static_assert((!std::is_convertible<std::weak_ptr<A>, std::weak_ptr<C> >::value), "");
    {
        const std::weak_ptr<A> pA(std::shared_ptr<A>(new A));
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
        {
            std::weak_ptr<B> pB(pA);
            TC_ASSERT_EXPR(B::count == 0);
            TC_ASSERT_EXPR(A::count == 0);
            TC_ASSERT_EXPR(pB.use_count() == 0);
            TC_ASSERT_EXPR(pA.use_count() == 0);
        }
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);
    {
        std::weak_ptr<A> pA;
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
        {
            std::weak_ptr<B> pB(pA);
            TC_ASSERT_EXPR(B::count == 0);
            TC_ASSERT_EXPR(A::count == 0);
            TC_ASSERT_EXPR(pB.use_count() == 0);
            TC_ASSERT_EXPR(pA.use_count() == 0);
        }
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);

    {
        std::shared_ptr<A> ps(new A);
        std::weak_ptr<A> pA = source(ps);
        std::weak_ptr<B> pB(std::move(pA));
        TC_ASSERT_EXPR(pB.use_count() == 1);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);

#if TEST_STD_VER > 14
    {
        std::shared_ptr<A[]> ps(new A[8]);
        std::weak_ptr<A[]> p1 = source(ps);
        std::weak_ptr<const A[]> p2(p1);
        TC_ASSERT_EXPR(p2.use_count() == 1);
    }
    TC_ASSERT_EXPR(A::count == 0);

    {
        std::shared_ptr<A[]> ps(new A[8]);
        std::weak_ptr<A[]> p1 = source(ps);
        std::weak_ptr<const A[]> p2(std::move(p1));
        TC_ASSERT_EXPR(p2.use_count() == 1);
    }
    TC_ASSERT_EXPR(A::count == 0);
#endif

  return 0;
}
