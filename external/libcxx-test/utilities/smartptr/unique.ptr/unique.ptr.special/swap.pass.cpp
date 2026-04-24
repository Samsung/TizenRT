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

// unique_ptr

// Test swap

#include <memory>
#include <cassert>

#include "test_macros.h"
#include "deleter_types.h"
#include "libcxx_tc_common.h"

struct A
{
    int state_;
    static int count;
    TEST_CONSTEXPR_CXX23 A() : state_(0) {
      if (!TEST_IS_CONSTANT_EVALUATED)
        ++count;
    }
    TEST_CONSTEXPR_CXX23 explicit A(int i) : state_(i) {
      if (!TEST_IS_CONSTANT_EVALUATED)
        ++count;
    }
    TEST_CONSTEXPR_CXX23 A(const A& a) : state_(a.state_) {
      if (!TEST_IS_CONSTANT_EVALUATED)
        ++count;
    }
    TEST_CONSTEXPR_CXX23 A& operator=(const A& a) {
      state_ = a.state_;
      return *this;
    }
    TEST_CONSTEXPR_CXX23 ~A() {
      if (!TEST_IS_CONSTANT_EVALUATED)
        --count;
    }

    friend TEST_CONSTEXPR_CXX23 bool operator==(const A& x, const A& y) { return x.state_ == y.state_; }
};

int A::count = 0;

template <class T>
struct NonSwappableDeleter {
  TEST_CONSTEXPR_CXX23 explicit NonSwappableDeleter(int) {}
  TEST_CONSTEXPR_CXX23 NonSwappableDeleter& operator=(NonSwappableDeleter const&) { return *this; }
  TEST_CONSTEXPR_CXX23 void operator()(T*) const {}

private:
  NonSwappableDeleter(NonSwappableDeleter const&);

};

TEST_CONSTEXPR_CXX23 bool test() {
  {
    A* p1 = new A(1);
    std::unique_ptr<A, Deleter<A> > s1(p1, Deleter<A>(1));
    A* p2 = new A(2);
    std::unique_ptr<A, Deleter<A> > s2(p2, Deleter<A>(2));
    TC_ASSERT_EXPR(s1.get() == p1);
    TC_ASSERT_EXPR(*s1 == A(1));
    TC_ASSERT_EXPR(s1.get_deleter().state() == 1);
    TC_ASSERT_EXPR(s2.get() == p2);
    TC_ASSERT_EXPR(*s2 == A(2));
    TC_ASSERT_EXPR(s2.get_deleter().state() == 2);
    swap(s1, s2);
    TC_ASSERT_EXPR(s1.get() == p2);
    TC_ASSERT_EXPR(*s1 == A(2));
    TC_ASSERT_EXPR(s1.get_deleter().state() == 2);
    TC_ASSERT_EXPR(s2.get() == p1);
    TC_ASSERT_EXPR(*s2 == A(1));
    TC_ASSERT_EXPR(s2.get_deleter().state() == 1);
    if (!TEST_IS_CONSTANT_EVALUATED)
      TC_ASSERT_EXPR(A::count == 2);
  }
  if (!TEST_IS_CONSTANT_EVALUATED)
    TC_ASSERT_EXPR(A::count == 0);
  {
    A* p1 = new A[3];
    std::unique_ptr<A[], Deleter<A[]> > s1(p1, Deleter<A[]>(1));
    A* p2 = new A[3];
    std::unique_ptr<A[], Deleter<A[]> > s2(p2, Deleter<A[]>(2));
    TC_ASSERT_EXPR(s1.get() == p1);
    TC_ASSERT_EXPR(s1.get_deleter().state() == 1);
    TC_ASSERT_EXPR(s2.get() == p2);
    TC_ASSERT_EXPR(s2.get_deleter().state() == 2);
    swap(s1, s2);
    TC_ASSERT_EXPR(s1.get() == p2);
    TC_ASSERT_EXPR(s1.get_deleter().state() == 2);
    TC_ASSERT_EXPR(s2.get() == p1);
    TC_ASSERT_EXPR(s2.get_deleter().state() == 1);
    if (!TEST_IS_CONSTANT_EVALUATED)
      TC_ASSERT_EXPR(A::count == 6);
  }
  if (!TEST_IS_CONSTANT_EVALUATED)
    TC_ASSERT_EXPR(A::count == 0);
#if TEST_STD_VER >= 11
    {
        // test that unique_ptr's specialized swap is disabled when the deleter
        // is non-swappable. Instead we should pick up the generic swap(T, T)
        // and perform 3 move constructions.
        typedef NonSwappableDeleter<int> D;
        D  d(42);
        int x = 42;
        int y = 43;
        std::unique_ptr<int, D&> p(&x, d);
        std::unique_ptr<int, D&> p2(&y, d);
        std::swap(p, p2);
    }
#endif

    return true;
}

int tc_utilities_smartptr_unique_ptr_unique_ptr_special_swap(void) {
  test();
#if TEST_STD_VER >= 23
  static_assert(test());
#endif

  return 0;
}
