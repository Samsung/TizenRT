//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++03

// <scoped_allocator>

// template <class OuterAlloc, class... InnerAllocs>
//   class scoped_allocator_adaptor

// template <class OuterA2>
//   scoped_allocator_adaptor(OuterA2&& outerAlloc,
//                            const InnerAllocs& ...innerAllocs);

#include <scoped_allocator>
#include <cassert>

#include "test_macros.h"
#include "allocators.h"
#include "libcxx_tc_common.h"

int tc_utilities_allocator_adaptor_allocator_adaptor_cnstr_allocs(void) {
    {
        typedef std::scoped_allocator_adaptor<A1<int>> A;
        A1<int> a3(3);
        A a(a3);
        TC_ASSERT_EXPR(a.outer_allocator() == A1<int>(3));
        TC_ASSERT_EXPR(a.inner_allocator() == a);
        TC_ASSERT_EXPR(A1<int>::copy_called == true);
        TC_ASSERT_EXPR(A1<int>::move_called == false);
    }
    A1<int>::copy_called = false;
    {
        typedef std::scoped_allocator_adaptor<A1<int>> A;
        A a(A1<int>(3));
        TC_ASSERT_EXPR(a.outer_allocator() == A1<int>(3));
        TC_ASSERT_EXPR(a.inner_allocator() == a);
        TC_ASSERT_EXPR(A1<int>::copy_called == false);
        TC_ASSERT_EXPR(A1<int>::move_called == true);
    }
    A1<int>::move_called = false;
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>> A;
        A1<int> a4(4);
        A a(a4, A2<int>(5));
        TC_ASSERT_EXPR(A1<int>::copy_called == true);
        TC_ASSERT_EXPR(A1<int>::move_called == false);
        TC_ASSERT_EXPR(A2<int>::copy_called == true);
        TC_ASSERT_EXPR(A2<int>::move_called == false);
        TC_ASSERT_EXPR(a.outer_allocator() == A1<int>(4));
        TC_ASSERT_EXPR(a.inner_allocator() == std::scoped_allocator_adaptor<A2<int>>(A2<int>(5)));
    }
    A1<int>::copy_called = false;
    A1<int>::move_called = false;
    A2<int>::copy_called = false;
    A2<int>::move_called = false;
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>> A;
        A a(A1<int>(4), A2<int>(5));
        TC_ASSERT_EXPR(A1<int>::copy_called == false);
        TC_ASSERT_EXPR(A1<int>::move_called == true);
        TC_ASSERT_EXPR(A2<int>::copy_called == true);
        TC_ASSERT_EXPR(A2<int>::move_called == false);
        TC_ASSERT_EXPR(a.outer_allocator() == A1<int>(4));
        TC_ASSERT_EXPR(a.inner_allocator() == std::scoped_allocator_adaptor<A2<int>>(A2<int>(5)));
    }
    A1<int>::copy_called = false;
    A1<int>::move_called = false;
    A2<int>::copy_called = false;
    A2<int>::move_called = false;
    A1<int>::move_called = false;
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>, A3<int>> A;
        A1<int> a4(4);
        A a(a4, A2<int>(5), A3<int>(6));
        TC_ASSERT_EXPR(A1<int>::copy_called == true);
        TC_ASSERT_EXPR(A1<int>::move_called == false);
        TC_ASSERT_EXPR(A2<int>::copy_called == true);
        TC_ASSERT_EXPR(A2<int>::move_called == false);
        TC_ASSERT_EXPR(A3<int>::copy_called == true);
        TC_ASSERT_EXPR(A3<int>::move_called == false);
        TC_ASSERT_EXPR(a.outer_allocator() == A1<int>(4));
        TC_ASSERT_EXPR((a.inner_allocator() ==
            std::scoped_allocator_adaptor<A2<int>, A3<int>>(A2<int>(5), A3<int>(6))));
    }
    A1<int>::copy_called = false;
    A1<int>::move_called = false;
    A2<int>::copy_called = false;
    A2<int>::move_called = false;
    A3<int>::copy_called = false;
    A3<int>::move_called = false;
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>, A3<int>> A;
        A a(A1<int>(4), A2<int>(5), A3<int>(6));
        TC_ASSERT_EXPR(A1<int>::copy_called == false);
        TC_ASSERT_EXPR(A1<int>::move_called == true);
        TC_ASSERT_EXPR(A2<int>::copy_called == true);
        TC_ASSERT_EXPR(A2<int>::move_called == false);
        TC_ASSERT_EXPR(A3<int>::copy_called == true);
        TC_ASSERT_EXPR(A3<int>::move_called == false);
        TC_ASSERT_EXPR(a.outer_allocator() == A1<int>(4));
        TC_ASSERT_EXPR((a.inner_allocator() ==
            std::scoped_allocator_adaptor<A2<int>, A3<int>>(A2<int>(5), A3<int>(6))));
    }
//  Test for LWG2782
    {
        static_assert(!std::is_convertible<A1<int>, A2<int>>::value, "");
        static_assert(!std::is_convertible<
             std::scoped_allocator_adaptor<A1<int>>,
             std::scoped_allocator_adaptor<A2<int>>>::value, "");
    }

  return 0;
}
