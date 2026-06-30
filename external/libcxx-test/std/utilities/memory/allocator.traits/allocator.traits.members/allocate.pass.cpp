//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// <memory>

// template <class Alloc>
// struct allocator_traits
// {
//     static constexpr pointer allocate(allocator_type& a, size_type n);
//     ...
// };

#include <memory>
#include <cstdint>
#include <cassert>

#include "test_macros.h"
#include "incomplete_type_helper.h"
#include "libcxx_tc_common.h"

template <class T>
struct A
{
    typedef T value_type;

    TEST_CONSTEXPR_CXX20 A() {}

    TEST_CONSTEXPR_CXX20 value_type* allocate(std::size_t n)
    {
        TC_ASSERT_EXPR(n == 10);
        return &storage;
    }

    value_type storage;
};

TEST_CONSTEXPR_CXX20 bool test()
{
    {
        A<int> a;
        TC_ASSERT_EXPR(std::allocator_traits<A<int> >::allocate(a, 10) == &a.storage);
    }
    {
        typedef A<IncompleteHolder*> Alloc;
        Alloc a;
        TC_ASSERT_EXPR(std::allocator_traits<Alloc>::allocate(a, 10) == &a.storage);
    }

    return true;
}

int tc_utilities_memory_allocator_traits_allocator_traits_members_allocate(void) {
    test();
#if TEST_STD_VER > 17
    static_assert(test());
#endif
    return 0;
}
