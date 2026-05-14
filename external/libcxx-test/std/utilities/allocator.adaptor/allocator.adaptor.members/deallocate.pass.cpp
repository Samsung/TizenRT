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

// void deallocate(pointer p, size_type n);

#include <scoped_allocator>
#include <cassert>

#include "test_macros.h"
#include "allocators.h"
#include "libcxx_tc_common.h"

int tc_utilities_allocator_adaptor_allocator_adaptor_members_deallocate(void) {

    {
        typedef std::scoped_allocator_adaptor<A1<int>> A;
        A a;
        a.deallocate((int*)10, 20);
        TC_ASSERT_EXPR((A1<int>::deallocate_called == std::pair<int*, std::size_t>((int*)10, 20)));
    }
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>> A;
        A a;
        a.deallocate((int*)10, 20);
        TC_ASSERT_EXPR((A1<int>::deallocate_called == std::pair<int*, std::size_t>((int*)10, 20)));
    }
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>, A3<int>> A;
        A a;
        a.deallocate((int*)10, 20);
        TC_ASSERT_EXPR((A1<int>::deallocate_called == std::pair<int*, std::size_t>((int*)10, 20)));
    }


  return 0;
}
