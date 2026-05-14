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

// template <class T> void destroy(T* p);

#include <scoped_allocator>
#include <cassert>
#include <string>

#include "test_macros.h"
#include "allocators.h"
#include "libcxx_tc_common.h"

struct B
{
    static bool constructed;

    B() {constructed = true;}
    ~B() {constructed = false;}
};

bool B::constructed = false;

int tc_utilities_allocator_adaptor_allocator_adaptor_members_destroy(void) {
    {
        typedef std::scoped_allocator_adaptor<A1<B>> A;
        A a;
        char buf[100];
        typedef B S;
        S* s = (S*)buf;
        TC_ASSERT_EXPR(!S::constructed);
        a.construct(s);
        TC_ASSERT_EXPR(S::constructed);
        a.destroy(s);
        TC_ASSERT_EXPR(!S::constructed);
    }

    {
        typedef std::scoped_allocator_adaptor<A3<B>, A1<B>> A;
        A a;
        char buf[100];
        typedef B S;
        S* s = (S*)buf;
        TC_ASSERT_EXPR(!S::constructed);
        TC_ASSERT_EXPR(!A3<S>::constructed);
        TC_ASSERT_EXPR(!A3<S>::destroy_called);
        a.construct(s);
        TC_ASSERT_EXPR(S::constructed);
        TC_ASSERT_EXPR(A3<S>::constructed);
        TC_ASSERT_EXPR(!A3<S>::destroy_called);
        a.destroy(s);
        TC_ASSERT_EXPR(!S::constructed);
        TC_ASSERT_EXPR(A3<S>::constructed);
        TC_ASSERT_EXPR(A3<S>::destroy_called);
    }


  return 0;
}
