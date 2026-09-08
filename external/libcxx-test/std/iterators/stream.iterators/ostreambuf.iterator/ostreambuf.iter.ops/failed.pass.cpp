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

// class ostreambuf_iterator

// bool failed() const throw();

#include <iterator>
#include <sstream>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"

int tc_libcxx_iterators_ostreambuf_iter_ops_failed(void)
{
    {
        std::ostreambuf_iterator<char> i(nullptr);
        TC_ASSERT_EXPR(i.failed());
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        std::ostreambuf_iterator<wchar_t> i(nullptr);
        TC_ASSERT_EXPR(i.failed());
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
