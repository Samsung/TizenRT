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

// template<class D, class A> shared_ptr(nullptr_t, D d, A a);

#include <memory>
#include <cassert>
#include "test_macros.h"
#include "deleter_types.h"
#include "test_allocator.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

struct A
{
    static int count;

    A() {++count;}
    A(const A&) {++count;}
    ~A() {--count;}
};

int A::count = 0;

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_nullptr_t_deleter_allocator(void) {
    test_allocator_statistics alloc_stats;
    {
        std::shared_ptr<A> p(nullptr, test_deleter<A>(3), test_allocator<A>(5, &alloc_stats));
        TC_ASSERT_EXPR(A::count == 0);
        TC_ASSERT_EXPR(p.use_count() == 1);
        TC_ASSERT_EXPR(p.get() == 0);
        TC_ASSERT_EXPR(test_deleter<A>::count == 1);
        TC_ASSERT_EXPR(test_deleter<A>::dealloc_count == 0);
#ifndef TEST_HAS_NO_RTTI
        test_deleter<A>* d = std::get_deleter<test_deleter<A> >(p);
        TC_ASSERT_EXPR(d);
        TC_ASSERT_EXPR(d->state() == 3);
#endif
        TC_ASSERT_EXPR(alloc_stats.count == 1);
        TC_ASSERT_EXPR(alloc_stats.alloc_count == 1);
    }
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(test_deleter<A>::count == 0);
    TC_ASSERT_EXPR(test_deleter<A>::dealloc_count == 1);
    TC_ASSERT_EXPR(alloc_stats.count == 0);
    TC_ASSERT_EXPR(alloc_stats.alloc_count == 0);
    test_deleter<A>::dealloc_count = 0;

    // Test an allocator with a minimal interface
    {
        std::shared_ptr<A> p(nullptr, test_deleter<A>(1), bare_allocator<void>());
        TC_ASSERT_EXPR(A::count == 0);
        TC_ASSERT_EXPR(p.use_count() == 1);
        TC_ASSERT_EXPR(p.get() == 0);
        TC_ASSERT_EXPR(test_deleter<A>::count ==1);
        TC_ASSERT_EXPR(test_deleter<A>::dealloc_count == 0);
#ifndef TEST_HAS_NO_RTTI
        test_deleter<A>* d = std::get_deleter<test_deleter<A> >(p);
        TC_ASSERT_EXPR(d);
        TC_ASSERT_EXPR(d->state() == 1);
#endif
    }
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(test_deleter<A>::count == 0);
    TC_ASSERT_EXPR(test_deleter<A>::dealloc_count == 1);
    test_deleter<A>::dealloc_count = 0;

#if TEST_STD_VER >= 11
    // Test an allocator that returns class-type pointers
    {
        std::shared_ptr<A> p(nullptr, test_deleter<A>(1), min_allocator<void>());
        TC_ASSERT_EXPR(A::count == 0);
        TC_ASSERT_EXPR(p.use_count() == 1);
        TC_ASSERT_EXPR(p.get() == 0);
        TC_ASSERT_EXPR(test_deleter<A>::count ==1);
        TC_ASSERT_EXPR(test_deleter<A>::dealloc_count == 0);
#ifndef TEST_HAS_NO_RTTI
        test_deleter<A>* d = std::get_deleter<test_deleter<A> >(p);
        TC_ASSERT_EXPR(d);
        TC_ASSERT_EXPR(d->state() == 1);
#endif
    }
    TC_ASSERT_EXPR(A::count == 0);
    TC_ASSERT_EXPR(test_deleter<A>::count == 0);
    TC_ASSERT_EXPR(test_deleter<A>::dealloc_count == 1);
#endif

    // Make sure we can use this constructor with a pointer-to-const
    {
        std::shared_ptr<A const> p(nullptr, test_deleter<A const>(3), test_allocator<A>(5, &alloc_stats));
        (void)p;
    }

  return 0;
}
