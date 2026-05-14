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
//     static constexpr void deallocate(allocator_type& a, pointer p, size_type n);
//     ...
// };

#include <memory>
#include <cassert>
#include <cstddef>

#include "test_macros.h"
#include "incomplete_type_helper.h"
#include "libcxx_tc_common.h"

template <class T>
struct A
{
    typedef T value_type;

    TEST_CONSTEXPR_CXX20 A(int& called) : called_(called) {}

    TEST_CONSTEXPR_CXX20 void deallocate(value_type* p, std::size_t n)
    {
        TC_ASSERT_EXPR(p == &storage);
        TC_ASSERT_EXPR(n == 10);
        ++called_;
    }

    int& called_;

    value_type storage;
};

TEST_CONSTEXPR_CXX20 bool test()
{
    {
        int called = 0;
        A<int> a(called);
        std::allocator_traits<A<int> >::deallocate(a, &a.storage, 10);
        TC_ASSERT_EXPR(called == 1);
    }
    {
        int called = 0;
        typedef A<IncompleteHolder*> Alloc;
        Alloc a(called);
        std::allocator_traits<Alloc>::deallocate(a, &a.storage, 10);
        TC_ASSERT_EXPR(called == 1);
    }

    return true;
}

int tc_utilities_memory_allocator_traits_allocator_traits_members_deallocate(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    return 0;
}
