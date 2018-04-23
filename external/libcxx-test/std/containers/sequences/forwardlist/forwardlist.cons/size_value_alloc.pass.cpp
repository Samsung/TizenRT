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
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <forward_list>

// forward_list(size_type n, const value_type& v, const allocator_type& a);

#include <forward_list>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_allocator.h"

int tc_libcxx_containers_forwardlist_cons_size_value_alloc(void)
{
    {
        typedef test_allocator<int> A;
        typedef A::value_type T;
        typedef std::forward_list<T, A> C;
        T v(6);
        unsigned N = 10;
        C c(N, v, A(12));
        unsigned n = 0;
        for (C::const_iterator i = c.begin(), e = c.end(); i != e; ++i, ++n)
            TC_ASSERT_EXPR(*i == v);
        TC_ASSERT_EXPR(n == N);
        TC_ASSERT_EXPR(c.get_allocator() == A(12));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
