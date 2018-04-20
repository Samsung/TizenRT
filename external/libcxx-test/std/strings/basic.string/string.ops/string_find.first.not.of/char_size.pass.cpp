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


template <class S>
void
test(const S& s, typename S::value_type c, typename S::size_type pos,
     typename S::size_type x)
{
    assert(s.find_first_not_of(c, pos) == x);
    if (x != S::npos)
        assert(pos <= x && x < s.size());
}

template <class S>
void
test(const S& s, typename S::value_type c, typename S::size_type x)
{
    assert(s.find_first_not_of(c) == x);
    if (x != S::npos)
        assert(x < s.size());
}

int main()
{
    {
    typedef std::string S;
    test(S(""), 'q', 0, S::npos);
    test(S(""), 'q', 1, S::npos);
    test(S("kitcj"), 'q', 0, 0);
    test(S("qkamf"), 'q', 1, 1);
    test(S("nhmko"), 'q', 2, 2);
    test(S("tpsaf"), 'q', 4, 4);
    test(S("lahfb"), 'q', 5, S::npos);
    test(S("irkhs"), 'q', 6, S::npos);
    test(S("gmfhdaipsr"), 'q', 0, 0);
    test(S("kantesmpgj"), 'q', 1, 1);
    test(S("odaftiegpm"), 'q', 5, 5);
    test(S("oknlrstdpi"), 'q', 9, 9);
    test(S("eolhfgpjqk"), 'q', 10, S::npos);
    test(S("pcdrofikas"), 'q', 11, S::npos);
    test(S("nbatdlmekrgcfqsophij"), 'q', 0, 0);
    test(S("bnrpehidofmqtcksjgla"), 'q', 1, 1);
    test(S("jdmciepkaqgotsrfnhlb"), 'q', 10, 10);
    test(S("jtdaefblsokrmhpgcnqi"), 'q', 19, 19);
    test(S("hkbgspofltajcnedqmri"), 'q', 20, S::npos);
    test(S("oselktgbcapndfjihrmq"), 'q', 21, S::npos);

    test(S(""), 'q', S::npos);
    test(S("q"), 'q', S::npos);
    test(S("qqq"), 'q', S::npos);
    test(S("csope"), 'q', 0);
    test(S("gfsmthlkon"), 'q', 0);
    test(S("laenfsbridchgotmkqpj"), 'q', 0);
    }
}
