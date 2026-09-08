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

// static int assign(size_type n, const value_type& v);

#include <forward_list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include <iterator>


int tc_libcxx_containers_forwardlist_cons_assign_size_value(void)
{
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t1[] = {10, 11, 12, 13};
        C c(std::begin(t1), std::end(t1));
        c.assign(10, 1);
        int n = 0;
        for (C::const_iterator i = c.cbegin(); i != c.cend(); ++i, ++n)
            TC_ASSERT_EXPR(*i == 1);
        TC_ASSERT_EXPR(n == 10);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t1[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
        C c(std::begin(t1), std::end(t1));
        c.assign(4, 10);
        int n = 0;
        for (C::const_iterator i = c.cbegin(); i != c.cend(); ++i, ++n)
            TC_ASSERT_EXPR(*i == 10);
        TC_ASSERT_EXPR(n == 4);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
