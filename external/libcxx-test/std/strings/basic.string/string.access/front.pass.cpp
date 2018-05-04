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

// const charT& front() const;
//       charT& front();

#ifdef _LIBCPP_DEBUG
#define _LIBCPP_ASSERT(x, m) ((x) ? (void)0 : std::exit(0))
#endif

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"


template <class S>
static int
test(S s)
{
    const S& cs = s;
    TC_ASSERT_EXPR(&cs.front() == &cs[0]);
    TC_ASSERT_EXPR(&s.front() == &s[0]);
    s.front() = typename S::value_type('z');
    TC_ASSERT_EXPR(s.front() == typename S::value_type('z'));
    return 0;
}

int tc_libcxx_strings_string_access_front(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S("1"))));
    TC_ASSERT_FUNC((test(S("1234567890123456789012345678901234567890"))));
    }
#ifdef _LIBCPP_DEBUG
    {
        std::string s;
        char c = s.front();
        TC_ASSERT_EXPR(false);
    }
#endif
    TC_SUCCESS_RESULT();
    return 0;
}
