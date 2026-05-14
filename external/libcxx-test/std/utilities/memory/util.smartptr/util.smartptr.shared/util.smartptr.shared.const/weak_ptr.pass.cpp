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
