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
