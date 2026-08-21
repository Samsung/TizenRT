//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// shared_ptr

// template<class Y> explicit shared_ptr(const weak_ptr<Y>& r);

#include <cassert>
#include <memory>
#include <type_traits>

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

// https://llvm.org/PR60258
// Invalid constructor SFINAE for std::shared_ptr's array ctors
static_assert(!std::is_constructible<std::shared_ptr<int>,     const std::weak_ptr<long>&>::value, "");
static_assert( std::is_constructible<std::shared_ptr<B>,       const std::weak_ptr<A>&>::value, "");
static_assert( std::is_constructible<std::shared_ptr<const A>, const std::weak_ptr<A>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<A>,       const std::weak_ptr<const A>&>::value, "");

#if TEST_STD_VER >= 17
static_assert(!std::is_constructible<std::shared_ptr<int>,     const std::weak_ptr<int[]>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int>,     const std::weak_ptr<int[5]>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[]>,   const std::weak_ptr<int>&>::value, "");
static_assert( std::is_constructible<std::shared_ptr<int[]>,   const std::weak_ptr<int[5]>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[5]>,  const std::weak_ptr<int>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[5]>,  const std::weak_ptr<int[]>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[7]>,  const std::weak_ptr<int[5]>&>::value, "");
#endif

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_weak_ptr(void) {
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        std::weak_ptr<A> wp;
        try
        {
            std::shared_ptr<A> sp(wp);
            TC_ASSERT_EXPR(false);
        }
        catch (std::bad_weak_ptr&)
        {
        }
        TC_ASSERT_EXPR(A::count == 0);
    }
#endif
    {
        std::shared_ptr<A> sp0(new A);
        std::weak_ptr<A> wp(sp0);
        std::shared_ptr<A> sp(wp);
        TC_ASSERT_EXPR(sp.use_count() == 2);
        TC_ASSERT_EXPR(sp.get() == sp0.get());
        TC_ASSERT_EXPR(A::count == 1);
    }
    TC_ASSERT_EXPR(A::count == 0);
    {
        std::shared_ptr<A const> sp0(new A);
        std::weak_ptr<A const> wp(sp0);
        std::shared_ptr<A const> sp(wp);
        TC_ASSERT_EXPR(sp.use_count() == 2);
        TC_ASSERT_EXPR(sp.get() == sp0.get());
        TC_ASSERT_EXPR(A::count == 1);
    }
    TC_ASSERT_EXPR(A::count == 0);
#ifndef TEST_HAS_NO_EXCEPTIONS
    {
        std::shared_ptr<A> sp0(new A);
        std::weak_ptr<A> wp(sp0);
        sp0.reset();
        try
        {
            std::shared_ptr<A> sp(wp);
            TC_ASSERT_EXPR(false);
        }
        catch (std::bad_weak_ptr&)
        {
        }
    }
    TC_ASSERT_EXPR(A::count == 0);
#endif

#if TEST_STD_VER > 14
    {
        std::shared_ptr<A[]> sp0(new A[8]);
        std::weak_ptr<A[]> wp(sp0);
        std::shared_ptr<const A[]> sp(wp);
        TC_ASSERT_EXPR(sp.use_count() == 2);
        TC_ASSERT_EXPR(sp.get() == sp0.get());
        TC_ASSERT_EXPR(A::count == 8);
    }
    TC_ASSERT_EXPR(A::count == 0);
#endif

  return 0;
}
