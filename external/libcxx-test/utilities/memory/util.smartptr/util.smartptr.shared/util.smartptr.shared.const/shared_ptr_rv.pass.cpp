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

// shared_ptr(shared_ptr&& r);

#include <memory>
#include <cassert>
#include <utility>

#include "test_macros.h"
#include "libcxx_tc_common.h"

struct A
{
    static int count;

    A() {++count;}
    A(const A&) {++count;}
    ~A() {--count;}
};

int A::count = 0;

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_rv(void) {
    {
        std::shared_ptr<A> pA(new A);
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(A::count == 1);
        {
            A* p = pA.get();
            std::shared_ptr<A> pA2(std::move(pA));
            TC_ASSERT_EXPR(A::count == 1);
#if TEST_STD_VER >= 11
            TC_ASSERT_EXPR(pA.use_count() == 0);
            TC_ASSERT_EXPR(pA2.use_count() == 1);
#else
            TC_ASSERT_EXPR(pA.use_count() == 2);
            TC_ASSERT_EXPR(pA2.use_count() == 2);
#endif
            TC_ASSERT_EXPR(pA2.get() == p);
        }
#if TEST_STD_VER >= 11
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(A::count == 0);
#else
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(A::count == 1);
#endif
    }

    TC_ASSERT_EXPR(A::count == 0);
    {
        std::shared_ptr<A> pA;
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(A::count == 0);
        {
            std::shared_ptr<A> pA2(std::move(pA));
            TC_ASSERT_EXPR(A::count == 0);
            TC_ASSERT_EXPR(pA.use_count() == 0);
            TC_ASSERT_EXPR(pA2.use_count() == 0);
            TC_ASSERT_EXPR(pA2.get() == pA.get());
        }
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(A::count == 0);

    {
        std::shared_ptr<A const> pA(new A);
        A const* p = pA.get();
        std::shared_ptr<A const> pA2(std::move(pA));
        TC_ASSERT_EXPR(pA2.get() == p);
    }

  return 0;
}
