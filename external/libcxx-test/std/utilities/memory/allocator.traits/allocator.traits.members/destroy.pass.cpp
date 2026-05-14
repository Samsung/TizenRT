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
//     template <class Ptr>
//     static constexpr void destroy(allocator_type& a, Ptr p);
//     ...
// };

#include <memory>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "incomplete_type_helper.h"
#include "libcxx_tc_common.h"

template <class T>
struct NoDestroy
{
    typedef T value_type;

    TEST_CONSTEXPR_CXX20 T* allocate(std::size_t n)
    {
        return std::allocator<T>().allocate(n);
    }

    TEST_CONSTEXPR_CXX20 void deallocate(T* p, std::size_t n)
    {
        return std::allocator<T>().deallocate(p, n);
    }
};

template <class T>
struct CountDestroy
{
    TEST_CONSTEXPR explicit CountDestroy(int* counter)
        : counter_(counter)
    { }

    typedef T value_type;

    TEST_CONSTEXPR_CXX20 T* allocate(std::size_t n)
    {
        return std::allocator<T>().allocate(n);
    }

    TEST_CONSTEXPR_CXX20 void deallocate(T* p, std::size_t n)
    {
        return std::allocator<T>().deallocate(p, n);
    }

    template <class U>
    TEST_CONSTEXPR_CXX20 void destroy(U* p)
    {
        ++*counter_;
        p->~U();
    }

    int* counter_;
};

struct CountDestructor
{
    TEST_CONSTEXPR explicit CountDestructor(int* counter)
        : counter_(counter)
    { }

    TEST_CONSTEXPR_CXX20 ~CountDestructor() { ++*counter_; }

    int* counter_;
};

TEST_CONSTEXPR_CXX20 bool test()
{
    {
        typedef NoDestroy<CountDestructor> Alloc;
        int destructors = 0;
        Alloc alloc;
        CountDestructor* pool = std::allocator_traits<Alloc>::allocate(alloc, 1);

        std::allocator_traits<Alloc>::construct(alloc, pool, &destructors);
        TC_ASSERT_EXPR(destructors == 0);

        std::allocator_traits<Alloc>::destroy(alloc, pool);
        TC_ASSERT_EXPR(destructors == 1);

        std::allocator_traits<Alloc>::deallocate(alloc, pool, 1);
    }
    {
        typedef IncompleteHolder* T;
        typedef NoDestroy<T> Alloc;
        Alloc alloc;
        T* pool = std::allocator_traits<Alloc>::allocate(alloc, 1);
        std::allocator_traits<Alloc>::construct(alloc, pool, nullptr);
        std::allocator_traits<Alloc>::destroy(alloc, pool);
        std::allocator_traits<Alloc>::deallocate(alloc, pool, 1);
    }
    {
        typedef CountDestroy<CountDestructor> Alloc;
        int destroys_called = 0;
        int destructors_called = 0;
        Alloc alloc(&destroys_called);

        CountDestructor* pool = std::allocator_traits<Alloc>::allocate(alloc, 1);
        std::allocator_traits<Alloc>::construct(alloc, pool, &destructors_called);
        TC_ASSERT_EXPR(destroys_called == 0);
        TC_ASSERT_EXPR(destructors_called == 0);

        std::allocator_traits<Alloc>::destroy(alloc, pool);
        TC_ASSERT_EXPR(destroys_called == 1);
        TC_ASSERT_EXPR(destructors_called == 1);

        std::allocator_traits<Alloc>::deallocate(alloc, pool, 1);
    }
    return true;
}

int tc_utilities_memory_allocator_traits_allocator_traits_members_destroy(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    return 0;
}
