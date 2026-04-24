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

// template <class Y, class D> shared_ptr& operator=(unique_ptr<Y, D>&& r);

#include <memory>
#include <type_traits>
#include <cassert>
#include <utility>

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

template <class T>
struct StatefulArrayDeleter {
  int state = 0;

  StatefulArrayDeleter(int val = 0) : state(val) {}
  StatefulArrayDeleter(StatefulArrayDeleter const&) { TC_ASSERT_EXPR(false); }

  void operator()(T* ptr) {
    TC_ASSERT_EXPR(state == 42);
    delete []ptr;
  }
};

// https://llvm.org/PR53368
// Bogus unique_ptr-to-shared_ptr conversions should be forbidden
#if TEST_STD_VER >= 17
static_assert( std::is_assignable<std::shared_ptr<A>&,   std::unique_ptr<A>&&>::value, "");
static_assert( std::is_assignable<std::shared_ptr<A[]>&, std::unique_ptr<A[]>&&>::value, "");
static_assert(!std::is_assignable<std::shared_ptr<A>&,   std::unique_ptr<A[]>&&>::value, "");
static_assert(!std::is_assignable<std::shared_ptr<B[]>&, std::unique_ptr<A[]>&&>::value, "");
static_assert(!std::is_assignable<std::shared_ptr<B>&,   std::unique_ptr<A[]>&&>::value, "");
#endif

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_assign_unique_ptr_Y(void) {
    {
        std::unique_ptr<A> pA(new A);
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB(new B);
            pB = std::move(pA);
            TC_ASSERT_EXPR(B::count == 1);
            TC_ASSERT_EXPR(A::count == 1);
            TC_ASSERT_EXPR(pB.use_count() == 1);
            TC_ASSERT_EXPR(pA.get() == 0);
            TC_ASSERT_EXPR(pB.get() == ptrA);
        }
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);
    {
        std::unique_ptr<A> pA;
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB(new B);
            pB = std::move(pA);
            TC_ASSERT_EXPR(B::count == 0);
            TC_ASSERT_EXPR(A::count == 0);
//          TC_ASSERT_EXPR(pB.use_count() == 1); // no longer true due to LWG 2415
            TC_ASSERT_EXPR(pA.get() == 0);
            TC_ASSERT_EXPR(pB.get() == ptrA);
        }
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);
    {
        std::unique_ptr<A> pA(new A);
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB;
            pB = std::move(pA);
            TC_ASSERT_EXPR(B::count == 1);
            TC_ASSERT_EXPR(A::count == 1);
            TC_ASSERT_EXPR(pB.use_count() == 1);
            TC_ASSERT_EXPR(pA.get() == 0);
            TC_ASSERT_EXPR(pB.get() == ptrA);
        }
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);
    {
        std::unique_ptr<A> pA;
        A* ptrA = pA.get();
        {
            std::shared_ptr<B> pB;
            pB = std::move(pA);
            TC_ASSERT_EXPR(B::count == 0);
            TC_ASSERT_EXPR(A::count == 0);
//          TC_ASSERT_EXPR(pB.use_count() == 1); // no longer true due to LWG 2415
            TC_ASSERT_EXPR(pA.get() == 0);
            TC_ASSERT_EXPR(pB.get() == ptrA);
        }
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);

#if TEST_STD_VER > 14
    {
      StatefulArrayDeleter<A> d;
      std::unique_ptr<A[], StatefulArrayDeleter<A>&> u(new A[4], d);
      std::shared_ptr<A[]> p;
      p = std::move(u);
      d.state = 42;
      TC_ASSERT_EXPR(A::count == 4);
    }
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(B::count == 0);

    {
      std::unique_ptr<A[]> ptr(new A[8]);
      A* raw_ptr = ptr.get();
      std::shared_ptr<A[]> p;
      p = std::move(ptr);
      TC_ASSERT_EXPR(A::count == 8);
      TC_ASSERT_EXPR(p.use_count() == 1);
      TC_ASSERT_EXPR(p.get() == raw_ptr);
      TC_ASSERT_EXPR(ptr.get() == 0);
    }
    TC_ASSERT_EXPR(A::count == 0);

    {
      std::unique_ptr<int[]> ptr(new int[8]);
      std::shared_ptr<int[]> p;
      p = std::move(ptr);
    }
#endif // TEST_STD_VER >= 14

  return 0;
}
