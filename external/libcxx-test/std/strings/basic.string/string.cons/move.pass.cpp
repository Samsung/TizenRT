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

// basic_string(basic_string<charT,traits,Allocator>&& str);

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"
#include "test_allocator.h"

template <class S>
static int
test(S s0)
{
    S s1 = s0;
    S s2 = std::move(s0);
    LIBCPP_ASSERT(s2.__invariants());
    LIBCPP_ASSERT(s0.__invariants());
    TC_ASSERT_EXPR(s2 == s1);
    TC_ASSERT_EXPR(s2.capacity() >= s2.size());
    TC_ASSERT_EXPR(s2.get_allocator() == s1.get_allocator());
    return 0;
}

int tc_libcxx_strings_string_cons_move(void)
{
    typedef test_allocator<char> A;
    typedef std::basic_string<char, std::char_traits<char>, A> S;
    TC_ASSERT_FUNC((test(S(A(3)))));
    TC_ASSERT_FUNC((test(S("1", A(5)))));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890123456789012345678901234567890", A(7)))));
    TC_SUCCESS_RESULT();
    return 0;
}
