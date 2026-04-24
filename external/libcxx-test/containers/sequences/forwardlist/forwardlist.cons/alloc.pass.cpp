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
// <forward_list>

// explicit forward_list(const allocator_type& a);

#include <forward_list>
#include <cassert>

#include "test_macros.h"
#include "test_allocator.h"
#include "../../../NotConstructible.h"
#include "min_allocator.h"
#include "libcxx_tc_common.h"

int tc_containers_sequences_forwardlist_forwardlist_cons_alloc(void) {
    {
        typedef test_allocator<NotConstructible> A;
        typedef A::value_type T;
        typedef std::forward_list<T, A> C;
        C c(A(12));
        TC_ASSERT_EXPR(c.get_allocator() == A(12));
        TC_ASSERT_EXPR(c.empty());
    }
#if TEST_STD_VER >= 11
    {
        typedef min_allocator<NotConstructible> A;
        typedef A::value_type T;
        typedef std::forward_list<T, A> C;
        C c(A{});
        TC_ASSERT_EXPR(c.get_allocator() == A());
        TC_ASSERT_EXPR(c.empty());
    }
    {
        typedef explicit_allocator<NotConstructible> A;
        typedef A::value_type T;
        typedef std::forward_list<T, A> C;
        C c(A{});
        TC_ASSERT_EXPR(c.get_allocator() == A());
        TC_ASSERT_EXPR(c.empty());
    }
#endif

  return 0;
}
