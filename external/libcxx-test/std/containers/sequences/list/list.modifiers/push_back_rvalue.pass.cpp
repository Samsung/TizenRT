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

// UNSUPPORTED: c++98, c++03

// <list>

// static int push_back(value_type&& x);

#include <list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

#include "MoveOnly.h"

int tc_libcxx_containers_list_modifiers_push_back_rvalue(void)
{
    {
    std::list<MoveOnly> l1;
    l1.push_back(MoveOnly(1));
    TC_ASSERT_EXPR(l1.size() == 1);
    TC_ASSERT_EXPR(l1.front() == MoveOnly(1));
    l1.push_back(MoveOnly(2));
    TC_ASSERT_EXPR(l1.size() == 2);
    TC_ASSERT_EXPR(l1.front() == MoveOnly(1));
    TC_ASSERT_EXPR(l1.back() == MoveOnly(2));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
