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

// <memory>

// shared_ptr

// template<class Y> shared_ptr(shared_ptr<Y>&& r);

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

// https://llvm.org/PR60258
// Invalid constructor SFINAE for std::shared_ptr's array ctors
static_assert(!std::is_constructible<std::shared_ptr<int>,     std::shared_ptr<long>&&>::value, "");
static_assert( std::is_constructible<std::shared_ptr<B>,       std::shared_ptr<A>&&>::value, "");
static_assert( std::is_constructible<std::shared_ptr<const A>, std::shared_ptr<A>&&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<A>,       std::shared_ptr<const A>&&>::value, "");

#if TEST_STD_VER >= 17
static_assert(!std::is_constructible<std::shared_ptr<int>,     std::shared_ptr<int[]>&&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int>,     std::shared_ptr<int[5]>&&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[]>,   std::shared_ptr<int>&&>::value, "");
static_assert( std::is_constructible<std::shared_ptr<int[]>,   std::shared_ptr<int[5]>&&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[5]>,  std::shared_ptr<int>&&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[5]>,  std::shared_ptr<int[]>&&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[7]>,  std::shared_ptr<int[5]>&&>::value, "");
#endif

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_Y_rv(void) {
    static_assert(( std::is_convertible<std::shared_ptr<A>, std::shared_ptr<B> >::value), "");
    static_assert((!std::is_convertible<std::shared_ptr<B>, std::shared_ptr<A> >::value), "");
    static_assert((!std::is_convertible<std::shared_ptr<A>, std::shared_ptr<C> >::value), "");
    {
        std::shared_ptr<A> pA(new A);
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(A::count == 1);
        {
            B* p = pA.get();
            std::shared_ptr<B> pB(std::move(pA));
            TC_ASSERT_EXPR(B::count == 1);
            TC_ASSERT_EXPR(A::count == 1);
#if TEST_STD_VER >= 11
            TC_ASSERT_EXPR(pB.use_count() == 1);
            TC_ASSERT_EXPR(pA.use_count() == 0);
#else
            TC_ASSERT_EXPR(pB.use_count() == 2);
            TC_ASSERT_EXPR(pA.use_count() == 2);
#endif
            TC_ASSERT_EXPR(p == pB.get());
        }
#if TEST_STD_VER >= 11
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
#else
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(A::count == 1);
#endif
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);
    {
        std::shared_ptr<A> pA;
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
        {
            std::shared_ptr<B> pB(std::move(pA));
            TC_ASSERT_EXPR(B::count == 0);
            TC_ASSERT_EXPR(A::count == 0);
            TC_ASSERT_EXPR(pB.use_count() == 0);
            TC_ASSERT_EXPR(pA.use_count() == 0);
            TC_ASSERT_EXPR(pA.get() == pB.get());
        }
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);

#if TEST_STD_VER > 14
    {
        std::shared_ptr<A[]> p1;
        TC_ASSERT_EXPR(p1.use_count() == 0);
        TC_ASSERT_EXPR(A::count == 0);
        {
            std::shared_ptr<const A[]> p2(p1);
            TC_ASSERT_EXPR(A::count == 0);
            TC_ASSERT_EXPR(p2.use_count() == 0);
            TC_ASSERT_EXPR(p1.use_count() == 0);
            TC_ASSERT_EXPR(p1.get() == p2.get());
        }
        TC_ASSERT_EXPR(p1.use_count() == 0);
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(A::count == 0);
#endif

    {
        std::shared_ptr<A const> pA(new A);
        B const* p = pA.get();
        std::shared_ptr<B const> pB(std::move(pA));
        TC_ASSERT_EXPR(pB.get() == p);
    }

  return 0;
}
