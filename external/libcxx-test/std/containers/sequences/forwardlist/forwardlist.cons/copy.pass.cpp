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

// forward_list(const forward_list& x);

#include <forward_list>
#include <cassert>
#include "libcxx_tc_common.h"
#include <iterator>

#include "test_macros.h"
#include "test_allocator.h"

int tc_libcxx_containers_forwardlist_cons_copy(void)
{
    {
        typedef int T;
        typedef test_allocator<int> A;
        typedef std::forward_list<T, A> C;
        const T t[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        C c0(std::begin(t), std::end(t), A(10));
        C c = c0;
        int n = 0;
        for (C::const_iterator i = c.begin(), e = c.end(); i != e; ++i, ++n)
            TC_ASSERT_EXPR(*i == n);
        TC_ASSERT_EXPR(n == std::end(t) - std::begin(t));
        TC_ASSERT_EXPR(c == c0);
        TC_ASSERT_EXPR(c.get_allocator() == A(10));
    }
#if TEST_STD_VER >= 11
    {
        typedef int T;
        typedef other_allocator<int> A;
        typedef std::forward_list<T, A> C;
        const T t[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        C c0(std::begin(t), std::end(t), A(10));
        C c = c0;
        int n = 0;
        for (C::const_iterator i = c.begin(), e = c.end(); i != e; ++i, ++n)
            TC_ASSERT_EXPR(*i == n);
        TC_ASSERT_EXPR(n == std::end(t) - std::begin(t));
        TC_ASSERT_EXPR(c == c0);
        TC_ASSERT_EXPR(c.get_allocator() == A(-2));
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
