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


template <class S>
void
test(const S& s, typename S::value_type c, typename S::size_type pos,
     typename S::size_type x)
{
    assert(s.find_last_not_of(c, pos) == x);
    if (x != S::npos)
        assert(x <= pos && x < s.size());
}

template <class S>
void
test(const S& s, typename S::value_type c, typename S::size_type x)
{
    assert(s.find_last_not_of(c) == x);
    if (x != S::npos)
        assert(x < s.size());
}

int main()
{
    {
    typedef std::string S;
    test(S(""), 'i', 0, S::npos);
    test(S(""), 'i', 1, S::npos);
    test(S("kitcj"), 'i', 0, 0);
    test(S("qkamf"), 'i', 1, 1);
    test(S("nhmko"), 'i', 2, 2);
    test(S("tpsaf"), 'i', 4, 4);
    test(S("lahfb"), 'i', 5, 4);
    test(S("irkhs"), 'i', 6, 4);
    test(S("gmfhdaipsr"), 'i', 0, 0);
    test(S("kantesmpgj"), 'i', 1, 1);
    test(S("odaftiegpm"), 'i', 5, 4);
    test(S("oknlrstdpi"), 'i', 9, 8);
    test(S("eolhfgpjqk"), 'i', 10, 9);
    test(S("pcdrofikas"), 'i', 11, 9);
    test(S("nbatdlmekrgcfqsophij"), 'i', 0, 0);
    test(S("bnrpehidofmqtcksjgla"), 'i', 1, 1);
    test(S("jdmciepkaqgotsrfnhlb"), 'i', 10, 10);
    test(S("jtdaefblsokrmhpgcnqi"), 'i', 19, 18);
    test(S("hkbgspofltajcnedqmri"), 'i', 20, 18);
    test(S("oselktgbcapndfjihrmq"), 'i', 21, 19);

    test(S(""), 'i', S::npos);
    test(S("csope"), 'i', 4);
    test(S("gfsmthlkon"), 'i', 9);
    test(S("laenfsbridchgotmkqpj"), 'i', 19);
    }
}
