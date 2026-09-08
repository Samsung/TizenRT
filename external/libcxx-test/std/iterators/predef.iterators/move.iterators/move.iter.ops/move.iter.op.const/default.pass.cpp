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

// move_iterator

// move_iterator();
//
//  constexpr in C++17

#include <iterator>

#include "test_macros.h"
#include "test_iterators.h"
#include "libcxx_tc_common.h"

template <class It>
static int
test()
{
    std::move_iterator<It> r;
    (void)r;
    return 0;
}

int tc_libcxx_iterators_move_iter_op_const_default(void)
{
    TC_ASSERT_FUNC((test<input_iterator<char*> >()));
    TC_ASSERT_FUNC((test<forward_iterator<char*> >()));
    TC_ASSERT_FUNC((test<bidirectional_iterator<char*> >()));
    TC_ASSERT_FUNC((test<random_access_iterator<char*> >()));
    TC_ASSERT_FUNC((test<char*>()));

#if TEST_STD_VER > 14
    {
    constexpr std::move_iterator<const char *> it;
    (void)it;
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
