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

// size_type find_last_of(charT c, size_type pos = npos) const;

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"


template <class S>
static int
test(const S& s, typename S::value_type c, typename S::size_type pos,
     typename S::size_type x)
{
    TC_ASSERT_EXPR(s.find_last_of(c, pos) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(x <= pos && x < s.size());
    return 0;
}

template <class S>
static int
test(const S& s, typename S::value_type c, typename S::size_type x)
{
    TC_ASSERT_EXPR(s.find_last_of(c) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(x < s.size());
    return 0;
}

int tc_libcxx_strings_string_find_last_of_char_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""), 'm', 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), 'm', 1, S::npos)));
    TC_ASSERT_FUNC((test(S("kitcj"), 'm', 0, S::npos)));
    TC_ASSERT_FUNC((test(S("qkamf"), 'm', 1, S::npos)));
    TC_ASSERT_FUNC((test(S("nhmko"), 'm', 2, 2)));
    TC_ASSERT_FUNC((test(S("tpsaf"), 'm', 4, S::npos)));
    TC_ASSERT_FUNC((test(S("lahfb"), 'm', 5, S::npos)));
    TC_ASSERT_FUNC((test(S("irkhs"), 'm', 6, S::npos)));
    TC_ASSERT_FUNC((test(S("gmfhdaipsr"), 'm', 0, S::npos)));
    TC_ASSERT_FUNC((test(S("kantesmpgj"), 'm', 1, S::npos)));
    TC_ASSERT_FUNC((test(S("odaftiegpm"), 'm', 5, S::npos)));
    TC_ASSERT_FUNC((test(S("oknlrstdpi"), 'm', 9, S::npos)));
    TC_ASSERT_FUNC((test(S("eolhfgpjqk"), 'm', 10, S::npos)));
    TC_ASSERT_FUNC((test(S("pcdrofikas"), 'm', 11, S::npos)));
    TC_ASSERT_FUNC((test(S("nbatdlmekrgcfqsophij"), 'm', 0, S::npos)));
    TC_ASSERT_FUNC((test(S("bnrpehidofmqtcksjgla"), 'm', 1, S::npos)));
    TC_ASSERT_FUNC((test(S("jdmciepkaqgotsrfnhlb"), 'm', 10, 2)));
    TC_ASSERT_FUNC((test(S("jtdaefblsokrmhpgcnqi"), 'm', 19, 12)));
    TC_ASSERT_FUNC((test(S("hkbgspofltajcnedqmri"), 'm', 20, 17)));
    TC_ASSERT_FUNC((test(S("oselktgbcapndfjihrmq"), 'm', 21, 18)));

    TC_ASSERT_FUNC((test(S(""), 'm', S::npos)));
    TC_ASSERT_FUNC((test(S("csope"), 'm', S::npos)));
    TC_ASSERT_FUNC((test(S("gfsmthlkon"), 'm', 3)));
    TC_ASSERT_FUNC((test(S("laenfsbridchgotmkqpj"), 'm', 15)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
