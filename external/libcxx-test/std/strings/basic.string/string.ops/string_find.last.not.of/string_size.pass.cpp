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

// size_type find_last_not_of(const basic_string& str, size_type pos = npos) const;

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"


template <class S>
static int
test(const S& s, const S& str, typename S::size_type pos, typename S::size_type x)
{
    TC_ASSERT_EXPR(s.find_last_not_of(str, pos) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(x <= pos && x < s.size());
    return 0;
}

template <class S>
static int
test(const S& s, const S& str, typename S::size_type x)
{
    TC_ASSERT_EXPR(s.find_last_not_of(str) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(x < s.size());
    return 0;
}

template <class S>
static int test0()
{
    TC_ASSERT_FUNC((test(S(""), S(""), 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), S("laenf"), 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), S("pqlnkmbdjo"), 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), S("qkamfogpnljdcshbreti"), 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), S(""), 1, S::npos)));
    TC_ASSERT_FUNC((test(S(""), S("bjaht"), 1, S::npos)));
    TC_ASSERT_FUNC((test(S(""), S("hjlcmgpket"), 1, S::npos)));
    TC_ASSERT_FUNC((test(S(""), S("htaobedqikfplcgjsmrn"), 1, S::npos)));
    TC_ASSERT_FUNC((test(S("fodgq"), S(""), 0, 0)));
    TC_ASSERT_FUNC((test(S("qanej"), S("dfkap"), 0, 0)));
    TC_ASSERT_FUNC((test(S("clbao"), S("ihqrfebgad"), 0, 0)));
    TC_ASSERT_FUNC((test(S("mekdn"), S("ngtjfcalbseiqrphmkdo"), 0, S::npos)));
    TC_ASSERT_FUNC((test(S("srdfq"), S(""), 1, 1)));
    TC_ASSERT_FUNC((test(S("oemth"), S("ikcrq"), 1, 1)));
    TC_ASSERT_FUNC((test(S("cdaih"), S("dmajblfhsg"), 1, 0)));
    TC_ASSERT_FUNC((test(S("qohtk"), S("oqftjhdmkgsblacenirp"), 1, S::npos)));
    TC_ASSERT_FUNC((test(S("cshmd"), S(""), 2, 2)));
    TC_ASSERT_FUNC((test(S("lhcdo"), S("oebqi"), 2, 2)));
    TC_ASSERT_FUNC((test(S("qnsoh"), S("kojhpmbsfe"), 2, 1)));
    TC_ASSERT_FUNC((test(S("pkrof"), S("acbsjqogpltdkhinfrem"), 2, S::npos)));
    TC_ASSERT_FUNC((test(S("fmtsp"), S(""), 4, 4)));
    TC_ASSERT_FUNC((test(S("khbpm"), S("aobjd"), 4, 4)));
    TC_ASSERT_FUNC((test(S("pbsji"), S("pcbahntsje"), 4, 4)));
    TC_ASSERT_FUNC((test(S("mprdj"), S("fhepcrntkoagbmldqijs"), 4, S::npos)));
    TC_ASSERT_FUNC((test(S("eqmpa"), S(""), 5, 4)));
    TC_ASSERT_FUNC((test(S("omigs"), S("kocgb"), 5, 4)));
    TC_ASSERT_FUNC((test(S("onmje"), S("fbslrjiqkm"), 5, 4)));
    TC_ASSERT_FUNC((test(S("oqmrj"), S("jeidpcmalhfnqbgtrsko"), 5, S::npos)));
    TC_ASSERT_FUNC((test(S("schfa"), S(""), 6, 4)));
    TC_ASSERT_FUNC((test(S("igdsc"), S("qngpd"), 6, 4)));
    TC_ASSERT_FUNC((test(S("brqgo"), S("rodhqklgmb"), 6, S::npos)));
    TC_ASSERT_FUNC((test(S("tnrph"), S("thdjgafrlbkoiqcspmne"), 6, S::npos)));
    TC_ASSERT_FUNC((test(S("hcjitbfapl"), S(""), 0, 0)));
    TC_ASSERT_FUNC((test(S("daiprenocl"), S("ashjd"), 0, S::npos)));
    TC_ASSERT_FUNC((test(S("litpcfdghe"), S("mgojkldsqh"), 0, S::npos)));
    TC_ASSERT_FUNC((test(S("aidjksrolc"), S("imqnaghkfrdtlopbjesc"), 0, S::npos)));
    TC_ASSERT_FUNC((test(S("qpghtfbaji"), S(""), 1, 1)));
    TC_ASSERT_FUNC((test(S("gfshlcmdjr"), S("nadkh"), 1, 1)));
    TC_ASSERT_FUNC((test(S("nkodajteqp"), S("ofdrqmkebl"), 1, 0)));
    TC_ASSERT_FUNC((test(S("gbmetiprqd"), S("bdfjqgatlksriohemnpc"), 1, S::npos)));
    TC_ASSERT_FUNC((test(S("crnklpmegd"), S(""), 5, 5)));
    TC_ASSERT_FUNC((test(S("jsbtafedoc"), S("prqgn"), 5, 5)));
    TC_ASSERT_FUNC((test(S("qnmodrtkeb"), S("pejafmnokr"), 5, 4)));
    TC_ASSERT_FUNC((test(S("cpebqsfmnj"), S("odnqkgijrhabfmcestlp"), 5, S::npos)));
    TC_ASSERT_FUNC((test(S("lmofqdhpki"), S(""), 9, 9)));
    TC_ASSERT_FUNC((test(S("hnefkqimca"), S("rtjpa"), 9, 8)));
    TC_ASSERT_FUNC((test(S("drtasbgmfp"), S("ktsrmnqagd"), 9, 9)));
    TC_ASSERT_FUNC((test(S("lsaijeqhtr"), S("rtdhgcisbnmoaqkfpjle"), 9, S::npos)));
    TC_ASSERT_FUNC((test(S("elgofjmbrq"), S(""), 10, 9)));
    TC_ASSERT_FUNC((test(S("mjqdgalkpc"), S("dplqa"), 10, 9)));
    TC_ASSERT_FUNC((test(S("kthqnfcerm"), S("dkacjoptns"), 10, 9)));
    TC_ASSERT_FUNC((test(S("dfsjhanorc"), S("hqfimtrgnbekpdcsjalo"), 10, S::npos)));
    TC_ASSERT_FUNC((test(S("eqsgalomhb"), S(""), 11, 9)));
    TC_ASSERT_FUNC((test(S("akiteljmoh"), S("lofbc"), 11, 9)));
    TC_ASSERT_FUNC((test(S("hlbdfreqjo"), S("astoegbfpn"), 11, 8)));
    TC_ASSERT_FUNC((test(S("taqobhlerg"), S("pdgreqomsncafklhtibj"), 11, S::npos)));
    TC_ASSERT_FUNC((test(S("snafbdlghrjkpqtoceim"), S(""), 0, 0)));
    TC_ASSERT_FUNC((test(S("aemtbrgcklhndjisfpoq"), S("lbtqd"), 0, 0)));
    TC_ASSERT_FUNC((test(S("pnracgfkjdiholtbqsem"), S("tboimldpjh"), 0, S::npos)));
    TC_ASSERT_FUNC((test(S("dicfltehbsgrmojnpkaq"), S("slcerthdaiqjfnobgkpm"), 0, S::npos)));
    TC_ASSERT_FUNC((test(S("jlnkraeodhcspfgbqitm"), S(""), 1, 1)));
    TC_ASSERT_FUNC((test(S("lhosrngtmfjikbqpcade"), S("aqibs"), 1, 1)));
    TC_ASSERT_FUNC((test(S("rbtaqjhgkneisldpmfoc"), S("gtfblmqinc"), 1, 0)));
    TC_ASSERT_FUNC((test(S("gpifsqlrdkbonjtmheca"), S("mkqpbtdalgniorhfescj"), 1, S::npos)));
    TC_ASSERT_FUNC((test(S("hdpkobnsalmcfijregtq"), S(""), 10, 10)));
    TC_ASSERT_FUNC((test(S("jtlshdgqaiprkbcoenfm"), S("pblas"), 10, 9)));
    TC_ASSERT_FUNC((test(S("fkdrbqltsgmcoiphneaj"), S("arosdhcfme"), 10, 9)));
    TC_ASSERT_FUNC((test(S("crsplifgtqedjohnabmk"), S("blkhjeogicatqfnpdmsr"), 10, S::npos)));
    TC_ASSERT_FUNC((test(S("niptglfbosehkamrdqcj"), S(""), 19, 19)));
    TC_ASSERT_FUNC((test(S("copqdhstbingamjfkler"), S("djkqc"), 19, 19)));
    TC_ASSERT_FUNC((test(S("mrtaefilpdsgocnhqbjk"), S("lgokshjtpb"), 19, 16)));
    TC_ASSERT_FUNC((test(S("kojatdhlcmigpbfrqnes"), S("bqjhtkfepimcnsgrlado"), 19, S::npos)));
    TC_ASSERT_FUNC((test(S("eaintpchlqsbdgrkjofm"), S(""), 20, 19)));
    TC_ASSERT_FUNC((test(S("gjnhidfsepkrtaqbmclo"), S("nocfa"), 20, 18)));
    TC_ASSERT_FUNC((test(S("spocfaktqdbiejlhngmr"), S("bgtajmiedc"), 20, 19)));
    TC_ASSERT_FUNC((test(S("rphmlekgfscndtaobiqj"), S("lsckfnqgdahejiopbtmr"), 20, S::npos)));
    TC_ASSERT_FUNC((test(S("liatsqdoegkmfcnbhrpj"), S(""), 21, 19)));
    TC_ASSERT_FUNC((test(S("binjagtfldkrspcomqeh"), S("gfsrt"), 21, 19)));
    TC_ASSERT_FUNC((test(S("latkmisecnorjbfhqpdg"), S("pfsocbhjtm"), 21, 19)));
    TC_ASSERT_FUNC((test(S("lecfratdjkhnsmqpoigb"), S("tpflmdnoicjgkberhqsa"), 21, S::npos)));
    return 0;
}

