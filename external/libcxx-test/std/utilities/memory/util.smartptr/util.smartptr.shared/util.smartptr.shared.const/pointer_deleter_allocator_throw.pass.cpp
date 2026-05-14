//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: no-exceptions
// <memory>

// template<class Y, class D, class A> shared_ptr(Y* p, D d, A a);

#include <memory>
#include <cassert>
#include "test_macros.h"
#include "deleter_types.h"
#include "test_allocator.h"
#include "libcxx_tc_common.h"

struct A
{
    static int count;

    A() {++count;}
    A(const A&) {++count;}
    ~A() {--count;}
};

int A::count = 0;

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_pointer_deleter_allocator_throw(void) {
    test_allocator_statistics alloc_stats;
    A* ptr = new A;
    try
    {
        alloc_stats.throw_after = 0;
        std::shared_ptr<A> p(ptr, test_deleter<A>(3), test_allocator<A>(5, &alloc_stats));
        TC_ASSERT_EXPR(false);
    }
    catch (std::bad_alloc&)
    {
        TC_ASSERT_EXPR(alloc_stats.count == 0);
        TC_ASSERT_EXPR(alloc_stats.count == 0);
        TC_ASSERT_EXPR(test_deleter<A>::dealloc_count == 1);
        TC_ASSERT_EXPR(alloc_stats.count == 0);
        TC_ASSERT_EXPR(alloc_stats.alloc_count == 0);
    }

  return 0;
}
