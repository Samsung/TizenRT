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

// istreambuf_iterator

// istreambuf_iterator(const proxy& p) throw();

#include <iterator>
#include <sstream>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_iterators_istreambuf_iterator_cons_proxy(void)
{
    {
        std::istringstream inf("abc");
        std::istreambuf_iterator<char> j(inf);
        std::istreambuf_iterator<char> i = j++;
        TC_ASSERT_EXPR(i != std::istreambuf_iterator<char>());
        TC_ASSERT_EXPR(*i == 'b');
    }
    TC_SUCCESS_RESULT();
    return 0;
}
