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

// int compare(const charT *s) const;

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


static int sign(int x)
{
    if (x == 0)
        return 0;
    if (x < 0)
        return -1;
    return 1;
}

template <class S>
static int
test(const S& s, const typename S::value_type* str, int x)
{
    TC_ASSERT_EXPR(sign(s.compare(str)) == sign(x));
    return 0;
}

int tc_libcxx_strings_string_compare_pointer(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""), "", 0)));
    TC_ASSERT_FUNC((test(S(""), "abcde", -5)));
    TC_ASSERT_FUNC((test(S(""), "abcdefghij", -10)));
    TC_ASSERT_FUNC((test(S(""), "abcdefghijklmnopqrst", -20)));
    TC_ASSERT_FUNC((test(S("abcde"), "", 5)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcde", 0)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdefghij", -5)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdefghijklmnopqrst", -15)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), "", 10)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), "abcde", 5)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), "abcdefghij", 0)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), "abcdefghijklmnopqrst", -10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), "", 20)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), "abcde", 15)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), "abcdefghij", 10)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), "abcdefghijklmnopqrst", 0)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
