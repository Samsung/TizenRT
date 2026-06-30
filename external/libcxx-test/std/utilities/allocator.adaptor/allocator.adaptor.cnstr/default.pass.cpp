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

// scoped_allocator_adaptor();

#include <scoped_allocator>
#include <cassert>

#include "test_macros.h"
#include "allocators.h"
#include "libcxx_tc_common.h"

int tc_utilities_allocator_adaptor_allocator_adaptor_cnstr_default(void) {
    {
        typedef std::scoped_allocator_adaptor<A1<int>> A;
        A a;
        TC_ASSERT_EXPR(a.outer_allocator() == A1<int>());
        TC_ASSERT_EXPR(a.inner_allocator() == a);
        TC_ASSERT_EXPR(A1<int>::copy_called == false);
        TC_ASSERT_EXPR(A1<int>::move_called == false);
    }
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>> A;
        A a;
        TC_ASSERT_EXPR(a.outer_allocator() == A1<int>());
        TC_ASSERT_EXPR(a.inner_allocator() == std::scoped_allocator_adaptor<A2<int>>());
        TC_ASSERT_EXPR(A1<int>::copy_called == false);
        TC_ASSERT_EXPR(A1<int>::move_called == false);
        TC_ASSERT_EXPR(A2<int>::copy_called == false);
        TC_ASSERT_EXPR(A2<int>::move_called == false);
    }
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>, A3<int>> A;
        A a;
        TC_ASSERT_EXPR(a.outer_allocator() == A1<int>());
        TC_ASSERT_EXPR((a.inner_allocator() == std::scoped_allocator_adaptor<A2<int>, A3<int>>()));
        TC_ASSERT_EXPR(A1<int>::copy_called == false);
        TC_ASSERT_EXPR(A1<int>::move_called == false);
        TC_ASSERT_EXPR(A2<int>::copy_called == false);
        TC_ASSERT_EXPR(A2<int>::move_called == false);
        TC_ASSERT_EXPR(A3<int>::copy_called == false);
        TC_ASSERT_EXPR(A3<int>::move_called == false);
    }


  return 0;
}
