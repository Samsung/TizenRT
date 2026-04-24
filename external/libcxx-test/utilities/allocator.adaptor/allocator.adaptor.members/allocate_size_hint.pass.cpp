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
// UNSUPPORTED: c++03

// <memory>

// template <class OuterAlloc, class... InnerAllocs>
//   class scoped_allocator_adaptor

// pointer allocate(size_type n, const_void_pointer hint);

#include <scoped_allocator>
#include <cassert>

#include "test_macros.h"
#include "allocators.h"
#include "libcxx_tc_common.h"

int tc_utilities_allocator_adaptor_allocator_adaptor_members_allocate_size_hint(void) {
    {
        typedef std::scoped_allocator_adaptor<A1<int>> A;
        A a;
        A1<int>::allocate_called = false;
        TC_ASSERT_EXPR(a.allocate(10, (const void*)0) == (int*)10);
        TC_ASSERT_EXPR(A1<int>::allocate_called == true);
    }
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>> A;
        A a;
        A1<int>::allocate_called = false;
        TC_ASSERT_EXPR(a.allocate(10, (const void*)10) == (int*)10);
        TC_ASSERT_EXPR(A1<int>::allocate_called == true);
    }
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>, A3<int>> A;
        A a;
        A1<int>::allocate_called = false;
        TC_ASSERT_EXPR(a.allocate(10, (const void*)20) == (int*)10);
        TC_ASSERT_EXPR(A1<int>::allocate_called == true);
    }

    {
        typedef std::scoped_allocator_adaptor<A2<int>> A;
        A a;
        A2<int>::allocate_called = false;
        TC_ASSERT_EXPR(a.allocate(10, (const void*)0) == (int*)0);
        TC_ASSERT_EXPR(A2<int>::allocate_called == true);
    }
    {
        typedef std::scoped_allocator_adaptor<A2<int>, A2<int>> A;
        A a;
        A2<int>::allocate_called = false;
        TC_ASSERT_EXPR(a.allocate(10, (const void*)10) == (int*)10);
        TC_ASSERT_EXPR(A2<int>::allocate_called == true);
    }
    {
        typedef std::scoped_allocator_adaptor<A2<int>, A2<int>, A3<int>> A;
        A a;
        A2<int>::allocate_called = false;
        TC_ASSERT_EXPR(a.allocate(10, (const void*)20) == (int*)20);
        TC_ASSERT_EXPR(A2<int>::allocate_called == true);
    }

  return 0;
}
