//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// weak_ptr

// bool expired() const;

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

int tc_utilities_memory_util_smartptr_util_smartptr_weak_util_smartptr_weak_obs_expired(void) {
    {
        std::weak_ptr<A> wp;
        TC_ASSERT_EXPR(wp.use_count() == 0);
        TC_ASSERT_EXPR(wp.expired() == (wp.use_count() == 0));
    }
    {
        std::shared_ptr<A> sp0(new A);
        std::weak_ptr<A> wp(sp0);
        TC_ASSERT_EXPR(wp.use_count() == 1);
        TC_ASSERT_EXPR(wp.expired() == (wp.use_count() == 0));
        sp0.reset();
        TC_ASSERT_EXPR(wp.use_count() == 0);
        TC_ASSERT_EXPR(wp.expired() == (wp.use_count() == 0));
    }

  return 0;
}
