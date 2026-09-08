//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// template<class Y> explicit shared_ptr(Y* p);

#include <cassert>
#include <memory>
#include <type_traits>

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

struct Derived : A {};

// https://llvm.org/PR60258
// Invalid constructor SFINAE for std::shared_ptr's array ctors
static_assert( std::is_constructible<std::shared_ptr<int>,  int*>::value, "");
static_assert( std::is_constructible<std::shared_ptr<A>,  Derived*>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<A>,  int*>::value, "");

#if TEST_STD_VER >= 17
static_assert( std::is_constructible<std::shared_ptr<int[]>,  int*>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[]>,  int(*)[]>::value, "");
static_assert( std::is_constructible<std::shared_ptr<int[5]>, int*>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[5]>, int(*)[5]>::value, "");
#endif

// Test explicit
static_assert(std::is_constructible<std::shared_ptr<int>, int*>::value, "");
static_assert(!std::is_convertible<int*, std::shared_ptr<int> >::value, "");

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_pointer(void) {
    {
        TC_ASSERT_EXPR(A::count == 0);
        A* ptr = new A;
        std::shared_ptr<A> p(ptr);
        TC_ASSERT_EXPR(A::count == 1);
        TC_ASSERT_EXPR(p.use_count() == 1);
        TC_ASSERT_EXPR(p.get() == ptr);
    }

    {
        TC_ASSERT_EXPR(A::count == 0);
        A const* ptr = new A;
        std::shared_ptr<A const> p(ptr);
        TC_ASSERT_EXPR(A::count == 1);
        TC_ASSERT_EXPR(p.use_count() == 1);
        TC_ASSERT_EXPR(p.get() == ptr);
    }

    {
        TC_ASSERT_EXPR(A::count == 0);
        A* ptr = new A;
        std::shared_ptr<void> p(ptr);
        TC_ASSERT_EXPR(A::count == 1);
        TC_ASSERT_EXPR(p.use_count() == 1);
        TC_ASSERT_EXPR(p.get() == ptr);
    }

#if TEST_STD_VER > 14
    {
        TC_ASSERT_EXPR(A::count == 0);
        std::shared_ptr<A[8]> pA(new A[8]);
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(A::count == 8);
    }

    {
        TC_ASSERT_EXPR(A::count == 0);
        std::shared_ptr<A[]> pA(new A[8]);
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(A::count == 8);
    }

    {
        TC_ASSERT_EXPR(A::count == 0);
        std::shared_ptr<const A[]> pA(new A[8]);
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(A::count == 8);
    }

    {
        TC_ASSERT_EXPR(A::count == 0);
        std::shared_ptr<A> pA(new Derived);
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(A::count == 1);
    }
#endif

    return 0;
}
