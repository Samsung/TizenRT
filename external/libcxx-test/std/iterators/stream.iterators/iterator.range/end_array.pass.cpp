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

// template <class T, size_t N> T* end(T (&array)[N]);

#include <iterator>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_iterators_iterator_range_end_array(void)
{
    int ia[] = {1, 2, 3};
    int* i = std::begin(ia);
    int* e = std::end(ia);
    TC_ASSERT_EXPR(e == ia + 3);
    TC_ASSERT_EXPR(e - i == 3);
    TC_SUCCESS_RESULT();
    return 0;
}
