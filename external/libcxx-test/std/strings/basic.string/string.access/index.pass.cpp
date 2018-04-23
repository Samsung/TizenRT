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

// const_reference operator[](size_type pos) const;
//       reference operator[](size_type pos);

#ifdef _LIBCPP_DEBUG
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"


int tc_libcxx_strings_string_access_index(void)
{
    {
    typedef std::string S;
    S s("0123456789");
    const S& cs = s;
    for (S::size_type i = 0; i < cs.size(); ++i)
    {
        TC_ASSERT_EXPR(s[i] == static_cast<char>('0' + i));
        TC_ASSERT_EXPR(cs[i] == s[i]);
    }
    TC_ASSERT_EXPR(cs[cs.size()] == '\0');
    const S s2 = S();
    TC_ASSERT_EXPR(s2[0] == '\0');
    }
#ifdef _LIBCPP_DEBUG
    {
        std::string s;
        char c = s[0];
        TC_ASSERT_EXPR(c == '\0');
        c = s[1];
        TC_ASSERT_EXPR(false);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
