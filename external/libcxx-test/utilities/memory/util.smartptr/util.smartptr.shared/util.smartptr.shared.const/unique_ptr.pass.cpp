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
// UNSUPPORTED: sanitizer-new-delete

// <memory>

// template <class Y, class D> shared_ptr(unique_ptr<Y, D>&&r);

#include <memory>
#include <new>
#include <cstdlib>
#include <cassert>
#include <utility>

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

void fn ( const std::shared_ptr<int> &) {}
void fn ( const std::shared_ptr<B> &) { TC_ASSERT_EXPR(false); }

template <typename T>
void assert_deleter ( T * ) { TC_ASSERT_EXPR(false); }

namespace adl {
struct D {
    void operator()(int *) const {}
};
void ref(D);
}

template <class T>
struct StatefulDeleter {
  int state = 0;

  StatefulDeleter(int val = 0) : state(val) {}
  StatefulDeleter(StatefulDeleter const&) { TC_ASSERT_EXPR(false); }

  void operator()(T* ptr) {
    TC_ASSERT_EXPR(state == 42);
    delete ptr;
  }
};

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

struct MovingDeleter {
  explicit MovingDeleter(int *moves) : moves_(moves) {}
  MovingDeleter(MovingDeleter&& rhs) : moves_(rhs.moves_) { *moves_ += 1; }
  void operator()(int*) const {}
  int *moves_;
};

// https://llvm.org/PR53368
// Bogus unique_ptr-to-shared_ptr conversions should be forbidden
#if TEST_STD_VER >= 17
static_assert( std::is_constructible<std::shared_ptr<A>,   std::unique_ptr<A>&&>::value, "");
static_assert( std::is_constructible<std::shared_ptr<A[]>, std::unique_ptr<A[]>&&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<A>,   std::unique_ptr<A[]>&&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<B[]>, std::unique_ptr<A[]>&&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<B>,   std::unique_ptr<A[]>&&>::value, "");
#endif

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_unique_ptr(void) {
    {
        std::unique_ptr<A> ptr(new A);
        A* raw_ptr = ptr.get();
        std::shared_ptr<B> p(std::move(ptr));
        TC_ASSERT_EXPR(A::count == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(p.use_count() == 1);
        TC_ASSERT_EXPR(p.get() == raw_ptr);
        TC_ASSERT_EXPR(ptr.get() == 0);
    }

    {
        std::unique_ptr<A const> ptr(new A);
        A const* raw_ptr = ptr.get();
        std::shared_ptr<B const> p(std::move(ptr));
        TC_ASSERT_EXPR(A::count == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(p.use_count() == 1);
        TC_ASSERT_EXPR(p.get() == raw_ptr);
        TC_ASSERT_EXPR(ptr.get() == 0);
    }

#ifndef TEST_HAS_NO_EXCEPTIONS
    TC_ASSERT_EXPR(A::count == 0);
    {
        std::unique_ptr<A> ptr(new A);
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
#endif

#if TEST_STD_VER > 14
    {
      std::unique_ptr<int> ptr;
      std::shared_ptr<int> p(std::move(ptr));
      TC_ASSERT_EXPR(p.get() == 0);
      TC_ASSERT_EXPR(p.use_count() == 0);
    }
#endif

    {
      StatefulDeleter<A> d;
      std::unique_ptr<A, StatefulDeleter<A>&> u(new A, d);
      std::shared_ptr<A> p(std::move(u));
      d.state = 42;
      TC_ASSERT_EXPR(A::count == 1);
    }
    TC_ASSERT_EXPR(A::count == 0);

    { // LWG 2399
        fn(std::unique_ptr<int>(new int));
    }
#if TEST_STD_VER >= 14
    { // LWG 2415
        std::unique_ptr<int, void (*)(int*)> p(nullptr, assert_deleter<int>);
        std::shared_ptr<int> p2(std::move(p)); // should not call deleter when going out of scope
    }
#endif

    {
    adl::D d;
    std::unique_ptr<int, adl::D&> u(nullptr, d);
    std::shared_ptr<int> s = std::move(u);
    }

    TC_ASSERT_EXPR(A::count == 0);

#if TEST_STD_VER > 14
    {
      StatefulArrayDeleter<A> d;
      std::unique_ptr<A[], StatefulArrayDeleter<A>&> u(new A[4], d);
      std::shared_ptr<A[]> p(std::move(u));
      d.state = 42;
      TC_ASSERT_EXPR(A::count == 4);
    }
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(B::count == 0);

    {
      std::unique_ptr<A[]> ptr(new A[8]);
      A* raw_ptr = ptr.get();
      std::shared_ptr<A[]> p(std::move(ptr));
      TC_ASSERT_EXPR(A::count == 8);
      TC_ASSERT_EXPR(p.use_count() == 1);
      TC_ASSERT_EXPR(p.get() == raw_ptr);
      TC_ASSERT_EXPR(ptr.get() == 0);
    }
    TC_ASSERT_EXPR(A::count == 0);

    {
      int *p = new int[8];
      std::unique_ptr<int[]> u(p);
      std::shared_ptr<int[]> s(std::move(u));
      TC_ASSERT_EXPR(u == nullptr);
      TC_ASSERT_EXPR(s.get() == p);
    }
#endif // TEST_STD_VER > 14

    { // LWG 3548
      {
        int moves = 0;
        int i = 42;
        std::unique_ptr<int, MovingDeleter> u(&i, MovingDeleter(&moves));
        TC_ASSERT_EXPR(moves == 1);
        std::shared_ptr<int> s(std::move(u));
        TC_ASSERT_EXPR(moves >= 2);
        TC_ASSERT_EXPR(u == nullptr);
        TC_ASSERT_EXPR(s.get() == &i);
      }

#if TEST_STD_VER > 14
      {
        int moves = 0;
        int a[8];
        std::unique_ptr<int[], MovingDeleter> u(a, MovingDeleter(&moves));
        TC_ASSERT_EXPR(moves == 1);
        std::shared_ptr<int[]> s = std::move(u);
        TC_ASSERT_EXPR(moves >= 2);
        TC_ASSERT_EXPR(u == nullptr);
        TC_ASSERT_EXPR(s.get() == a);
      }
#endif // TEST_STD_VER > 14
    }

    return 0;
}
