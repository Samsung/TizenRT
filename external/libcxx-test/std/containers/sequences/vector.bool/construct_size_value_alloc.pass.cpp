//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// <vector>
// vector<bool>

// vector(size_type n, const value_type& x, const allocator_type& a);

#include <vector>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class C>
static int
test(typename C::size_type n, const typename C::value_type& x,
     const typename C::allocator_type& a)
{
    C c(n, x, a);
    LIBCPP_ASSERT(c.__invariants());
    TC_ASSERT_EXPR(a == c.get_allocator());
    TC_ASSERT_EXPR(c.size() == n);
    for (typename C::const_iterator i = c.cbegin(), e = c.cend(); i != e; ++i)
        TC_ASSERT_EXPR(*i == x);
    return 0;
}

int tc_libcxx_containers_vector_bool_construct_size_value_alloc(void)
{
    TC_ASSERT_FUNC((test<std::vector<bool> >(50, true, std::allocator<bool>())));
    TC_SUCCESS_RESULT();
    return 0;
}
