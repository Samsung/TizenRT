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

// template<class Y, class D, class A> void reset(Y* p, D d, A a);

#include <cassert>
#include <memory>
#include "test_macros.h"
#include "deleter_types.h"
#include "reset_helper.h"
#include "test_allocator.h"
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

struct bad_ty { };

struct bad_deleter
{
    void operator()(bad_ty) { }
};

struct Base { };
struct Derived : Base { };

static_assert( HasReset<std::shared_ptr<int>,  int*, test_deleter<int>, test_allocator<int> >::value, "");
static_assert(!HasReset<std::shared_ptr<int>,  int*, bad_deleter, test_allocator<int> >::value, "");
static_assert( HasReset<std::shared_ptr<Base>,  Derived*, test_deleter<Base>, test_allocator<Base> >::value, "");
static_assert(!HasReset<std::shared_ptr<A>,  int*, test_deleter<A>, test_allocator<A> >::value, "");

#if TEST_STD_VER >= 17
static_assert( HasReset<std::shared_ptr<int[]>,  int*, test_deleter<int>, test_allocator<int>>::value, "");
static_assert(!HasReset<std::shared_ptr<int[]>,  int*, bad_deleter, test_allocator<int>>::value, "");
static_assert(!HasReset<std::shared_ptr<int[]>,  int(*)[], test_deleter<int>, test_allocator<int>>::value, "");
static_assert( HasReset<std::shared_ptr<int[5]>, int*, test_deleter<int>, test_allocator<int>>::value, "");
static_assert(!HasReset<std::shared_ptr<int[5]>, int*, bad_deleter, test_allocator<int>>::value, "");
static_assert(!HasReset<std::shared_ptr<int[5]>, int(*)[5], test_deleter<int>, test_allocator<int>>::value, "");
#endif

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_mod_reset_pointer_deleter_allocator(void) {
    test_allocator_statistics alloc_stats;
    {
        std::shared_ptr<B> p(new B);
        A* ptr = new A;
        p.reset(ptr, test_deleter<A>(3), test_allocator<A>(4, &alloc_stats));
        TC_ASSERT_EXPR(A::count == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(p.use_count() == 1);
        TC_ASSERT_EXPR(p.get() == ptr);
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
    {
        std::shared_ptr<B> p;
        A* ptr = new A;
        p.reset(ptr, test_deleter<A>(3), test_allocator<A>(4, &alloc_stats));
        TC_ASSERT_EXPR(A::count == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(p.use_count() == 1);
        TC_ASSERT_EXPR(p.get() == ptr);
        TC_ASSERT_EXPR(test_deleter<A>::count == 1);
        TC_ASSERT_EXPR(test_deleter<A>::dealloc_count == 1);
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
    TC_ASSERT_EXPR(test_deleter<A>::dealloc_count == 2);
    TC_ASSERT_EXPR(alloc_stats.count == 0);
    TC_ASSERT_EXPR(alloc_stats.alloc_count == 0);

#if TEST_STD_VER > 14
    {
        std::shared_ptr<const A[]> p;
        A* ptr = new A[8];
        p.reset(ptr, CDeleter<A[]>(), test_allocator<A[]>());
        TC_ASSERT_EXPR(A::count == 8);
        TC_ASSERT_EXPR(p.use_count() == 1);
        TC_ASSERT_EXPR(p.get() == ptr);
    }
    TC_ASSERT_EXPR(A::count == 0);
#endif

  return 0;
}
