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
//   bool operator>=(const basic_string<charT,traits,Allocator>& lhs, const charT* rhs);

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


template <class S>
static int
test(const S& lhs, const typename S::value_type* rhs, bool x)
{
    TC_ASSERT_EXPR((lhs >= rhs) == x);
    return 0;
}

int tc_libcxx_strings_string_opgt__string_pointer(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""), "", true)));
    TC_ASSERT_FUNC((test(S(""), "abcde", false)));
    TC_ASSERT_FUNC((test(S(""), "abcdefghij", false)));
    TC_ASSERT_FUNC((test(S(""), "abcdefghijklmnopqrst", false)));
    TC_ASSERT_FUNC((test(S("abcde"), "", true)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcde", true)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdefghij", false)));
    TC_ASSERT_FUNC((test(S("abcde"), "abcdefghijklmnopqrst", false)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), "", true)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), "abcde", true)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), "abcdefghij", true)));
    TC_ASSERT_FUNC((test(S("abcdefghij"), "abcdefghijklmnopqrst", false)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), "", true)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), "abcde", true)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), "abcdefghij", true)));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"), "abcdefghijklmnopqrst", true)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
