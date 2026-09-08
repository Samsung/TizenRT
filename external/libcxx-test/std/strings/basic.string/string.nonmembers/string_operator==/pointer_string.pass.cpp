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

// template<class charT, class traits, class Allocator>
//   bool operator==(const charT* lhs, const basic_string<charT,traits,Allocator>& rhs);

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


template <class S>
static int
test(const typename S::value_type* lhs, const S& rhs, bool x)
{
    TC_ASSERT_EXPR((lhs == rhs) == x);
    return 0;
}

int tc_libcxx_strings_string_operator___pointer_string(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test("", S(""), true)));
    TC_ASSERT_FUNC((test("", S("abcde"), false)));
    TC_ASSERT_FUNC((test("", S("abcdefghij"), false)));
    TC_ASSERT_FUNC((test("", S("abcdefghijklmnopqrst"), false)));
    TC_ASSERT_FUNC((test("abcde", S(""), false)));
    TC_ASSERT_FUNC((test("abcde", S("abcde"), true)));
    TC_ASSERT_FUNC((test("abcde", S("abcdefghij"), false)));
    TC_ASSERT_FUNC((test("abcde", S("abcdefghijklmnopqrst"), false)));
    TC_ASSERT_FUNC((test("abcdefghij", S(""), false)));
    TC_ASSERT_FUNC((test("abcdefghij", S("abcde"), false)));
    TC_ASSERT_FUNC((test("abcdefghij", S("abcdefghij"), true)));
    TC_ASSERT_FUNC((test("abcdefghij", S("abcdefghijklmnopqrst"), false)));
    TC_ASSERT_FUNC((test("abcdefghijklmnopqrst", S(""), false)));
    TC_ASSERT_FUNC((test("abcdefghijklmnopqrst", S("abcde"), false)));
    TC_ASSERT_FUNC((test("abcdefghijklmnopqrst", S("abcdefghij"), false)));
    TC_ASSERT_FUNC((test("abcdefghijklmnopqrst", S("abcdefghijklmnopqrst"), true)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
