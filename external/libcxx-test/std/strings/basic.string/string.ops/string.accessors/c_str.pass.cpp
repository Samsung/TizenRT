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

// const charT* c_str() const;

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"


template <class S>
static int
test(const S& s)
{
    typedef typename S::traits_type T;
    const typename S::value_type* str = s.c_str();
    if (s.size() > 0)
    {
        TC_ASSERT_EXPR(T::compare(str, &s[0], s.size()) == 0);
        TC_ASSERT_EXPR(T::eq(str[s.size()], typename S::value_type()));
    }
    else
        TC_ASSERT_EXPR(T::eq(str[0], typename S::value_type()));
    return 0;
}

int tc_libcxx_strings_string_accessors_c_str(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""))));
    TC_ASSERT_FUNC((test(S("abcde"))));
    TC_ASSERT_FUNC((test(S("abcdefghij"))));
    TC_ASSERT_FUNC((test(S("abcdefghijklmnopqrst"))));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
