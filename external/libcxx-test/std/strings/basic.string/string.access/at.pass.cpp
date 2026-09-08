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

// const_reference at(size_type pos) const;
//       reference at(size_type pos);

#include <string>
#include <stdexcept>
#include <cassert>
#include "libcxx_tc_common.h"


#include "test_macros.h"

template <class S>
static int
test(S s, typename S::size_type pos)
{
    const S& cs = s;
    if (pos < s.size())
    {
        TC_ASSERT_EXPR(s.at(pos) == s[pos]);
        TC_ASSERT_EXPR(cs.at(pos) == cs[pos]);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            TEST_IGNORE_NODISCARD s.at(pos);
            TC_ASSERT_EXPR(false);
        }
        catch (std::out_of_range&)
        {
            TC_ASSERT_EXPR(pos >= s.size());
        }
        try
        {
            TEST_IGNORE_NODISCARD cs.at(pos);
            TC_ASSERT_EXPR(false);
        }
        catch (std::out_of_range&)
        {
            TC_ASSERT_EXPR(pos >= s.size());
        }
    }
#endif
    return 0;
}

int tc_libcxx_strings_string_access_at(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(), 0)));
    TC_ASSERT_FUNC((test(S("123"), 0)));
    TC_ASSERT_FUNC((test(S("123"), 1)));
    TC_ASSERT_FUNC((test(S("123"), 2)));
    TC_ASSERT_FUNC((test(S("123"), 3)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
