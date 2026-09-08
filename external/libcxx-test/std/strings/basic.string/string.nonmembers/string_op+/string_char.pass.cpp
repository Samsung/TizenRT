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
//   basic_string<charT,traits,Allocator>
//   operator+(const basic_string<charT,traits,Allocator>& lhs, charT rhs);

// template<class charT, class traits, class Allocator>
//   basic_string<charT,traits,Allocator>&&
//   operator+(basic_string<charT,traits,Allocator>&& lhs, charT rhs);

#include <string>
#include <utility>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int test0(const S& lhs, typename S::value_type rhs, const S& x) {
  TC_ASSERT_EXPR(lhs + rhs == x);
    return 0;
}


int tc_libcxx_strings_string_op__string_char(void)
{
    typedef std::string S;
    TC_ASSERT_FUNC((test0(S(""), '1', S("1"))));
    TC_ASSERT_FUNC((test0(S("abcde"), '1', S("abcde1"))));
    TC_ASSERT_FUNC((test0(S("abcdefghij"), '1', S("abcdefghij1"))));
    TC_ASSERT_FUNC((test0(S("abcdefghijklmnopqrst"), '1', S("abcdefghijklmnopqrst1"))));
    TC_SUCCESS_RESULT();
    return 0;
}
