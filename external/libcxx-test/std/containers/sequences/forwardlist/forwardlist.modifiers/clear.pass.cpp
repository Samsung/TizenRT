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

// static int clear();

#include <forward_list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "NotConstructible.h"

int tc_libcxx_containers_forwardlist_modifiers_clear(void)
{
    {
        typedef NotConstructible T;
        typedef std::forward_list<T> C;
        C c;
        c.clear();
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 0);
    }
    {
        typedef int T;
        typedef std::forward_list<T> C;
        const T t[] = {0, 1, 2, 3, 4};
        C c(std::begin(t), std::end(t));

        c.clear();
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 0);

        c.clear();
        TC_ASSERT_EXPR(distance(c.begin(), c.end()) == 0);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
