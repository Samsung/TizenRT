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

// size_type rfind(charT c, size_type pos = npos) const;

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"


template <class S>
static int
test(const S& s, typename S::value_type c, typename S::size_type pos,
     typename S::size_type x)
{
    TC_ASSERT_EXPR(s.rfind(c, pos) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(x <= pos && x + 1 <= s.size());
    return 0;
}

template <class S>
static int
test(const S& s, typename S::value_type c, typename S::size_type x)
{
    TC_ASSERT_EXPR(s.rfind(c) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(x + 1 <= s.size());
    return 0;
}

int tc_libcxx_strings_string_rfind_char_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""), 'b', 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), 'b', 1, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), 'b', 0, S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), 'b', 1, 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 'b', 2, 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 'b', 4, 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 'b', 5, 1)));
    TC_ASSERT_FUNC((test(S("abcde"), 'b', 6, 1)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), 'b', 0, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), 'b', 1, 1)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), 'b', 5, 1)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), 'b', 9, 6)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), 'b', 10, 6)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), 'b', 11, 6)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), 'b', 0, S::npos)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), 'b', 1, 1)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), 'b', 10, 6)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), 'b', 19, 16)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), 'b', 20, 16)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), 'b', 21, 16)));

    TC_ASSERT_FUNC((test(S(""), 'b', S::npos)));
    TC_ASSERT_FUNC((test(S("abcde"), 'b', 1)));
    TC_ASSERT_FUNC((test(S("abcdeabcde"), 'b', 6)));
    TC_ASSERT_FUNC((test(S("abcdeabcdeabcdeabcde"), 'b', 16)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
