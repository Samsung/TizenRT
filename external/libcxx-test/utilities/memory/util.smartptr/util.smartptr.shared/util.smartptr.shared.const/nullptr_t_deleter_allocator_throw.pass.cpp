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
// UNSUPPORTED: no-exceptions
// <memory>

// template<class D, class A> shared_ptr(nullptr_t, D d, A a);

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

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_nullptr_t_deleter_allocator_throw(void) {
    test_allocator_statistics alloc_stats;
    try
    {
        alloc_stats.throw_after = 0;
        std::shared_ptr<A> p(nullptr, test_deleter<A>(3), test_allocator<A>(5, &alloc_stats));
        TC_ASSERT_EXPR(false);
    }
    catch (std::bad_alloc&)
    {
        TC_ASSERT_EXPR(A::count == 0);
        TC_ASSERT_EXPR(test_deleter<A>::count == 0);
        TC_ASSERT_EXPR(test_deleter<A>::dealloc_count == 1);
        TC_ASSERT_EXPR(alloc_stats.count == 0);
        TC_ASSERT_EXPR(alloc_stats.alloc_count == 0);
    }

  return 0;
}
