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

// <vector>

// vector(const vector& v, const allocator_type& a);

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_allocator.h"

template <class C>
static int
test(const C& x, const typename C::allocator_type& a)
{
    typename C::size_type s = x.size();
    C c(x, a);
    LIBCPP_ASSERT(c.__invariants());
    TC_ASSERT_EXPR(c.size() == s);
    TC_ASSERT_EXPR(c == x);
    return 0;
}

int tc_libcxx_containers_vector_bool_copy_alloc(void)
{
    {
        bool a[] = {0, 1, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0};
        bool* an = a + sizeof(a)/sizeof(a[0]);
        test(std::vector<bool>(a, an), std::allocator<bool>());
    }
    {
        std::vector<bool, test_allocator<bool> > l(3, true, test_allocator<bool>(5));
        std::vector<bool, test_allocator<bool> > l2(l, test_allocator<bool>(3));
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == test_allocator<bool>(3));
    }
    {
        std::vector<bool, other_allocator<bool> > l(3, true, other_allocator<bool>(5));
        std::vector<bool, other_allocator<bool> > l2(l, other_allocator<bool>(3));
        TC_ASSERT_EXPR(l2 == l);
        TC_ASSERT_EXPR(l2.get_allocator() == other_allocator<bool>(3));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
