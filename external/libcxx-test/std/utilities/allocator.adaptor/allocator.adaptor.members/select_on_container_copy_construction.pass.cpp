//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <memory>

// template <class OuterAlloc, class... InnerAllocs>
//   class scoped_allocator_adaptor

// scoped_allocator_adaptor select_on_container_copy_construction() const;

#include <scoped_allocator>
#include <cassert>

#include "test_macros.h"
#include "allocators.h"
#include "libcxx_tc_common.h"

int tc_utilities_allocator_adaptor_allocator_adaptor_members_select_on_container_copy_construction(void) {
    {
        typedef std::scoped_allocator_adaptor<A1<int>> A;
        A a1(A1<int>(3));
        TC_ASSERT_EXPR(a1.outer_allocator().id() == 3);
        A a2 = std::allocator_traits<A>::select_on_container_copy_construction(a1);
        TC_ASSERT_EXPR(a2.outer_allocator().id() == 3);
    }

    {
        typedef std::scoped_allocator_adaptor<A3<int>> A;
        A a1(A3<int>(3));
        TC_ASSERT_EXPR(a1.outer_allocator().id() == 3);
        A a2 = std::allocator_traits<A>::select_on_container_copy_construction(a1);
        TC_ASSERT_EXPR(a2.outer_allocator().id() == -1);
    }

    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>, A3<int>> A;
        A a1(A1<int>(1), A2<int>(2), A3<int>(3));
        TC_ASSERT_EXPR(a1.outer_allocator().id() == 1);
        TC_ASSERT_EXPR(a1.inner_allocator().outer_allocator().id() == 2);
        TC_ASSERT_EXPR(a1.inner_allocator().inner_allocator().outer_allocator().id() == 3);
        A a2 = std::allocator_traits<A>::select_on_container_copy_construction(a1);
        TC_ASSERT_EXPR(a2.outer_allocator().id() == 1);
        TC_ASSERT_EXPR(a2.inner_allocator().outer_allocator().id() == 2);
        TC_ASSERT_EXPR(a2.inner_allocator().inner_allocator().outer_allocator().id() == -1);
    }


  return 0;
}
