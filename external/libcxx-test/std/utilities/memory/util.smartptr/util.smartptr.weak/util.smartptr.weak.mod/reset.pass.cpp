//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// weak_ptr

// void swap(weak_ptr& r);

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

int tc_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_mod_reset(void) {
    {
        std::shared_ptr<A> p1(new A);
        std::weak_ptr<A> w1(p1);
        TC_ASSERT_EXPR(w1.use_count() == 1);
        w1.reset();
        TC_ASSERT_EXPR(w1.use_count() == 0);
        TC_ASSERT_EXPR(p1.use_count() == 1);
    }
    TC_ASSERT_EXPR(A::count == 0);

  return 0;
}
