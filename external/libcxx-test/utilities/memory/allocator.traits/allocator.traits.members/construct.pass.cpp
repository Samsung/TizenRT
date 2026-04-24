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

// template <class Alloc>
// struct allocator_traits
// {
//     template <class Ptr, class... Args>
//     static constexpr void construct(allocator_type& a, Ptr p, Args&&... args);
//     ...
// };

#include <memory>
#include <new>
#include <type_traits>
#include <cassert>
#include <utility>

#include "test_macros.h"
#include "incomplete_type_helper.h"
#include "libcxx_tc_common.h"

template <class T>
struct A
{
    typedef T value_type;

};

template <class T>
struct B
{
    typedef T value_type;

    TEST_CONSTEXPR_CXX20 B(int& count) : count_(count) {}

#if TEST_STD_VER >= 11
    template <class U, class ...Args>
    TEST_CONSTEXPR_CXX20 void construct(U* p, Args&& ...args)
    {
        ++count_;
#if TEST_STD_VER > 17
        std::construct_at(p, std::forward<Args>(args)...);
#else
        ::new ((void*)p) U(std::forward<Args>(args)...);
#endif
    }
#endif

    int& count_;
};

struct A0
{
    TEST_CONSTEXPR_CXX20 A0(int* count) {++*count;}
};

struct A1
{
    TEST_CONSTEXPR_CXX20 A1(int* count, char c)
    {
        TC_ASSERT_EXPR(c == 'c');
        ++*count;
    }
};

struct A2
{
    TEST_CONSTEXPR_CXX20 A2(int* count, char c, int i)
    {
        TC_ASSERT_EXPR(c == 'd');
        TC_ASSERT_EXPR(i == 5);
        ++*count;
    }
};

TEST_CONSTEXPR_CXX20 bool test()
{
    {
        int A0_count = 0;
        A<A0> a;
        std::allocator<A0> alloc;
        A0* a0 = alloc.allocate(1);
        TC_ASSERT_EXPR(A0_count == 0);
        std::allocator_traits<A<A0> >::construct(a, a0, &A0_count);
        TC_ASSERT_EXPR(A0_count == 1);
        alloc.deallocate(a0, 1);
    }
    {
        int A1_count = 0;
        A<A1> a;
        std::allocator<A1> alloc;
        A1* a1 = alloc.allocate(1);
        TC_ASSERT_EXPR(A1_count == 0);
        std::allocator_traits<A<A1> >::construct(a, a1, &A1_count, 'c');
        TC_ASSERT_EXPR(A1_count == 1);
        alloc.deallocate(a1, 1);
    }
    {
        int A2_count = 0;
        A<A2> a;
        std::allocator<A2> alloc;
        A2* a2 = alloc.allocate(1);
        TC_ASSERT_EXPR(A2_count == 0);
        std::allocator_traits<A<A2> >::construct(a, a2, &A2_count, 'd', 5);
        TC_ASSERT_EXPR(A2_count == 1);
        alloc.deallocate(a2, 1);
    }
    {
      typedef IncompleteHolder* VT;
      typedef A<VT> Alloc;
      Alloc a;
      std::allocator<VT> alloc;
      VT* vt = alloc.allocate(1);
      std::allocator_traits<Alloc>::construct(a, vt, nullptr);
      alloc.deallocate(vt, 1);
    }

#if TEST_STD_VER >= 11
    {
        int A0_count = 0;
        int b_construct = 0;
        B<A0> b(b_construct);
        std::allocator<A0> alloc;
        A0* a0 = alloc.allocate(1);
        TC_ASSERT_EXPR(A0_count == 0);
        TC_ASSERT_EXPR(b_construct == 0);
        std::allocator_traits<B<A0> >::construct(b, a0, &A0_count);
        TC_ASSERT_EXPR(A0_count == 1);
        TC_ASSERT_EXPR(b_construct == 1);
        alloc.deallocate(a0, 1);
    }
    {
        int A1_count = 0;
        int b_construct = 0;
        B<A1> b(b_construct);
        std::allocator<A1> alloc;
        A1* a1 = alloc.allocate(1);
        TC_ASSERT_EXPR(A1_count == 0);
        TC_ASSERT_EXPR(b_construct == 0);
        std::allocator_traits<B<A1> >::construct(b, a1, &A1_count, 'c');
        TC_ASSERT_EXPR(A1_count == 1);
        TC_ASSERT_EXPR(b_construct == 1);
        alloc.deallocate(a1, 1);
    }
    {
        int A2_count = 0;
        int b_construct = 0;
        B<A2> b(b_construct);
        std::allocator<A2> alloc;
        A2* a2 = alloc.allocate(1);
        TC_ASSERT_EXPR(A2_count == 0);
        TC_ASSERT_EXPR(b_construct == 0);
        std::allocator_traits<B<A2> >::construct(b, a2, &A2_count, 'd', 5);
        TC_ASSERT_EXPR(A2_count == 1);
        TC_ASSERT_EXPR(b_construct == 1);
        alloc.deallocate(a2, 1);
    }
#endif

    return true;
}

int tc_utilities_memory_allocator_traits_allocator_traits_members_construct(void) {
    test();

#if TEST_STD_VER > 17
    static_assert(test());
#endif

    return 0;
}
