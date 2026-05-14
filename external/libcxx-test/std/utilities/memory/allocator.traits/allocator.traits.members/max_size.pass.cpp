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
//     static constexpr size_type max_size(const allocator_type& a) noexcept;
//     ...
// };

#include <limits>
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

};

template <class T>
struct B
{
    typedef T value_type;

    TEST_CONSTEXPR_CXX20 std::size_t max_size() const
    {
        return 100;
    }
};

TEST_CONSTEXPR_CXX20 bool test()
{
    {
        B<int> b;
        TC_ASSERT_EXPR(std::allocator_traits<B<int> >::max_size(b) == 100);
    }
    {
        const B<int> b = {};
        TC_ASSERT_EXPR(std::allocator_traits<B<int> >::max_size(b) == 100);
    }
    {
        typedef IncompleteHolder* VT;
        typedef B<VT> Alloc;
        Alloc a;
        TC_ASSERT_EXPR(std::allocator_traits<Alloc >::max_size(a) == 100);
    }
#if TEST_STD_VER >= 11
    {
        A<int> a;
        TC_ASSERT_EXPR(std::allocator_traits<A<int> >::max_size(a) ==
               std::numeric_limits<std::size_t>::max() / sizeof(int));
    }
    {
        const A<int> a = {};
        TC_ASSERT_EXPR(std::allocator_traits<A<int> >::max_size(a) ==
               std::numeric_limits<std::size_t>::max() / sizeof(int));
    }
    {
        std::allocator<int> a;
        static_assert(noexcept(std::allocator_traits<std::allocator<int>>::max_size(a)) == true, "");
    }
#endif

    return true;
}

int tc_utilities_memory_allocator_traits_allocator_traits_members_max_size(void) {
    test();

#if TEST_STD_VER > 17
    static_assert(test());
#endif

    return 0;
}
