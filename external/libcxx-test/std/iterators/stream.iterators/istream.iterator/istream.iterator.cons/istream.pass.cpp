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

// class istream_iterator

// istream_iterator(istream_type& s);

#include <iterator>
#include <sstream>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_iterators_istream_iterator_cons_istream(void)
{
    std::istringstream inf(" 1 23");
    std::istream_iterator<int> i(inf);
    TC_ASSERT_EXPR(i != std::istream_iterator<int>());
    TC_ASSERT_EXPR(inf.peek() == ' ');
    TC_ASSERT_EXPR(inf.good());
    int j = 0;
    inf >> j;
    TC_ASSERT_EXPR(j == 23);
    TC_SUCCESS_RESULT();
    return 0;
}
