//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// shared_ptr

// void swap(shared_ptr& r);

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

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_swap(void) {
    {
        A* ptr1 = new A;
        A* ptr2 = new A;
        std::shared_ptr<A> p1(ptr1);
        {
            std::shared_ptr<A> p2(ptr2);
            p1.swap(p2);
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
            p1.swap(p2);
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
            p1.swap(p2);
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
            p1.swap(p2);
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
