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

// static int pop_back();

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, S expected)
{
    s.pop_back();
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s[s.size()] == typename S::value_type());
    TC_ASSERT_EXPR(s == expected);
    return 0;
}

int tc_libcxx_strings_string_erase_pop_back(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S("abcde"), S("abcd"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"), S("abcdefghi"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), S("abcdefghijklmnopqrs"))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
