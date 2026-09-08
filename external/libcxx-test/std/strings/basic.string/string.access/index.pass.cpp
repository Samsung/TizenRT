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

// <string>

// const_reference operator[](size_type pos) const;
//       reference operator[](size_type pos);

#ifdef _LIBCPP_DEBUG
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


int tc_libcxx_strings_string_access_index(void)
{
    {
    typedef std::string S;
    S s("0123456789");
    const S& cs = s;
    for (S::size_type i = 0; i < cs.size(); ++i)
    {
        TC_ASSERT_EXPR(s[i] == static_cast<char>('0' + i));
        TC_ASSERT_EXPR(cs[i] == s[i]);
    }
    TC_ASSERT_EXPR(cs[cs.size()] == '\0');
    const S s2 = S();
    TC_ASSERT_EXPR(s2[0] == '\0');
    }
#ifdef _LIBCPP_DEBUG
    {
        std::string s;
        char c = s[0];
        TC_ASSERT_EXPR(c == '\0');
        c = s[1];
        TC_ASSERT_EXPR(false);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
