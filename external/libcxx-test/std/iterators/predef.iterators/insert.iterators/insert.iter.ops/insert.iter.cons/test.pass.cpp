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

// insert_iterator

// insert_iterator(Cont& x, Cont::iterator i);

#include <iterator>
#include <vector>
#include "nasty_containers.hpp"
#include "test_macros.h"
#include "libcxx_tc_common.h"

template <class C>
static int
test(C c)
{
    std::insert_iterator<C> i(c, c.begin());
    return 0;
}

int tc_libcxx_iterators_insert_iter_cons_test(void)
{
    TC_ASSERT_FUNC((test(std::vector<int>())));
    TC_ASSERT_FUNC((test(nasty_vector<int>())));
    TC_SUCCESS_RESULT();
    return 0;
}
