/****************************************************************************
 *
 * Copyright 2018 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/
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
