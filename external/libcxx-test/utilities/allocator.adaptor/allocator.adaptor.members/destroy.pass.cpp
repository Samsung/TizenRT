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
