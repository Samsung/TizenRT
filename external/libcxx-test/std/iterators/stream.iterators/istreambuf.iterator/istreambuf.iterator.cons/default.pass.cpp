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
//
// istreambuf_iterator() throw();
//
// All specializations of istreambuf_iterator shall have a trivial copy constructor,
//    a constexpr default constructor and a trivial destructor.

#include <iterator>
#include <sstream>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

int tc_libcxx_iterators_istreambuf_iterator_cons_default(void)
{
    {
        typedef std::istreambuf_iterator<char> T;
        T it;
        TC_ASSERT_EXPR(it == T());
#if TEST_STD_VER >= 11
        constexpr T it2;
        (void)it2;
#endif
    }
#ifndef TEST_HAS_NO_WIDE_CHARACTERS
    {
        typedef std::istreambuf_iterator<wchar_t> T;
        T it;
        TC_ASSERT_EXPR(it == T());
#if TEST_STD_VER >= 11
        constexpr T it2;
        (void)it2;
#endif
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
