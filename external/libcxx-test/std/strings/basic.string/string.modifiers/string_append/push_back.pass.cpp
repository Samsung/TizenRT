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

// static int push_back(charT c)

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

struct veryLarge
{
  long long a;
  char b;
};

template <class S>
static int
test(S s, typename S::value_type c, S expected)
{
    s.push_back(c);
    LIBCPP_ASSERT(s.__invariants());
    TC_ASSERT_EXPR(s == expected);
    return 0;
}

int tc_libcxx_strings_string_append_push_back(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(), 'a', S(1, 'a'))));
    TC_ASSERT_FUNC((test(S("12345"), 'a', S("12345a"))));
    TC_ASSERT_FUNC((test(S("12345678901234567890"), 'a', S("12345678901234567890a"))));
    }

    TC_SUCCESS_RESULT();
    return 0;
}
