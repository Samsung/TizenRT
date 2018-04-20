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


template <class S>
void
test(const S& s, typename S::value_type c, typename S::size_type pos,
     typename S::size_type x)
{
    assert(s.find_last_of(c, pos) == x);
    if (x != S::npos)
        assert(x <= pos && x < s.size());
}

template <class S>
void
test(const S& s, typename S::value_type c, typename S::size_type x)
{
    assert(s.find_last_of(c) == x);
    if (x != S::npos)
        assert(x < s.size());
}

int main()
{
    {
    typedef std::string S;
    test(S(""), 'm', 0, S::npos);
    test(S(""), 'm', 1, S::npos);
    test(S("kitcj"), 'm', 0, S::npos);
    test(S("qkamf"), 'm', 1, S::npos);
    test(S("nhmko"), 'm', 2, 2);
    test(S("tpsaf"), 'm', 4, S::npos);
    test(S("lahfb"), 'm', 5, S::npos);
    test(S("irkhs"), 'm', 6, S::npos);
    test(S("gmfhdaipsr"), 'm', 0, S::npos);
    test(S("kantesmpgj"), 'm', 1, S::npos);
    test(S("odaftiegpm"), 'm', 5, S::npos);
    test(S("oknlrstdpi"), 'm', 9, S::npos);
    test(S("eolhfgpjqk"), 'm', 10, S::npos);
    test(S("pcdrofikas"), 'm', 11, S::npos);
    test(S("nbatdlmekrgcfqsophij"), 'm', 0, S::npos);
    test(S("bnrpehidofmqtcksjgla"), 'm', 1, S::npos);
    test(S("jdmciepkaqgotsrfnhlb"), 'm', 10, 2);
    test(S("jtdaefblsokrmhpgcnqi"), 'm', 19, 12);
    test(S("hkbgspofltajcnedqmri"), 'm', 20, 17);
    test(S("oselktgbcapndfjihrmq"), 'm', 21, 18);

    test(S(""), 'm', S::npos);
    test(S("csope"), 'm', S::npos);
    test(S("gfsmthlkon"), 'm', 3);
    test(S("laenfsbridchgotmkqpj"), 'm', 15);
    }
}
