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

// template<class Y> shared_ptr(const shared_ptr<Y>& r, T *p);

#include <memory>
#include <cassert>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct B
{
    static int count;

    B() {++count;}
    B(const B&) {++count;}
    ~B() {--count;}
};

int B::count = 0;

struct A
{
    static int count;

    A() {++count;}
    A(const A&) {++count;}
    ~A() {--count;}
};

int A::count = 0;

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_pointer(void) {
    {
        std::shared_ptr<A> pA(new A);
        TC_ASSERT_EXPR(pA.use_count() == 1);

        {
            B b;
            std::shared_ptr<B> pB(pA, &b);
            TC_ASSERT_EXPR(A::count == 1);
            TC_ASSERT_EXPR(B::count == 1);
            TC_ASSERT_EXPR(pA.use_count() == 2);
            TC_ASSERT_EXPR(pB.use_count() == 2);
            TC_ASSERT_EXPR(pB.get() == &b);
        }
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(A::count == 1);
        TC_ASSERT_EXPR(B::count == 0);
    }
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(B::count == 0);

    {
        std::shared_ptr<A const> pA(new A);
        TC_ASSERT_EXPR(pA.use_count() == 1);

        {
            B const b;
            std::shared_ptr<B const> pB(pA, &b);
            TC_ASSERT_EXPR(A::count == 1);
            TC_ASSERT_EXPR(B::count == 1);
            TC_ASSERT_EXPR(pA.use_count() == 2);
            TC_ASSERT_EXPR(pB.use_count() == 2);
            TC_ASSERT_EXPR(pB.get() == &b);
        }
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(A::count == 1);
        TC_ASSERT_EXPR(B::count == 0);
    }
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(B::count == 0);

    int *pi = new int;
    {
      std::shared_ptr<int> p1(nullptr);
      std::shared_ptr<int> p2(p1, pi);
      TC_ASSERT_EXPR(p2.get() == pi);
    }
    delete pi;
    {
      std::shared_ptr<int> p1(new int);
      std::shared_ptr<int> p2(p1, nullptr);
      TC_ASSERT_EXPR(p2.get() == nullptr);
    }

#if TEST_STD_VER > 17 && defined(_LIBCPP_VERSION)
    {
      std::shared_ptr<A> pA(new A);
      TC_ASSERT_EXPR(pA.use_count() == 1);

#  if TEST_STD_VER >= 20
      // LWG-2996 is only implemented in c++20 and beyond.
      // We don't backport because it is an evolutionary change.
      {
        B b;
        std::shared_ptr<B> pB(std::move(pA), &b);
        TC_ASSERT_EXPR(A::count == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(pB.use_count() == 1);
        TC_ASSERT_EXPR(pB.get() == &b);
      }
#  endif // TEST_STD_VER > 20
    }
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(B::count == 0);
#endif

    return 0;
}
