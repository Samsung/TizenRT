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

// template<class Y> shared_ptr& operator=(const shared_ptr<Y>& r);

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

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_shared_ptr_Y(void) {
    {
        const std::shared_ptr<A> pA(new A);
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB(new B);
            pB = pA;
            TC_ASSERT_EXPR(B::count == 1);
            TC_ASSERT_EXPR(A::count == 1);
            TC_ASSERT_EXPR(pB.use_count() == 2);
            TC_ASSERT_EXPR(pA.use_count() == 2);
            TC_ASSERT_EXPR(pA.get() == pB.get());
            TC_ASSERT_EXPR(pB.get() == ptrA);
        }
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(A::count == 1);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);
    {
        const std::shared_ptr<A> pA;
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB(new B);
            pB = pA;
            TC_ASSERT_EXPR(B::count == 0);
            TC_ASSERT_EXPR(A::count == 0);
            TC_ASSERT_EXPR(pB.use_count() == 0);
            TC_ASSERT_EXPR(pA.use_count() == 0);
            TC_ASSERT_EXPR(pA.get() == pB.get());
            TC_ASSERT_EXPR(pB.get() == ptrA);
        }
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);
    {
        const std::shared_ptr<A> pA(new A);
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB;
            pB = pA;
            TC_ASSERT_EXPR(B::count == 1);
            TC_ASSERT_EXPR(A::count == 1);
            TC_ASSERT_EXPR(pB.use_count() == 2);
            TC_ASSERT_EXPR(pA.use_count() == 2);
            TC_ASSERT_EXPR(pA.get() == pB.get());
            TC_ASSERT_EXPR(pB.get() == ptrA);
        }
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(A::count == 1);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);
    {
        const std::shared_ptr<A> pA;
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB;
            pB = pA;
            TC_ASSERT_EXPR(B::count == 0);
            TC_ASSERT_EXPR(A::count == 0);
            TC_ASSERT_EXPR(pB.use_count() == 0);
            TC_ASSERT_EXPR(pA.use_count() == 0);
            TC_ASSERT_EXPR(pA.get() == pB.get());
            TC_ASSERT_EXPR(pB.get() == ptrA);
        }
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);

#if TEST_STD_VER > 14
    {
        std::shared_ptr<A[]> p1(new A[8]);
        A* ptr = p1.get();
        TC_ASSERT_EXPR(A::count == 8);
        {
            std::shared_ptr<const A[]> p2;
            p2 = p1;
            TC_ASSERT_EXPR(A::count == 8);
            TC_ASSERT_EXPR(p2.use_count() == 2);
            TC_ASSERT_EXPR(p1.use_count() == 2);
            TC_ASSERT_EXPR(p1.get() == p2.get());
            TC_ASSERT_EXPR(p2.get() == ptr);
        }
        TC_ASSERT_EXPR(p1.use_count() == 1);
        TC_ASSERT_EXPR(A::count == 8);
    }
    TC_ASSERT_EXPR(A::count == 0);
#endif

  return 0;
}
