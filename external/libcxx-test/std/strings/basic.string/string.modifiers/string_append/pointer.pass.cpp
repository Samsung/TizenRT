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

// basic_string<charT,traits,Allocator>& append(const charT* s);

#include <string>
#include <stdexcept>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, const typename S::value_type* str, S expected)
{
    s.append(str);
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == expected);
    return 0;
}

int tc_libcxx_strings_string_append_pointer(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(), "", S())));
    TC_ASSERT_FUNC((test(S(), "12345", S("12345"))));
    TC_ASSERT_FUNC((test(S(), "12345678901234567890", S("12345678901234567890"))));

    TC_ASSERT_FUNC((test(S("12345"), "", S("12345"))));
    TC_ASSERT_FUNC((test(S("12345"), "12345", S("1234512345"))));
    TC_ASSERT_FUNC((test(S("12345"), "1234567890", S("123451234567890"))));

    TC_ASSERT_FUNC((test(S("12345678901234567890"), "", S("12345678901234567890"))));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), "12345", S("1234567890123456789012345"))));
    test(S("12345678901234567890"), "12345678901234567890",
         S("1234567890123456789012345678901234567890"));
    }

    { // test appending to self
    typedef std::string S;
    S s_short = "123/";
    S s_long  = "Lorem ipsum dolor sit amet, consectetur/";

    s_short.append(s_short.c_str());
    TC_ASSERT_EXPR(s_short == "123/123/");
    s_short.append(s_short.c_str());
    TC_ASSERT_EXPR(s_short == "123/123/123/123/");
    s_short.append(s_short.c_str());
    TC_ASSERT_EXPR(s_short == "123/123/123/123/123/123/123/123/");

    s_long.append(s_long.c_str());
    TC_ASSERT_EXPR(s_long == "Lorem ipsum dolor sit amet, consectetur/Lorem ipsum dolor sit amet, consectetur/");
    }
    TC_SUCCESS_RESULT();
    return 0;
}
