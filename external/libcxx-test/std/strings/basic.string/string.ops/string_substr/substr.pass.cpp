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

// basic_string substr(size_type pos = 0, size_type n = npos) const;

#include <string>
#include <stdexcept>
#include <algorithm>
#include <cassert>
#include "libcxx_tc_common.h"

#include "test_macros.h"

template <class S>
static int
test(const S& s, typename S::size_type pos, typename S::size_type n)
{
    if (pos <= s.size())
    {
        S str = s.substr(pos, n);
        LIBCPP_ASSERT(str.__invariants());
        TC_ASSERT_EXPR(pos <= s.size());
        typename S::size_type rlen = std::min(n, s.size() - pos);
        TC_ASSERT_EXPR(str.size() == rlen);
        TC_ASSERT_EXPR(S::traits_type::compare(s.data()+pos, str.data(), rlen) == 0);
    }
#ifndef TEST_HAS_NO_EXCEPTIONS
    else
    {
        try
        {
            S str = s.substr(pos, n);
            TC_ASSERT_EXPR(false);
        }
        catch (std::out_of_range&)
        {
            TC_ASSERT_EXPR(pos > s.size());
        }
    }
#endif
    return 0;
}

int tc_libcxx_strings_string_substr_substr(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test(S(""), 0, 0)));
    TC_ASSERT_FUNC((test(S(""), 1, 0)));
    TC_ASSERT_FUNC((test(S("pniot"), 0, 0)));
    TC_ASSERT_FUNC((test(S("htaob"), 0, 1)));
    TC_ASSERT_FUNC((test(S("fodgq"), 0, 2)));
    TC_ASSERT_FUNC((test(S("hpqia"), 0, 4)));
    TC_ASSERT_FUNC((test(S("qanej"), 0, 5)));
    TC_ASSERT_FUNC((test(S("dfkap"), 1, 0)));
    TC_ASSERT_FUNC((test(S("clbao"), 1, 1)));
    TC_ASSERT_FUNC((test(S("ihqrf"), 1, 2)));
    TC_ASSERT_FUNC((test(S("mekdn"), 1, 3)));
    TC_ASSERT_FUNC((test(S("ngtjf"), 1, 4)));
    TC_ASSERT_FUNC((test(S("srdfq"), 2, 0)));
    TC_ASSERT_FUNC((test(S("qkdrs"), 2, 1)));
    TC_ASSERT_FUNC((test(S("ikcrq"), 2, 2)));
    TC_ASSERT_FUNC((test(S("cdaih"), 2, 3)));
    TC_ASSERT_FUNC((test(S("dmajb"), 4, 0)));
    TC_ASSERT_FUNC((test(S("karth"), 4, 1)));
    TC_ASSERT_FUNC((test(S("lhcdo"), 5, 0)));
    TC_ASSERT_FUNC((test(S("acbsj"), 6, 0)));
    TC_ASSERT_FUNC((test(S("pbsjikaole"), 0, 0)));
    TC_ASSERT_FUNC((test(S("pcbahntsje"), 0, 1)));
    TC_ASSERT_FUNC((test(S("mprdjbeiak"), 0, 5)));
    TC_ASSERT_FUNC((test(S("fhepcrntko"), 0, 9)));
    TC_ASSERT_FUNC((test(S("eqmpaidtls"), 0, 10)));
    TC_ASSERT_FUNC((test(S("joidhalcmq"), 1, 0)));
    TC_ASSERT_FUNC((test(S("omigsphflj"), 1, 1)));
    TC_ASSERT_FUNC((test(S("kocgbphfji"), 1, 4)));
    TC_ASSERT_FUNC((test(S("onmjekafbi"), 1, 8)));
    TC_ASSERT_FUNC((test(S("fbslrjiqkm"), 1, 9)));
    TC_ASSERT_FUNC((test(S("oqmrjahnkg"), 5, 0)));
    TC_ASSERT_FUNC((test(S("jeidpcmalh"), 5, 1)));
    TC_ASSERT_FUNC((test(S("schfalibje"), 5, 2)));
    TC_ASSERT_FUNC((test(S("crliponbqe"), 5, 4)));
    TC_ASSERT_FUNC((test(S("igdscopqtm"), 5, 5)));
    TC_ASSERT_FUNC((test(S("qngpdkimlc"), 9, 0)));
    TC_ASSERT_FUNC((test(S("thdjgafrlb"), 9, 1)));
    TC_ASSERT_FUNC((test(S("hcjitbfapl"), 10, 0)));
    TC_ASSERT_FUNC((test(S("mgojkldsqh"), 11, 0)));
    TC_ASSERT_FUNC((test(S("gfshlcmdjreqipbontak"), 0, 0)));
    TC_ASSERT_FUNC((test(S("nadkhpfemgclosibtjrq"), 0, 1)));
    TC_ASSERT_FUNC((test(S("nkodajteqplrbifhmcgs"), 0, 10)));
    TC_ASSERT_FUNC((test(S("ofdrqmkeblthacpgijsn"), 0, 19)));
    TC_ASSERT_FUNC((test(S("gbmetiprqdoasckjfhln"), 0, 20)));
    TC_ASSERT_FUNC((test(S("bdfjqgatlksriohemnpc"), 1, 0)));
    TC_ASSERT_FUNC((test(S("crnklpmegdqfiashtojb"), 1, 1)));
    TC_ASSERT_FUNC((test(S("ejqcnahdrkfsmptilgbo"), 1, 9)));
    TC_ASSERT_FUNC((test(S("jsbtafedocnirgpmkhql"), 1, 18)));
    TC_ASSERT_FUNC((test(S("prqgnlbaejsmkhdctoif"), 1, 19)));
    TC_ASSERT_FUNC((test(S("qnmodrtkebhpasifgcjl"), 10, 0)));
    TC_ASSERT_FUNC((test(S("pejafmnokrqhtisbcdgl"), 10, 1)));
    TC_ASSERT_FUNC((test(S("cpebqsfmnjdolhkratgi"), 10, 5)));
    TC_ASSERT_FUNC((test(S("odnqkgijrhabfmcestlp"), 10, 9)));
    TC_ASSERT_FUNC((test(S("lmofqdhpkibagnrcjste"), 10, 10)));
    TC_ASSERT_FUNC((test(S("lgjqketopbfahrmnsicd"), 19, 0)));
    TC_ASSERT_FUNC((test(S("ktsrmnqagdecfhijpobl"), 19, 1)));
    TC_ASSERT_FUNC((test(S("lsaijeqhtrbgcdmpfkno"), 20, 0)));
    TC_ASSERT_FUNC((test(S("dplqartnfgejichmoskb"), 21, 0)));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
