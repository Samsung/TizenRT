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

// UNSUPPORTED: c++98, c++03

// <string>

// basic_string<charT,traits,Allocator>&
//   operator=(basic_string<charT,traits,Allocator>&& str);

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_allocator.h"

template <class S>
static int
test(S s1, S s2)
{
    S s0 = s2;
    s1 = std::move(s2);
    LIBCPP_ASSERT(s1.__invariants());
    LIBCPP_ASSERT(s2.__invariants());
    TC_ASSERT_EXPR(s1 == s0);
    TC_ASSERT_EXPR(s1.capacity() >= s1.size());
    return 0;
}

int tc_libcxx_strings_string_cons_move_assignment(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(), S())));
    TC_ASSERT_FUNC((test(S("1"), S())));
    TC_ASSERT_FUNC((test(S(), S("1"))));
    TC_ASSERT_FUNC((test(S("1"), S("2"))));
    TC_ASSERT_FUNC((test(S("1"), S("2"))));

    test(S(),
         S("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"));
    test(S("123456789"),
         S("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"));
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890"),
         S("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"));
    test(S("1234567890123456789012345678901234567890123456789012345678901234567890"
           "1234567890123456789012345678901234567890123456789012345678901234567890"),
         S("abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
