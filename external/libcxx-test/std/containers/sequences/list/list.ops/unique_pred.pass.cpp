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

// <list>

// template <class BinaryPred> static int unique(BinaryPred pred);

#include <list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


static bool g(int x, int y)
{
    return x == y;
    return 0;
}

int tc_libcxx_containers_list_ops_unique_pred(void)
{
    {
    int a1[] = {2, 1, 1, 4, 4, 4, 4, 3, 3};
    int a2[] = {2, 1, 4, 3};
    std::list<int> c(a1, a1+sizeof(a1)/sizeof(a1[0]));
    c.unique(g);
    TC_ASSERT_EXPR(c == std::list<int>(a2, a2+4));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
