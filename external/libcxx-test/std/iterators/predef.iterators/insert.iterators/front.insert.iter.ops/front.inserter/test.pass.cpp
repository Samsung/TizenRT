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

// <iterator>

// template <BackInsertionContainer Cont>
//   front_insert_iterator<Cont>
//   front_inserter(Cont& x);

#include <iterator>
#include <list>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include "nasty_containers.hpp"

template <class C>
static int
test(C c)
{
    std::front_insert_iterator<C> i = std::front_inserter(c);
    i = 0;
    TC_ASSERT_EXPR(c.size() == 1);
    TC_ASSERT_EXPR(c.front() == 0);
    return 0;
}

int tc_libcxx_iterators_front_inserter_test(void)
{
    TC_ASSERT_FUNC((test(std::list<int>())));
    TC_ASSERT_FUNC((test(nasty_list<int>())));
    TC_SUCCESS_RESULT();
    return 0;
}
