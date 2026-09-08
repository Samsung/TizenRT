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

// explicit move_iterator(Iter i);
//
//  constexpr in C++17

#include <iterator>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_iterators.h"

template <class It>
static int
test(It i)
{
    std::move_iterator<It> r(i);
    TC_ASSERT_EXPR(r.base() == i);
    return 0;
}

int tc_libcxx_iterators_move_iter_op_const_iter(void)
{
    char s[] = "123";
    TC_ASSERT_FUNC((test(input_iterator<char*>(s))));
    TC_ASSERT_FUNC((test(forward_iterator<char*>(s))));
    TC_ASSERT_FUNC((test(bidirectional_iterator<char*>(s))));
    TC_ASSERT_FUNC((test(random_access_iterator<char*>(s))));
    TC_ASSERT_FUNC((test(s)));

#if TEST_STD_VER > 14
    {
    constexpr const char *p = "123456789";
    constexpr std::move_iterator<const char *> it(p);
    static_assert(it.base() == p);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
