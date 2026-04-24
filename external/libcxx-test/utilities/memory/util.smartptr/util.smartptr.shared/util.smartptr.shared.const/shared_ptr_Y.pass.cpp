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

// template<class Y> shared_ptr(const shared_ptr<Y>& r);

#include <memory>
#include <type_traits>
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

struct C
{
    static int count;

    C() {++count;}
    C(const C&) {++count;}
    virtual ~C() {--count;}
};

int C::count = 0;

class private_delete_op
{
    static void operator delete (void *p) {
        return delete static_cast<char*>(p);
    }
public:
    static void operator delete[] (void *p) {
        return delete[] static_cast<char*>(p);
    }
};

class private_delete_arr_op
{
    static void operator delete[] (void *p) {
        return delete[] static_cast<char*>(p);
    }
public:
    static void operator delete (void *p) {
        return delete static_cast<char*>(p);
    }
};

// https://llvm.org/PR60258
// Invalid constructor SFINAE for std::shared_ptr's array ctors
static_assert(!std::is_constructible<std::shared_ptr<int>,     const std::shared_ptr<long>&>::value, "");
static_assert( std::is_constructible<std::shared_ptr<B>,       const std::shared_ptr<A>&>::value, "");
static_assert( std::is_constructible<std::shared_ptr<const A>, const std::shared_ptr<A>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<A>,       const std::shared_ptr<const A>&>::value, "");

#if TEST_STD_VER >= 17
static_assert(!std::is_constructible<std::shared_ptr<int>,     const std::shared_ptr<int[]>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int>,     const std::shared_ptr<int[5]>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[]>,   const std::shared_ptr<int>&>::value, "");
static_assert( std::is_constructible<std::shared_ptr<int[]>,   const std::shared_ptr<int[5]>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[5]>,  const std::shared_ptr<int>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[5]>,  const std::shared_ptr<int[]>&>::value, "");
static_assert(!std::is_constructible<std::shared_ptr<int[7]>,  const std::shared_ptr<int[5]>&>::value, "");
#endif

int tc_utilities_memory_util_smartptr_util_smartptr_shared_util_smartptr_shared_const_shared_ptr_Y(void) {
    static_assert(( std::is_convertible<std::shared_ptr<A>, std::shared_ptr<B> >::value), "");
    static_assert((!std::is_convertible<std::shared_ptr<B>, std::shared_ptr<A> >::value), "");
    static_assert((!std::is_convertible<std::shared_ptr<A>, std::shared_ptr<C> >::value), "");
    {
        const std::shared_ptr<A> pA(new A);
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(A::count == 1);
        {
            std::shared_ptr<B> pB(pA);
            TC_ASSERT_EXPR(B::count == 1);
            TC_ASSERT_EXPR(A::count == 1);
            TC_ASSERT_EXPR(pB.use_count() == 2);
            TC_ASSERT_EXPR(pA.use_count() == 2);
            TC_ASSERT_EXPR(pA.get() == pB.get());
        }
        TC_ASSERT_EXPR(pA.use_count() == 1);
        TC_ASSERT_EXPR(B::count == 1);
        TC_ASSERT_EXPR(A::count == 1);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);
    {
        std::shared_ptr<A> pA;
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
        {
            std::shared_ptr<B> pB(pA);
            TC_ASSERT_EXPR(B::count == 0);
            TC_ASSERT_EXPR(A::count == 0);
            TC_ASSERT_EXPR(pB.use_count() == 0);
            TC_ASSERT_EXPR(pA.use_count() == 0);
            TC_ASSERT_EXPR(pA.get() == pB.get());
        }
        TC_ASSERT_EXPR(pA.use_count() == 0);
        TC_ASSERT_EXPR(B::count == 0);
        TC_ASSERT_EXPR(A::count == 0);
    }
    TC_ASSERT_EXPR(B::count == 0);
    TC_ASSERT_EXPR(A::count == 0);

    // This should work in C++03 but we get errors when trying to do SFINAE with the delete operator.
    // GCC also complains about this.
#if TEST_STD_VER >= 11 && !defined(TEST_COMPILER_GCC)
    {
        // LWG2874: Make sure that when T (for std::shared_ptr<T>) is an array type,
        //          this constructor only participates in overload resolution when
        //          `delete[] p` is well formed. And when T is not an array type,
        //          this constructor only participates in overload resolution when
        //          `delete p` is well formed.
        static_assert(!std::is_constructible<std::shared_ptr<private_delete_op>,
                                                             private_delete_op*>::value, "");
        static_assert(!std::is_constructible<std::shared_ptr<private_delete_arr_op[4]>,
                                                             private_delete_arr_op*>::value, "");
    }
#endif

#if TEST_STD_VER > 14
    {
        std::shared_ptr<A[]> p1(new A[8]);
        TC_ASSERT_EXPR(p1.use_count() == 1);
        TC_ASSERT_EXPR(A::count == 8);
        {
            std::shared_ptr<const A[]> p2(p1);
            TC_ASSERT_EXPR(A::count == 8);
            TC_ASSERT_EXPR(p2.use_count() == 2);
            TC_ASSERT_EXPR(p1.use_count() == 2);
            TC_ASSERT_EXPR(p1.get() == p2.get());
        }
        TC_ASSERT_EXPR(p1.use_count() == 1);
        TC_ASSERT_EXPR(A::count == 8);
    }
    TC_ASSERT_EXPR(A::count == 0);
#endif

    {
        std::shared_ptr<A const> pA(new A);
        std::shared_ptr<B const> pB(pA);
        TC_ASSERT_EXPR(pB.get() == pA.get());
    }

    return 0;
}
