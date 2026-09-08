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

// basic_string<charT,traits,Allocator>&
//   assign(size_type n, charT c);

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(S s, typename S::size_type n, typename S::value_type c, S expected)
{
    s.assign(n, c);
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == expected);
    return 0;
}

int tc_libcxx_strings_string_assign_size_char(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(), 0, 'a', S())));
    TC_ASSERT_FUNC((test(S(), 1, 'a', S(1, 'a'))));
    TC_ASSERT_FUNC((test(S(), 10, 'a', S(10, 'a'))));
    TC_ASSERT_FUNC((test(S(), 100, 'a', S(100, 'a'))));

    TC_ASSERT_FUNC((test(S("12345"), 0, 'a', S())));
    TC_ASSERT_FUNC((test(S("12345"), 1, 'a', S(1, 'a'))));
    TC_ASSERT_FUNC((test(S("12345"), 10, 'a', S(10, 'a'))));

    TC_ASSERT_FUNC((test(S("12345678901234567890"), 0, 'a', S())));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), 1, 'a', S(1, 'a'))));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), 10, 'a', S(10, 'a'))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
