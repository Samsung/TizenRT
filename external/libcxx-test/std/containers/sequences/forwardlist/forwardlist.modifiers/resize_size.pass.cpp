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

// static int resize(size_type n);

#include <forward_list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "DefaultOnly.h"

int tc_libcxx_containers_forwardlist_modifiers_resize_size(void)
{
    {
        typedef DefaultOnly T;
        typedef std::forward_list<T> C;
        C c;
        c.resize(0);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 0);
        c.resize(10);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 10);
        c.resize(20);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 20);
        c.resize(5);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 5);
        c.resize(0);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 0);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t[] = {0, 1, 2, 3, 4};
        C c(std::begin(t), std::end(t));

        c.resize(3);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 3);
        TC_ASSERT_EXPR(*next(c.begin(), 0) == 0);
        TC_ASSERT_EXPR(*next(c.begin(), 1) == 1);
        TC_ASSERT_EXPR(*next(c.begin(), 2) == 2);

        c.resize(6);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 6);
        TC_ASSERT_EXPR(*next(c.begin(), 0) == 0);
        TC_ASSERT_EXPR(*next(c.begin(), 1) == 1);
        TC_ASSERT_EXPR(*next(c.begin(), 2) == 2);
        TC_ASSERT_EXPR(*next(c.begin(), 3) == 0);
        TC_ASSERT_EXPR(*next(c.begin(), 4) == 0);
        TC_ASSERT_EXPR(*next(c.begin(), 5) == 0);

        c.resize(6);
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 6);
        TC_ASSERT_EXPR(*next(c.begin(), 0) == 0);
        TC_ASSERT_EXPR(*next(c.begin(), 1) == 1);
        TC_ASSERT_EXPR(*next(c.begin(), 2) == 2);
        TC_ASSERT_EXPR(*next(c.begin(), 3) == 0);
        TC_ASSERT_EXPR(*next(c.begin(), 4) == 0);
        TC_ASSERT_EXPR(*next(c.begin(), 5) == 0);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
