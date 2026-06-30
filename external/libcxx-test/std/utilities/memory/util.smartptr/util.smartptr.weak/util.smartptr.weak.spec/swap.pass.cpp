//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// weak_ptr

// template<class T> void swap(weak_ptr<T>& a, weak_ptr<T>& b)

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

int tc_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_spec_swap(void) {
    {
        A* ptr1 = new A;
        A* ptr2 = new A;
        std::shared_ptr<A> p1(ptr1);
        std::weak_ptr<A> w1(p1);
        {
            std::shared_ptr<A> p2(ptr2);
            std::weak_ptr<A> w2(p2);
            swap(w1, w2);
            TC_ASSERT_EXPR(w1.use_count() == 1);
            TC_ASSERT_EXPR(w1.lock().get() == ptr2);
            TC_ASSERT_EXPR(w2.use_count() == 1);
            TC_ASSERT_EXPR(w2.lock().get() == ptr1);
            TC_ASSERT_EXPR(A::count == 2);
        }
    }
    TC_ASSERT_EXPR(A::count == 0);

  return 0;
}
