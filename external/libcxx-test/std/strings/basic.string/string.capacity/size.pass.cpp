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

// size_type size() const;

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


template <class S>
static int
test(const S& s, typename S::size_type c)
{
    TC_ASSERT_EXPR(s.size() == c);
    return 0;
}

int tc_libcxx_strings_string_capacity_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(), 0)));
    TC_ASSERT_FUNC((test(S("123"), 3)));
    TC_ASSERT_FUNC((test(S("12345678901234567890123456789012345678901234567890"), 50)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
