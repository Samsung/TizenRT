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

// template <class OuterA1, class OuterA2, class... InnerAllocs>
//     bool
//     operator==(const scoped_allocator_adaptor<OuterA1, InnerAllocs...>& a,
//                const scoped_allocator_adaptor<OuterA2, InnerAllocs...>& b);
//
// template <class OuterA1, class OuterA2, class... InnerAllocs>
//     bool
//     operator!=(const scoped_allocator_adaptor<OuterA1, InnerAllocs...>& a,
//                const scoped_allocator_adaptor<OuterA2, InnerAllocs...>& b);

#include <scoped_allocator>
#include <cassert>

#include "test_macros.h"
#include "allocators.h"
#include "libcxx_tc_common.h"

int tc_utilities_allocator_adaptor_scoped_adaptor_operators_eq(void) {
    {
        typedef std::scoped_allocator_adaptor<A1<int>> A;
        A a1(A1<int>(3));
        A a2 = a1;
        TC_ASSERT_EXPR(a2 == a1);
        TC_ASSERT_EXPR(!(a2 != a1));
    }
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>> A;
        A a1(A1<int>(4), A2<int>(5));
        A a2 = a1;
        TC_ASSERT_EXPR(a2 == a1);
        TC_ASSERT_EXPR(!(a2 != a1));
    }
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>, A3<int>> A;
        A a1(A1<int>(4), A2<int>(5), A3<int>(6));
        A a2 = a1;
        TC_ASSERT_EXPR(a2 == a1);
        TC_ASSERT_EXPR(!(a2 != a1));
    }
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>, A3<int>> A;
        A a1(A1<int>(4), A2<int>(5), A3<int>(6));
        A a2(A1<int>(4), A2<int>(5), A3<int>(5));
        TC_ASSERT_EXPR(a2 != a1);
        TC_ASSERT_EXPR(!(a2 == a1));
    }

  return 0;
}
