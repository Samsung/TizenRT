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

// iterator erase(const_iterator p);

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, typename S::difference_type pos, S expected)
{
    typename S::const_iterator p = s.begin() + pos;
    typename S::iterator i = s.erase(p);
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s[s.size()] == typename S::value_type());
    TC_ASSERT_EXPR(s == expected);
    TC_ASSERT_EXPR(i - s.begin() == pos);
    return 0;
}

int tc_libcxx_strings_string_erase_iter(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S("abcde"), 0, S("bcde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 1, S("acde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 2, S("abde"))));
    TC_ASSERT_FUNC((test(S("abcde"), 4, S("abcd"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 0, S("bcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 1, S("acdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 5, S("abcdeghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), 9, S("abcdefghi"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 0, S("bcdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 1, S("acdefghijklmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 10, S("abcdefghijlmnopqrst"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), 19, S("abcdefghijklmnopqrs"))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
