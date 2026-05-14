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

// template<class T> void swap(shared_ptr<T>& a, shared_ptr<T>& b);

#include <memory>
#include <cassert>

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

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_spec_swap(void) {
    {
        A* ptr1 = new A;
        A* ptr2 = new A;
        std::shared_ptr<A> p1(ptr1);
        {
            std::shared_ptr<A> p2(ptr2);
            swap(p1, p2);
            TC_ASSERT_EXPR(p1.use_count() == 1);
            TC_ASSERT_EXPR(p1.get() == ptr2);
            TC_ASSERT_EXPR(p2.use_count() == 1);
            TC_ASSERT_EXPR(p2.get() == ptr1);
            TC_ASSERT_EXPR(A::count == 2);
        }
        TC_ASSERT_EXPR(p1.use_count() == 1);
        TC_ASSERT_EXPR(p1.get() == ptr2);
        TC_ASSERT_EXPR(A::count == 1);
    }
    TC_ASSERT_EXPR(A::count == 0);
    {
        A* ptr1 = new A;
        A* ptr2 = 0;
        std::shared_ptr<A> p1(ptr1);
        {
            std::shared_ptr<A> p2;
            swap(p1, p2);
            TC_ASSERT_EXPR(p1.use_count() == 0);
            TC_ASSERT_EXPR(p1.get() == ptr2);
            TC_ASSERT_EXPR(p2.use_count() == 1);
            TC_ASSERT_EXPR(p2.get() == ptr1);
            TC_ASSERT_EXPR(A::count == 1);
        }
        TC_ASSERT_EXPR(p1.use_count() == 0);
        TC_ASSERT_EXPR(p1.get() == ptr2);
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(A::count == 0);
    {
        A* ptr1 = 0;
        A* ptr2 = new A;
        std::shared_ptr<A> p1;
        {
            std::shared_ptr<A> p2(ptr2);
            swap(p1, p2);
            TC_ASSERT_EXPR(p1.use_count() == 1);
            TC_ASSERT_EXPR(p1.get() == ptr2);
            TC_ASSERT_EXPR(p2.use_count() == 0);
            TC_ASSERT_EXPR(p2.get() == ptr1);
            TC_ASSERT_EXPR(A::count == 1);
        }
        TC_ASSERT_EXPR(p1.use_count() == 1);
        TC_ASSERT_EXPR(p1.get() == ptr2);
        TC_ASSERT_EXPR(A::count == 1);
    }
    TC_ASSERT_EXPR(A::count == 0);
    {
        A* ptr1 = 0;
        A* ptr2 = 0;
        std::shared_ptr<A> p1;
        {
            std::shared_ptr<A> p2;
            swap(p1, p2);
            TC_ASSERT_EXPR(p1.use_count() == 0);
            TC_ASSERT_EXPR(p1.get() == ptr2);
            TC_ASSERT_EXPR(p2.use_count() == 0);
            TC_ASSERT_EXPR(p2.get() == ptr1);
            TC_ASSERT_EXPR(A::count == 0);
        }
        TC_ASSERT_EXPR(p1.use_count() == 0);
        TC_ASSERT_EXPR(p1.get() == ptr2);
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(A::count == 0);

  return 0;
}
