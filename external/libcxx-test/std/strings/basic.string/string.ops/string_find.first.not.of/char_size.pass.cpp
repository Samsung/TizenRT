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

// size_type find_first_not_of(charT c, size_type pos = 0) const;

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"


template <class S>
static int
test(const S& s, typename S::value_type c, typename S::size_type pos,
     typename S::size_type x)
{
    TC_ASSERT_EXPR(s.find_first_not_of(c, pos) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(pos <= x && x < s.size());
    return 0;
}

template <class S>
static int
test(const S& s, typename S::value_type c, typename S::size_type x)
{
    TC_ASSERT_EXPR(s.find_first_not_of(c) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(x < s.size());
    return 0;
}

int tc_libcxx_strings_string_find_first_not_of_char_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""), 'q', 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), 'q', 1, S::npos)));
    TC_ASSERT_FUNC((test(S("kitcj"), 'q', 0, 0)));
    TC_ASSERT_FUNC((test(S("qkamf"), 'q', 1, 1)));
    TC_ASSERT_FUNC((test(S("nhmko"), 'q', 2, 2)));
    TC_ASSERT_FUNC((test(S("tpsaf"), 'q', 4, 4)));
    TC_ASSERT_FUNC((test(S("lahfb"), 'q', 5, S::npos)));
    TC_ASSERT_FUNC((test(S("irkhs"), 'q', 6, S::npos)));
    TC_ASSERT_FUNC((test(S("gmfhdaipsr"), 'q', 0, 0)));
    TC_ASSERT_FUNC((test(S("kantesmpgj"), 'q', 1, 1)));
    TC_ASSERT_FUNC((test(S("odaftiegpm"), 'q', 5, 5)));
    TC_ASSERT_FUNC((test(S("oknlrstdpi"), 'q', 9, 9)));
    TC_ASSERT_FUNC((test(S("eolhfgpjqk"), 'q', 10, S::npos)));
    TC_ASSERT_FUNC((test(S("pcdrofikas"), 'q', 11, S::npos)));
    TC_ASSERT_FUNC((test(S("nbatdlmekrgcfqsophij"), 'q', 0, 0)));
    TC_ASSERT_FUNC((test(S("bnrpehidofmqtcksjgla"), 'q', 1, 1)));
    TC_ASSERT_FUNC((test(S("jdmciepkaqgotsrfnhlb"), 'q', 10, 10)));
    TC_ASSERT_FUNC((test(S("jtdaefblsokrmhpgcnqi"), 'q', 19, 19)));
    TC_ASSERT_FUNC((test(S("hkbgspofltajcnedqmri"), 'q', 20, S::npos)));
    TC_ASSERT_FUNC((test(S("oselktgbcapndfjihrmq"), 'q', 21, S::npos)));

    TC_ASSERT_FUNC((test(S(""), 'q', S::npos)));
    TC_ASSERT_FUNC((test(S("q"), 'q', S::npos)));
    TC_ASSERT_FUNC((test(S("qqq"), 'q', S::npos)));
    TC_ASSERT_FUNC((test(S("csope"), 'q', 0)));
    TC_ASSERT_FUNC((test(S("gfsmthlkon"), 'q', 0)));
    TC_ASSERT_FUNC((test(S("laenfsbridchgotmkqpj"), 'q', 0)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
