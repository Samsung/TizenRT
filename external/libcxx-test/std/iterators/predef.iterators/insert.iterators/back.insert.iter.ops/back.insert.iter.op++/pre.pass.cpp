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

// back_insert_iterator

// back_insert_iterator<Cont>& operator++();

#include <iterator>
#include <vector>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"
#include "nasty_containers.hpp"

template <class C>
static int
test(C c)
{
    std::back_insert_iterator<C> i(c);
    std::back_insert_iterator<C>& r = ++i;
    TC_ASSERT_EXPR(&r == &i);
    return 0;
}

int tc_libcxx_iterators_back_insert_iter_op___pre(void)
{
    TC_ASSERT_FUNC((test(std::vector<int>())));
    TC_ASSERT_FUNC((test(nasty_vector<int>())));
    TC_SUCCESS_RESULT();
    return 0;
}