template <class S>
static int test1()
{
    TC_ASSERT_FUNC((test(S(""), S(""), S::npos)));
    TC_ASSERT_FUNC((test(S(""), S("laenf"), S::npos)));
    TC_ASSERT_FUNC((test(S(""), S("pqlnkmbdjo"), S::npos)));
    TC_ASSERT_FUNC((test(S(""), S("qkamfogpnljdcshbreti"), S::npos)));
    TC_ASSERT_FUNC((test(S("nhmko"), S(""), 4)));
    TC_ASSERT_FUNC((test(S("lahfb"), S("irkhs"), 4)));
    TC_ASSERT_FUNC((test(S("gmfhd"), S("kantesmpgj"), 4)));
    TC_ASSERT_FUNC((test(S("odaft"), S("oknlrstdpiqmjbaghcfe"), S::npos)));
    TC_ASSERT_FUNC((test(S("eolhfgpjqk"), S(""), 9)));
    TC_ASSERT_FUNC((test(S("nbatdlmekr"), S("bnrpe"), 8)));
    TC_ASSERT_FUNC((test(S("jdmciepkaq"), S("jtdaefblso"), 9)));
    TC_ASSERT_FUNC((test(S("hkbgspoflt"), S("oselktgbcapndfjihrmq"), S::npos)));
    TC_ASSERT_FUNC((test(S("gprdcokbnjhlsfmtieqa"), S(""), 19)));
    TC_ASSERT_FUNC((test(S("qjghlnftcaismkropdeb"), S("bjaht"), 18)));
    TC_ASSERT_FUNC((test(S("pnalfrdtkqcmojiesbhg"), S("hjlcmgpket"), 17)));
    TC_ASSERT_FUNC((test(S("pniotcfrhqsmgdkjbael"), S("htaobedqikfplcgjsmrn"), S::npos)));
    return 0;
}

int tc_libcxx_strings_string_find_last_not_of_string_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test0<S>()));
    TC_ASSERT_FUNC((test1<S>()));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
