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

// bool empty() const noexcept;

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(const S& s)
{
    ASSERT_NOEXCEPT(s.empty());
    TC_ASSERT_EXPR(s.empty() == (s.size() == 0));
    return 0;
}

int tc_libcxx_strings_string_capacity_empty(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S())));
    TC_ASSERT_FUNC((test(S("123"))));
    TC_ASSERT_FUNC((test(S("12345678901234567890123456789012345678901234567890"))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
