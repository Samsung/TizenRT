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

// size_type find_first_of(charT c, size_type pos = 0) const;

#include <string>
#include <cassert>
#include "test_macros.h"
#include "libcxx_tc_common.h"


template <class S>
static int
test(const S& s, typename S::value_type c, typename S::size_type pos,
     typename S::size_type x)
{
    TC_ASSERT_EXPR(s.find_first_of(c, pos) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(pos <= x && x < s.size());
    return 0;
}

template <class S>
static int
test(const S& s, typename S::value_type c, typename S::size_type x)
{
    TC_ASSERT_EXPR(s.find_first_of(c) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(x < s.size());
    return 0;
}

int tc_libcxx_strings_string_find_first_of_char_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""), 'e', 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), 'e', 1, S::npos)));
    TC_ASSERT_FUNC((test(S("kitcj"), 'e', 0, S::npos)));
    TC_ASSERT_FUNC((test(S("qkamf"), 'e', 1, S::npos)));
    TC_ASSERT_FUNC((test(S("nhmko"), 'e', 2, S::npos)));
    TC_ASSERT_FUNC((test(S("tpsaf"), 'e', 4, S::npos)));
    TC_ASSERT_FUNC((test(S("lahfb"), 'e', 5, S::npos)));
    TC_ASSERT_FUNC((test(S("irkhs"), 'e', 6, S::npos)));
    TC_ASSERT_FUNC((test(S("gmfhdaipsr"), 'e', 0, S::npos)));
    TC_ASSERT_FUNC((test(S("kantesmpgj"), 'e', 1, 4)));
    TC_ASSERT_FUNC((test(S("odaftiegpm"), 'e', 5, 6)));
    TC_ASSERT_FUNC((test(S("oknlrstdpi"), 'e', 9, S::npos)));
    TC_ASSERT_FUNC((test(S("eolhfgpjqk"), 'e', 10, S::npos)));
    TC_ASSERT_FUNC((test(S("pcdrofikas"), 'e', 11, S::npos)));
    TC_ASSERT_FUNC((test(S("nbatdlmekrgcfqsophij"), 'e', 0, 7)));
    TC_ASSERT_FUNC((test(S("bnrpehidofmqtcksjgla"), 'e', 1, 4)));
    TC_ASSERT_FUNC((test(S("jdmciepkaqgotsrfnhlb"), 'e', 10, S::npos)));
    TC_ASSERT_FUNC((test(S("jtdaefblsokrmhpgcnqi"), 'e', 19, S::npos)));
    TC_ASSERT_FUNC((test(S("hkbgspofltajcnedqmri"), 'e', 20, S::npos)));
    TC_ASSERT_FUNC((test(S("oselktgbcapndfjihrmq"), 'e', 21, S::npos)));

    TC_ASSERT_FUNC((test(S(""), 'e', S::npos)));
    TC_ASSERT_FUNC((test(S("csope"), 'e', 4)));
    TC_ASSERT_FUNC((test(S("gfsmthlkon"), 'e', S::npos)));
    TC_ASSERT_FUNC((test(S("laenfsbridchgotmkqpj"), 'e', 2)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
