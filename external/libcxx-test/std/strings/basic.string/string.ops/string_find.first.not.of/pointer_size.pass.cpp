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

// size_type find_first_not_of(const charT* s, size_type pos = 0) const;

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"


template <class S>
static int
test(const S& s, const typename S::value_type* str, typename S::size_type pos,
     typename S::size_type x)
{
    TC_ASSERT_EXPR(s.find_first_not_of(str, pos) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(pos <= x && x < s.size());
    return 0;
}

template <class S>
static int
test(const S& s, const typename S::value_type* str, typename S::size_type x)
{
    TC_ASSERT_EXPR(s.find_first_not_of(str) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(x < s.size());
    return 0;
}

template <class S>
static int test0()
{
    TC_ASSERT_FUNC((test(S(""), "", 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "laenf", 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "pqlnkmbdjo", 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "qkamfogpnljdcshbreti", 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "", 1, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "bjaht", 1, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "hjlcmgpket", 1, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "htaobedqikfplcgjsmrn", 1, S::npos)));
    TC_ASSERT_FUNC((test(S("fodgq"), "", 0, 0)));
    TC_ASSERT_FUNC((test(S("qanej"), "dfkap", 0, 0)));
    TC_ASSERT_FUNC((test(S("clbao"), "ihqrfebgad", 0, 0)));
    TC_ASSERT_FUNC((test(S("mekdn"), "ngtjfcalbseiqrphmkdo", 0, S::npos)));
    TC_ASSERT_FUNC((test(S("srdfq"), "", 1, 1)));
    TC_ASSERT_FUNC((test(S("oemth"), "ikcrq", 1, 1)));
    TC_ASSERT_FUNC((test(S("cdaih"), "dmajblfhsg", 1, 3)));
    TC_ASSERT_FUNC((test(S("qohtk"), "oqftjhdmkgsblacenirp", 1, S::npos)));
    TC_ASSERT_FUNC((test(S("cshmd"), "", 2, 2)));
    TC_ASSERT_FUNC((test(S("lhcdo"), "oebqi", 2, 2)));
    TC_ASSERT_FUNC((test(S("qnsoh"), "kojhpmbsfe", 2, S::npos)));
    TC_ASSERT_FUNC((test(S("pkrof"), "acbsjqogpltdkhinfrem", 2, S::npos)));
    TC_ASSERT_FUNC((test(S("fmtsp"), "", 4, 4)));
    TC_ASSERT_FUNC((test(S("khbpm"), "aobjd", 4, 4)));
    TC_ASSERT_FUNC((test(S("pbsji"), "pcbahntsje", 4, 4)));
    TC_ASSERT_FUNC((test(S("mprdj"), "fhepcrntkoagbmldqijs", 4, S::npos)));
    TC_ASSERT_FUNC((test(S("eqmpa"), "", 5, S::npos)));
    TC_ASSERT_FUNC((test(S("omigs"), "kocgb", 5, S::npos)));
    TC_ASSERT_FUNC((test(S("onmje"), "fbslrjiqkm", 5, S::npos)));
    TC_ASSERT_FUNC((test(S("oqmrj"), "jeidpcmalhfnqbgtrsko", 5, S::npos)));
    TC_ASSERT_FUNC((test(S("schfa"), "", 6, S::npos)));
    TC_ASSERT_FUNC((test(S("igdsc"), "qngpd", 6, S::npos)));
    TC_ASSERT_FUNC((test(S("brqgo"), "rodhqklgmb", 6, S::npos)));
    TC_ASSERT_FUNC((test(S("tnrph"), "thdjgafrlbkoiqcspmne", 6, S::npos)));
    TC_ASSERT_FUNC((test(S("hcjitbfapl"), "", 0, 0)));
    TC_ASSERT_FUNC((test(S("daiprenocl"), "ashjd", 0, 2)));
    TC_ASSERT_FUNC((test(S("litpcfdghe"), "mgojkldsqh", 0, 1)));
    TC_ASSERT_FUNC((test(S("aidjksrolc"), "imqnaghkfrdtlopbjesc", 0, S::npos)));
    TC_ASSERT_FUNC((test(S("qpghtfbaji"), "", 1, 1)));
    TC_ASSERT_FUNC((test(S("gfshlcmdjr"), "nadkh", 1, 1)));
    TC_ASSERT_FUNC((test(S("nkodajteqp"), "ofdrqmkebl", 1, 4)));
    TC_ASSERT_FUNC((test(S("gbmetiprqd"), "bdfjqgatlksriohemnpc", 1, S::npos)));
    TC_ASSERT_FUNC((test(S("crnklpmegd"), "", 5, 5)));
    TC_ASSERT_FUNC((test(S("jsbtafedoc"), "prqgn", 5, 5)));
    TC_ASSERT_FUNC((test(S("qnmodrtkeb"), "pejafmnokr", 5, 6)));
    TC_ASSERT_FUNC((test(S("cpebqsfmnj"), "odnqkgijrhabfmcestlp", 5, S::npos)));
    TC_ASSERT_FUNC((test(S("lmofqdhpki"), "", 9, 9)));
    TC_ASSERT_FUNC((test(S("hnefkqimca"), "rtjpa", 9, S::npos)));
    TC_ASSERT_FUNC((test(S("drtasbgmfp"), "ktsrmnqagd", 9, 9)));
    TC_ASSERT_FUNC((test(S("lsaijeqhtr"), "rtdhgcisbnmoaqkfpjle", 9, S::npos)));
    TC_ASSERT_FUNC((test(S("elgofjmbrq"), "", 10, S::npos)));
    TC_ASSERT_FUNC((test(S("mjqdgalkpc"), "dplqa", 10, S::npos)));
    TC_ASSERT_FUNC((test(S("kthqnfcerm"), "dkacjoptns", 10, S::npos)));
    TC_ASSERT_FUNC((test(S("dfsjhanorc"), "hqfimtrgnbekpdcsjalo", 10, S::npos)));
    TC_ASSERT_FUNC((test(S("eqsgalomhb"), "", 11, S::npos)));
    TC_ASSERT_FUNC((test(S("akiteljmoh"), "lofbc", 11, S::npos)));
    TC_ASSERT_FUNC((test(S("hlbdfreqjo"), "astoegbfpn", 11, S::npos)));
    TC_ASSERT_FUNC((test(S("taqobhlerg"), "pdgreqomsncafklhtibj", 11, S::npos)));
    TC_ASSERT_FUNC((test(S("snafbdlghrjkpqtoceim"), "", 0, 0)));
    TC_ASSERT_FUNC((test(S("aemtbrgcklhndjisfpoq"), "lbtqd", 0, 0)));
    TC_ASSERT_FUNC((test(S("pnracgfkjdiholtbqsem"), "tboimldpjh", 0, 1)));
    TC_ASSERT_FUNC((test(S("dicfltehbsgrmojnpkaq"), "slcerthdaiqjfnobgkpm", 0, S::npos)));
    TC_ASSERT_FUNC((test(S("jlnkraeodhcspfgbqitm"), "", 1, 1)));
    TC_ASSERT_FUNC((test(S("lhosrngtmfjikbqpcade"), "aqibs", 1, 1)));
    TC_ASSERT_FUNC((test(S("rbtaqjhgkneisldpmfoc"), "gtfblmqinc", 1, 3)));
    TC_ASSERT_FUNC((test(S("gpifsqlrdkbonjtmheca"), "mkqpbtdalgniorhfescj", 1, S::npos)));
    TC_ASSERT_FUNC((test(S("hdpkobnsalmcfijregtq"), "", 10, 10)));
    TC_ASSERT_FUNC((test(S("jtlshdgqaiprkbcoenfm"), "pblas", 10, 11)));
    TC_ASSERT_FUNC((test(S("fkdrbqltsgmcoiphneaj"), "arosdhcfme", 10, 13)));
    TC_ASSERT_FUNC((test(S("crsplifgtqedjohnabmk"), "blkhjeogicatqfnpdmsr", 10, S::npos)));
    TC_ASSERT_FUNC((test(S("niptglfbosehkamrdqcj"), "", 19, 19)));
    TC_ASSERT_FUNC((test(S("copqdhstbingamjfkler"), "djkqc", 19, 19)));
    TC_ASSERT_FUNC((test(S("mrtaefilpdsgocnhqbjk"), "lgokshjtpb", 19, S::npos)));
    TC_ASSERT_FUNC((test(S("kojatdhlcmigpbfrqnes"), "bqjhtkfepimcnsgrlado", 19, S::npos)));
    TC_ASSERT_FUNC((test(S("eaintpchlqsbdgrkjofm"), "", 20, S::npos)));
    TC_ASSERT_FUNC((test(S("gjnhidfsepkrtaqbmclo"), "nocfa", 20, S::npos)));
    TC_ASSERT_FUNC((test(S("spocfaktqdbiejlhngmr"), "bgtajmiedc", 20, S::npos)));
    TC_ASSERT_FUNC((test(S("rphmlekgfscndtaobiqj"), "lsckfnqgdahejiopbtmr", 20, S::npos)));
    TC_ASSERT_FUNC((test(S("liatsqdoegkmfcnbhrpj"), "", 21, S::npos)));
    TC_ASSERT_FUNC((test(S("binjagtfldkrspcomqeh"), "gfsrt", 21, S::npos)));
    TC_ASSERT_FUNC((test(S("latkmisecnorjbfhqpdg"), "pfsocbhjtm", 21, S::npos)));
    TC_ASSERT_FUNC((test(S("lecfratdjkhnsmqpoigb"), "tpflmdnoicjgkberhqsa", 21, S::npos)));
    return 0;
}

template <class S>
static int test1()
{
    TC_ASSERT_FUNC((test(S(""), "", S::npos)));
    TC_ASSERT_FUNC((test(S(""), "laenf", S::npos)));
    TC_ASSERT_FUNC((test(S(""), "pqlnkmbdjo", S::npos)));
    TC_ASSERT_FUNC((test(S(""), "qkamfogpnljdcshbreti", S::npos)));
    TC_ASSERT_FUNC((test(S("nhmko"), "", 0)));
    TC_ASSERT_FUNC((test(S("lahfb"), "irkhs", 0)));
    TC_ASSERT_FUNC((test(S("gmfhd"), "kantesmpgj", 2)));
    TC_ASSERT_FUNC((test(S("odaft"), "oknlrstdpiqmjbaghcfe", S::npos)));
    TC_ASSERT_FUNC((test(S("eolhfgpjqk"), "", 0)));
    TC_ASSERT_FUNC((test(S("nbatdlmekr"), "bnrpe", 2)));
    TC_ASSERT_FUNC((test(S("jdmciepkaq"), "jtdaefblso", 2)));
    TC_ASSERT_FUNC((test(S("hkbgspoflt"), "oselktgbcapndfjihrmq", S::npos)));
    TC_ASSERT_FUNC((test(S("gprdcokbnjhlsfmtieqa"), "", 0)));
    TC_ASSERT_FUNC((test(S("qjghlnftcaismkropdeb"), "bjaht", 0)));
    TC_ASSERT_FUNC((test(S("pnalfrdtkqcmojiesbhg"), "hjlcmgpket", 1)));
    TC_ASSERT_FUNC((test(S("pniotcfrhqsmgdkjbael"), "htaobedqikfplcgjsmrn", S::npos)));
    return 0;
}

int tc_libcxx_strings_string_find_first_not_of_pointer_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test0<S>()));
    TC_ASSERT_FUNC((test1<S>()));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
