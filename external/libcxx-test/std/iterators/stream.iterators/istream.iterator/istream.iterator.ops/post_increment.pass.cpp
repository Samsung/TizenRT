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

// istream_iterator operator++(int);

#include <iterator>
#include <sstream>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_iterators_istream_iterator_ops_post_increment(void)
{
    std::istringstream inf(" 1 23");
    std::istream_iterator<int> i(inf);
    std::istream_iterator<int> icopy = i++;
    TC_ASSERT_EXPR(icopy == i);
    int j = 0;
    j = *i;
    TC_ASSERT_EXPR(j == 23);
    j = 0;
    j = *icopy;
    TC_ASSERT_EXPR(j == 1);
    TC_SUCCESS_RESULT();
    return 0;
}
