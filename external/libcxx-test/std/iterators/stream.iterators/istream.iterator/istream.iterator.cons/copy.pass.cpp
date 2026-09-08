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

// istream_iterator(const istream_iterator& x);
//  C++17 says:  If is_trivially_copy_constructible_v<T> is true, then
//     this constructor is a trivial copy constructor.

#include <iterator>
#include <sstream>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

int tc_libcxx_iterators_istream_iterator_cons_copy(void)
{
    {
        std::istream_iterator<int> io;
        std::istream_iterator<int> i = io;
        TC_ASSERT_EXPR(i == std::istream_iterator<int>());
    }
    {
        std::istringstream inf(" 1 23");
        std::istream_iterator<int> io(inf);
        std::istream_iterator<int> i = io;
        TC_ASSERT_EXPR(i != std::istream_iterator<int>());
        int j = 0;
        j = *i;
        TC_ASSERT_EXPR(j == 1);
    }
    TC_SUCCESS_RESULT();
    return 0;
}
