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

// const charT& back() const;
//       charT& back();

#ifdef _LIBCPP_DEBUG
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


template <class S>
static int
test(S s)
{
    const S& cs = s;
    TC_ASSERT_EXPR(&cs.back() == &cs[cs.size()-1]);
    TC_ASSERT_EXPR(&s.back() == &s[cs.size()-1]);
    s.back() = typename S::value_type('z');
    TC_ASSERT_EXPR(s.back() == typename S::value_type('z'));
    return 0;
}

int tc_libcxx_strings_string_access_back(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S("1"))));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890"))));
    }
#ifdef _LIBCPP_DEBUG
    {
        std::string s;
        char c = s.back();
        TC_ASSERT_EXPR(false);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
