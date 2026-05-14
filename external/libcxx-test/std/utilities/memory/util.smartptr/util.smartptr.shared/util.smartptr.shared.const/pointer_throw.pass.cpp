//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: no-exceptions
// UNSUPPORTED: sanitizer-new-delete

// <memory>

// template<class Y> explicit shared_ptr(Y* p);


#include <memory>
#include <new>
#include <cstdlib>
#include <cassert>

#include "count_new.h"

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


int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_pointer_throw(void) {
    globalMemCounter.reset();
    A* ptr = new A;
    TC_ASSERT_EXPR(A::count == 1);
    globalMemCounter.throw_after = 0;
    try
    {
        std::shared_ptr<A> p(ptr);
        TC_ASSERT_EXPR(false);
    }
    catch (std::bad_alloc&)
    {
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(globalMemCounter.checkOutstandingNewEq(0));

  return 0;
}
