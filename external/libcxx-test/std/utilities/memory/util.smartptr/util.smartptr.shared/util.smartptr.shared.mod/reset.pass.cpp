//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// shared_ptr

// void reset();

#include <memory>
#include <cassert>

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

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset(void) {
    {
        std::shared_ptr<B> p(new B);
        p.reset();
        TC_ASSERT_EXPR(A::count == 0);
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(p.use_count() == 0);
        TC_ASSERT_EXPR(p.get() == 0);
    }
    TC_ASSERT_EXPR(A::count == 0);
    {
        std::shared_ptr<B> p;
        p.reset();
        TC_ASSERT_EXPR(A::count == 0);
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(p.use_count() == 0);
        TC_ASSERT_EXPR(p.get() == 0);
    }
    TC_ASSERT_EXPR(A::count == 0);

  return 0;
}
