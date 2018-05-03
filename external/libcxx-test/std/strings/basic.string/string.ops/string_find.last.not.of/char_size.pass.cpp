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

// size_type find_last_not_of(charT c, size_type pos = npos) const;

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"


template <class S>
static int
test(const S& s, typename S::value_type c, typename S::size_type pos,
     typename S::size_type x)
{
    TC_ASSERT_EXPR(s.find_last_not_of(c, pos) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(x <= pos && x < s.size());
    return 0;
}

template <class S>
static int
test(const S& s, typename S::value_type c, typename S::size_type x)
{
    TC_ASSERT_EXPR(s.find_last_not_of(c) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(x < s.size());
    return 0;
}

int tc_libcxx_strings_string_find_last_not_of_char_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""), 'i', 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), 'i', 1, S::npos)));
    TC_ASSERT_FUNC((test(S("kitcj"), 'i', 0, 0)));
    TC_ASSERT_FUNC((test(S("qkamf"), 'i', 1, 1)));
    TC_ASSERT_FUNC((test(S("nhmko"), 'i', 2, 2)));
    TC_ASSERT_FUNC((test(S("tpsaf"), 'i', 4, 4)));
    TC_ASSERT_FUNC((test(S("lahfb"), 'i', 5, 4)));
    TC_ASSERT_FUNC((test(S("irkhs"), 'i', 6, 4)));
    TC_ASSERT_FUNC((test(S("gmfhdaipsr"), 'i', 0, 0)));
    TC_ASSERT_FUNC((test(S("kantesmpgj"), 'i', 1, 1)));
    TC_ASSERT_FUNC((test(S("odaftiegpm"), 'i', 5, 4)));
    TC_ASSERT_FUNC((test(S("oknlrstdpi"), 'i', 9, 8)));
    TC_ASSERT_FUNC((test(S("eolhfgpjqk"), 'i', 10, 9)));
    TC_ASSERT_FUNC((test(S("pcdrofikas"), 'i', 11, 9)));
    TC_ASSERT_FUNC((test(S("nbatdlmekrgcfqsophij"), 'i', 0, 0)));
    TC_ASSERT_FUNC((test(S("bnrpehidofmqtcksjgla"), 'i', 1, 1)));
    TC_ASSERT_FUNC((test(S("jdmciepkaqgotsrfnhlb"), 'i', 10, 10)));
    TC_ASSERT_FUNC((test(S("jtdaefblsokrmhpgcnqi"), 'i', 19, 18)));
    TC_ASSERT_FUNC((test(S("hkbgspofltajcnedqmri"), 'i', 20, 18)));
    TC_ASSERT_FUNC((test(S("oselktgbcapndfjihrmq"), 'i', 21, 19)));

    TC_ASSERT_FUNC((test(S(""), 'i', S::npos)));
    TC_ASSERT_FUNC((test(S("csope"), 'i', 4)));
    TC_ASSERT_FUNC((test(S("gfsmthlkon"), 'i', 9)));
    TC_ASSERT_FUNC((test(S("laenfsbridchgotmkqpj"), 'i', 19)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
