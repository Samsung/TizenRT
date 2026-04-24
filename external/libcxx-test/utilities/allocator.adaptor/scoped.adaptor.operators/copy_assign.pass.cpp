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

// scoped_allocator_adaptor& operator=(const scoped_allocator_adaptor& other);


#include <scoped_allocator>
#include <cassert>

#include "test_macros.h"
#include "allocators.h"
#include "libcxx_tc_common.h"

int tc_utilities_allocator_adaptor_scoped_adaptor_operators_copy_assign(void) {
    {
        typedef std::scoped_allocator_adaptor<A1<int>> A;
        A a1(A1<int>(3));
        A aN;
        A1<int>::copy_called = false;
        A1<int>::move_called = false;
        aN = a1;
        TC_ASSERT_EXPR(A1<int>::copy_called == true);
        TC_ASSERT_EXPR(A1<int>::move_called == false);
        TC_ASSERT_EXPR(aN == a1);
    }
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>> A;
        A a1(A1<int>(4), A2<int>(5));
        A aN;
        A1<int>::copy_called = false;
        A1<int>::move_called = false;
        A2<int>::copy_called = false;
        A2<int>::move_called = false;
        aN = a1;
        TC_ASSERT_EXPR(A1<int>::copy_called == true);
        TC_ASSERT_EXPR(A1<int>::move_called == false);
        TC_ASSERT_EXPR(A2<int>::copy_called == true);
        TC_ASSERT_EXPR(A2<int>::move_called == false);
        TC_ASSERT_EXPR(aN == a1);
    }
    {
        typedef std::scoped_allocator_adaptor<A1<int>, A2<int>, A3<int>> A;
        A a1(A1<int>(4), A2<int>(5), A3<int>(6));
        A aN;
        A1<int>::copy_called = false;
        A1<int>::move_called = false;
        A2<int>::copy_called = false;
        A2<int>::move_called = false;
        A3<int>::copy_called = false;
        A3<int>::move_called = false;
        aN = a1;
        TC_ASSERT_EXPR(A1<int>::copy_called == true);
        TC_ASSERT_EXPR(A1<int>::move_called == false);
        TC_ASSERT_EXPR(A2<int>::copy_called == true);
        TC_ASSERT_EXPR(A2<int>::move_called == false);
        TC_ASSERT_EXPR(A3<int>::copy_called == true);
        TC_ASSERT_EXPR(A3<int>::move_called == false);
        TC_ASSERT_EXPR(aN == a1);
    }

  return 0;
}
