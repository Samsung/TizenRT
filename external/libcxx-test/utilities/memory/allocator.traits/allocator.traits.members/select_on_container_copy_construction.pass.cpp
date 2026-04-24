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
//     static constexpr allocator_type
//         select_on_container_copy_construction(const allocator_type& a);
//     ...
// };

#include <memory>
#include <new>
#include <type_traits>
#include <cassert>

#include "test_macros.h"
#include "incomplete_type_helper.h"
#include "libcxx_tc_common.h"

template <class T>
struct A
{
    typedef T value_type;
    int id;
    TEST_CONSTEXPR_CXX20 explicit A(int i = 0) : id(i) {}

};

template <class T>
struct B
{
    typedef T value_type;

    int id;
    TEST_CONSTEXPR_CXX20 explicit B(int i = 0) : id(i) {}

    TEST_CONSTEXPR_CXX20 B select_on_container_copy_construction() const
    {
        return B(100);
    }
};

TEST_CONSTEXPR_CXX20 bool test()
{
    {
        A<int> a;
        TC_ASSERT_EXPR(std::allocator_traits<A<int> >::select_on_container_copy_construction(a).id == 0);
    }
    {
        const A<int> a(0);
        TC_ASSERT_EXPR(std::allocator_traits<A<int> >::select_on_container_copy_construction(a).id == 0);
    }
    {
        typedef IncompleteHolder* VT;
        typedef A<VT> Alloc;
        Alloc a;
        TC_ASSERT_EXPR(std::allocator_traits<Alloc>::select_on_container_copy_construction(a).id == 0);
    }
#if TEST_STD_VER >= 11
    {
        B<int> b;
        TC_ASSERT_EXPR(std::allocator_traits<B<int> >::select_on_container_copy_construction(b).id == 100);
    }
    {
        const B<int> b(0);
        TC_ASSERT_EXPR(std::allocator_traits<B<int> >::select_on_container_copy_construction(b).id == 100);
    }
#endif

    return true;
}

int tc_utilities_memory_allocator_traits_allocator_traits_members_select_on_container_copy_construction(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    return 0;
}
