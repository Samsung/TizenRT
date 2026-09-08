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

// size_type find(charT c, size_type pos = 0) const;

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


template <class S>
static int
test(const S& s, typename S::value_type c, typename S::size_type pos,
     typename S::size_type x)
{
    TC_ASSERT_EXPR(s.find(c, pos) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(pos <= x && x + 1 <= s.size());
    return 0;
}

template <class S>
static int
test(const S& s, typename S::value_type c, typename S::size_type x)
{
    TC_ASSERT_EXPR(s.find(c) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(0 <= x && x + 1 <= s.size());
    return 0;
}

int tc_libcxx_strings_string_find_char_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""), 'c', 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), 'c', 1, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), 'c', 0, 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 'c', 1, 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 'c', 2, 2)));
    TC_ASSERT_FUNC((test(S("abcde"), 'c', 4, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), 'c', 5, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), 'c', 6, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), 'c', 0, 2)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), 'c', 1, 2)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), 'c', 5, 7)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), 'c', 9, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), 'c', 10, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), 'c', 11, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), 'c', 0, 2)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), 'c', 1, 2)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), 'c', 10, 12)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), 'c', 19, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), 'c', 20, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), 'c', 21, S::npos)));

    TC_ASSERT_FUNC((test(S(""), 'c', S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), 'c', 2)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), 'c', 2)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), 'c', 2)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
