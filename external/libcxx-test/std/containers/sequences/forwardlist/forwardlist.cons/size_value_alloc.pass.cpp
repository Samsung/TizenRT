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

// <forward_list>

// forward_list(size_type n, const value_type& v, const allocator_type& a);

#include <forward_list>
#include <cassert>
#include "test_macros.h"
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
