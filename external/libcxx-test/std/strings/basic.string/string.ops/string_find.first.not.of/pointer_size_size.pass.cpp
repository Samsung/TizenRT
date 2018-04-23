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

// size_type find_first_not_of(const charT* s, size_type pos, size_type n) const;

#include <string>
#include <cassert>
#include "libcxx_tc_common.h"


template <class S>
static int
test(const S& s, const typename S::value_type* str, typename S::size_type pos,
     typename S::size_type n, typename S::size_type x)
{
    TC_ASSERT_EXPR(s.find_first_not_of(str, pos, n) == x);
    if (x != S::npos)
        TC_ASSERT_EXPR(pos <= x && x < s.size());
    return 0;
}

template <class S>
static int test0()
{
    TC_ASSERT_FUNC((test(S(""), "", 0, 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "irkhs", 0, 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "kante", 0, 1, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "oknlr", 0, 2, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "pcdro", 0, 4, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "bnrpe", 0, 5, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "jtdaefblso", 0, 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "oselktgbca", 0, 1, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "eqgaplhckj", 0, 5, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "bjahtcmnlp", 0, 9, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "hjlcmgpket", 0, 10, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "htaobedqikfplcgjsmrn", 0, 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "hpqiarojkcdlsgnmfetb", 0, 1, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "dfkaprhjloqetcsimnbg", 0, 10, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "ihqrfebgadntlpmjksoc", 0, 19, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "ngtjfcalbseiqrphmkdo", 0, 20, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "", 1, 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "lbtqd", 1, 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "tboim", 1, 1, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "slcer", 1, 2, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "cbjfs", 1, 4, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "aqibs", 1, 5, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "gtfblmqinc", 1, 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "mkqpbtdalg", 1, 1, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "kphatlimcd", 1, 5, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "pblasqogic", 1, 9, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "arosdhcfme", 1, 10, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "blkhjeogicatqfnpdmsr", 1, 0, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "bmhineprjcoadgstflqk", 1, 1, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "djkqcmetslnghpbarfoi", 1, 10, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "lgokshjtpbemarcdqnfi", 1, 19, S::npos)));
    TC_ASSERT_FUNC((test(S(""), "bqjhtkfepimcnsgrlado", 1, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("eaint"), "", 0, 0, 0)));
    TC_ASSERT_FUNC((test(S("binja"), "gfsrt", 0, 0, 0)));
    TC_ASSERT_FUNC((test(S("latkm"), "pfsoc", 0, 1, 0)));
    TC_ASSERT_FUNC((test(S("lecfr"), "tpflm", 0, 2, 0)));
    TC_ASSERT_FUNC((test(S("eqkst"), "sgkec", 0, 4, 1)));
    TC_ASSERT_FUNC((test(S("cdafr"), "romds", 0, 5, 0)));
    TC_ASSERT_FUNC((test(S("prbhe"), "qhjistlgmr", 0, 0, 0)));
    TC_ASSERT_FUNC((test(S("lbisk"), "pedfirsglo", 0, 1, 0)));
    TC_ASSERT_FUNC((test(S("hrlpd"), "aqcoslgrmk", 0, 5, 0)));
    TC_ASSERT_FUNC((test(S("ehmja"), "dabckmepqj", 0, 9, 1)));
    TC_ASSERT_FUNC((test(S("mhqgd"), "pqscrjthli", 0, 10, 0)));
    TC_ASSERT_FUNC((test(S("tgklq"), "kfphdcsjqmobliagtren", 0, 0, 0)));
    TC_ASSERT_FUNC((test(S("bocjs"), "rokpefncljibsdhqtagm", 0, 1, 0)));
    TC_ASSERT_FUNC((test(S("grbsd"), "afionmkphlebtcjqsgrd", 0, 10, 0)));
    TC_ASSERT_FUNC((test(S("ofjqr"), "aenmqplidhkofrjbctsg", 0, 19, S::npos)));
    TC_ASSERT_FUNC((test(S("btlfi"), "osjmbtcadhiklegrpqnf", 0, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("clrgb"), "", 1, 0, 1)));
    TC_ASSERT_FUNC((test(S("tjmek"), "osmia", 1, 0, 1)));
    TC_ASSERT_FUNC((test(S("bgstp"), "ckonl", 1, 1, 1)));
    TC_ASSERT_FUNC((test(S("hstrk"), "ilcaj", 1, 2, 1)));
    TC_ASSERT_FUNC((test(S("kmspj"), "lasiq", 1, 4, 1)));
    TC_ASSERT_FUNC((test(S("tjboh"), "kfqmr", 1, 5, 1)));
    TC_ASSERT_FUNC((test(S("ilbcj"), "klnitfaobg", 1, 0, 1)));
    TC_ASSERT_FUNC((test(S("jkngf"), "gjhmdlqikp", 1, 1, 1)));
    TC_ASSERT_FUNC((test(S("gfcql"), "skbgtahqej", 1, 5, 1)));
    TC_ASSERT_FUNC((test(S("dqtlg"), "bjsdgtlpkf", 1, 9, 1)));
    TC_ASSERT_FUNC((test(S("bthpg"), "bjgfmnlkio", 1, 10, 1)));
    TC_ASSERT_FUNC((test(S("dgsnq"), "lbhepotfsjdqigcnamkr", 1, 0, 1)));
    TC_ASSERT_FUNC((test(S("rmfhp"), "tebangckmpsrqdlfojhi", 1, 1, 1)));
    TC_ASSERT_FUNC((test(S("jfdam"), "joflqbdkhtegimscpanr", 1, 10, 3)));
    TC_ASSERT_FUNC((test(S("edapb"), "adpmcohetfbsrjinlqkg", 1, 19, S::npos)));
    TC_ASSERT_FUNC((test(S("brfsm"), "iacldqjpfnogbsrhmetk", 1, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("ndrhl"), "", 2, 0, 2)));
    TC_ASSERT_FUNC((test(S("mrecp"), "otkgb", 2, 0, 2)));
    TC_ASSERT_FUNC((test(S("qlasf"), "cqsjl", 2, 1, 2)));
    TC_ASSERT_FUNC((test(S("smaqd"), "dpifl", 2, 2, 2)));
    TC_ASSERT_FUNC((test(S("hjeni"), "oapht", 2, 4, 2)));
    TC_ASSERT_FUNC((test(S("ocmfj"), "cifts", 2, 5, 2)));
    TC_ASSERT_FUNC((test(S("hmftq"), "nmsckbgalo", 2, 0, 2)));
    TC_ASSERT_FUNC((test(S("fklad"), "tpksqhamle", 2, 1, 2)));
    TC_ASSERT_FUNC((test(S("dirnm"), "tpdrchmkji", 2, 5, 3)));
    TC_ASSERT_FUNC((test(S("hrgdc"), "ijagfkblst", 2, 9, 3)));
    TC_ASSERT_FUNC((test(S("ifakg"), "kpocsignjb", 2, 10, 2)));
    TC_ASSERT_FUNC((test(S("ebrgd"), "pecqtkjsnbdrialgmohf", 2, 0, 2)));
    TC_ASSERT_FUNC((test(S("rcjml"), "aiortphfcmkjebgsndql", 2, 1, 2)));
    TC_ASSERT_FUNC((test(S("peqmt"), "sdbkeamglhipojqftrcn", 2, 10, 2)));
    TC_ASSERT_FUNC((test(S("frehn"), "ljqncehgmfktroapidbs", 2, 19, S::npos)));
    TC_ASSERT_FUNC((test(S("tqolf"), "rtcfodilamkbenjghqps", 2, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("cjgao"), "", 4, 0, 4)));
    TC_ASSERT_FUNC((test(S("kjplq"), "mabns", 4, 0, 4)));
    TC_ASSERT_FUNC((test(S("herni"), "bdnrp", 4, 1, 4)));
    TC_ASSERT_FUNC((test(S("tadrb"), "scidp", 4, 2, 4)));
    TC_ASSERT_FUNC((test(S("pkfeo"), "agbjl", 4, 4, 4)));
    TC_ASSERT_FUNC((test(S("hoser"), "jfmpr", 4, 5, S::npos)));
    TC_ASSERT_FUNC((test(S("kgrsp"), "rbpefghsmj", 4, 0, 4)));
    TC_ASSERT_FUNC((test(S("pgejb"), "apsfntdoqc", 4, 1, 4)));
    TC_ASSERT_FUNC((test(S("thlnq"), "ndkjeisgcl", 4, 5, 4)));
    TC_ASSERT_FUNC((test(S("nbmit"), "rnfpqatdeo", 4, 9, S::npos)));
    TC_ASSERT_FUNC((test(S("jgmib"), "bntjlqrfik", 4, 10, S::npos)));
    TC_ASSERT_FUNC((test(S("ncrfj"), "kcrtmpolnaqejghsfdbi", 4, 0, 4)));
    TC_ASSERT_FUNC((test(S("ncsik"), "lobheanpkmqidsrtcfgj", 4, 1, 4)));
    TC_ASSERT_FUNC((test(S("sgbfh"), "athdkljcnreqbgpmisof", 4, 10, S::npos)));
    TC_ASSERT_FUNC((test(S("dktbn"), "qkdmjialrscpbhefgont", 4, 19, S::npos)));
    TC_ASSERT_FUNC((test(S("fthqm"), "dmasojntqleribkgfchp", 4, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("klopi"), "", 5, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("dajhn"), "psthd", 5, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("jbgno"), "rpmjd", 5, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("hkjae"), "dfsmk", 5, 2, S::npos)));
    return 0;
}

template <class S>
static int test1()
{
    TC_ASSERT_FUNC((test(S("gbhqo"), "skqne", 5, 4, S::npos)));
    TC_ASSERT_FUNC((test(S("ktdor"), "kipnf", 5, 5, S::npos)));
    TC_ASSERT_FUNC((test(S("ldprn"), "hmrnqdgifl", 5, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("egmjk"), "fsmjcdairn", 5, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("armql"), "pcdgltbrfj", 5, 5, S::npos)));
    TC_ASSERT_FUNC((test(S("cdhjo"), "aekfctpirg", 5, 9, S::npos)));
    TC_ASSERT_FUNC((test(S("jcons"), "ledihrsgpf", 5, 10, S::npos)));
    TC_ASSERT_FUNC((test(S("cbrkp"), "mqcklahsbtirgopefndj", 5, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("fhgna"), "kmlthaoqgecrnpdbjfis", 5, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("ejfcd"), "sfhbamcdptojlkrenqgi", 5, 10, S::npos)));
    TC_ASSERT_FUNC((test(S("kqjhe"), "pbniofmcedrkhlstgaqj", 5, 19, S::npos)));
    TC_ASSERT_FUNC((test(S("pbdjl"), "mongjratcskbhqiepfdl", 5, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("gajqn"), "", 6, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("stedk"), "hrnat", 6, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("tjkaf"), "gsqdt", 6, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("dthpe"), "bspkd", 6, 2, S::npos)));
    TC_ASSERT_FUNC((test(S("klhde"), "ohcmb", 6, 4, S::npos)));
    TC_ASSERT_FUNC((test(S("bhlki"), "heatr", 6, 5, S::npos)));
    TC_ASSERT_FUNC((test(S("lqmoh"), "pmblckedfn", 6, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("mtqin"), "aceqmsrbik", 6, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("dpqbr"), "lmbtdehjrn", 6, 5, S::npos)));
    TC_ASSERT_FUNC((test(S("kdhmo"), "teqmcrlgib", 6, 9, S::npos)));
    TC_ASSERT_FUNC((test(S("jblqp"), "njolbmspac", 6, 10, S::npos)));
    TC_ASSERT_FUNC((test(S("qmjgl"), "pofnhidklamecrbqjgst", 6, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("rothp"), "jbhckmtgrqnosafedpli", 6, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("ghknq"), "dobntpmqklicsahgjerf", 6, 10, S::npos)));
    TC_ASSERT_FUNC((test(S("eopfi"), "tpdshainjkbfoemlrgcq", 6, 19, S::npos)));
    TC_ASSERT_FUNC((test(S("dsnmg"), "oldpfgeakrnitscbjmqh", 6, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("jnkrfhotgl"), "", 0, 0, 0)));
    TC_ASSERT_FUNC((test(S("dltjfngbko"), "rqegt", 0, 0, 0)));
    TC_ASSERT_FUNC((test(S("bmjlpkiqde"), "dashm", 0, 1, 0)));
    TC_ASSERT_FUNC((test(S("skrflobnqm"), "jqirk", 0, 2, 0)));
    TC_ASSERT_FUNC((test(S("jkpldtshrm"), "rckeg", 0, 4, 0)));
    TC_ASSERT_FUNC((test(S("ghasdbnjqo"), "jscie", 0, 5, 0)));
    TC_ASSERT_FUNC((test(S("igrkhpbqjt"), "efsphndliq", 0, 0, 0)));
    TC_ASSERT_FUNC((test(S("ikthdgcamf"), "gdicosleja", 0, 1, 0)));
    TC_ASSERT_FUNC((test(S("pcofgeniam"), "qcpjibosfl", 0, 5, 2)));
    TC_ASSERT_FUNC((test(S("rlfjgesqhc"), "lrhmefnjcq", 0, 9, 4)));
    TC_ASSERT_FUNC((test(S("itphbqsker"), "dtablcrseo", 0, 10, 0)));
    TC_ASSERT_FUNC((test(S("skjafcirqm"), "apckjsftedbhgomrnilq", 0, 0, 0)));
    TC_ASSERT_FUNC((test(S("tcqomarsfd"), "pcbrgflehjtiadnsokqm", 0, 1, 0)));
    TC_ASSERT_FUNC((test(S("rocfeldqpk"), "nsiadegjklhobrmtqcpf", 0, 10, 0)));
    TC_ASSERT_FUNC((test(S("cfpegndlkt"), "cpmajdqnolikhgsbretf", 0, 19, 1)));
    TC_ASSERT_FUNC((test(S("fqbtnkeasj"), "jcflkntmgiqrphdosaeb", 0, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("shbcqnmoar"), "", 1, 0, 1)));
    TC_ASSERT_FUNC((test(S("bdoshlmfin"), "ontrs", 1, 0, 1)));
    TC_ASSERT_FUNC((test(S("khfrebnsgq"), "pfkna", 1, 1, 1)));
    TC_ASSERT_FUNC((test(S("getcrsaoji"), "ekosa", 1, 2, 2)));
    TC_ASSERT_FUNC((test(S("fjiknedcpq"), "anqhk", 1, 4, 1)));
    TC_ASSERT_FUNC((test(S("tkejgnafrm"), "jekca", 1, 5, 4)));
    TC_ASSERT_FUNC((test(S("jnakolqrde"), "ikemsjgacf", 1, 0, 1)));
    TC_ASSERT_FUNC((test(S("lcjptsmgbe"), "arolgsjkhm", 1, 1, 1)));
    TC_ASSERT_FUNC((test(S("itfsmcjorl"), "oftkbldhre", 1, 5, 3)));
    TC_ASSERT_FUNC((test(S("omchkfrjea"), "gbkqdoeftl", 1, 9, 1)));
    TC_ASSERT_FUNC((test(S("cigfqkated"), "sqcflrgtim", 1, 10, 5)));
    TC_ASSERT_FUNC((test(S("tscenjikml"), "fmhbkislrjdpanogqcet", 1, 0, 1)));
    TC_ASSERT_FUNC((test(S("qcpaemsinf"), "rnioadktqlgpbcjsmhef", 1, 1, 1)));
    TC_ASSERT_FUNC((test(S("gltkojeipd"), "oakgtnldpsefihqmjcbr", 1, 10, 5)));
    TC_ASSERT_FUNC((test(S("qistfrgnmp"), "gbnaelosidmcjqktfhpr", 1, 19, 5)));
    TC_ASSERT_FUNC((test(S("bdnpfcqaem"), "akbripjhlosndcmqgfet", 1, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("ectnhskflp"), "", 5, 0, 5)));
    TC_ASSERT_FUNC((test(S("fgtianblpq"), "pijag", 5, 0, 5)));
    TC_ASSERT_FUNC((test(S("mfeqklirnh"), "jrckd", 5, 1, 5)));
    TC_ASSERT_FUNC((test(S("astedncjhk"), "qcloh", 5, 2, 5)));
    TC_ASSERT_FUNC((test(S("fhlqgcajbr"), "thlmp", 5, 4, 5)));
    TC_ASSERT_FUNC((test(S("epfhocmdng"), "qidmo", 5, 5, 5)));
    TC_ASSERT_FUNC((test(S("apcnsibger"), "lnegpsjqrd", 5, 0, 5)));
    TC_ASSERT_FUNC((test(S("aqkocrbign"), "rjqdablmfs", 5, 1, 6)));
    TC_ASSERT_FUNC((test(S("ijsmdtqgce"), "enkgpbsjaq", 5, 5, 5)));
    TC_ASSERT_FUNC((test(S("clobgsrken"), "kdsgoaijfh", 5, 9, 6)));
    TC_ASSERT_FUNC((test(S("jbhcfposld"), "trfqgmckbe", 5, 10, 5)));
    TC_ASSERT_FUNC((test(S("oqnpblhide"), "igetsracjfkdnpoblhqm", 5, 0, 5)));
    TC_ASSERT_FUNC((test(S("lroeasctif"), "nqctfaogirshlekbdjpm", 5, 1, 5)));
    TC_ASSERT_FUNC((test(S("bpjlgmiedh"), "csehfgomljdqinbartkp", 5, 10, 6)));
    TC_ASSERT_FUNC((test(S("pamkeoidrj"), "qahoegcmplkfsjbdnitr", 5, 19, 8)));
    TC_ASSERT_FUNC((test(S("espogqbthk"), "dpteiajrqmsognhlfbkc", 5, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("shoiedtcjb"), "", 9, 0, 9)));
    TC_ASSERT_FUNC((test(S("ebcinjgads"), "tqbnh", 9, 0, 9)));
    TC_ASSERT_FUNC((test(S("dqmregkcfl"), "akmle", 9, 1, 9)));
    TC_ASSERT_FUNC((test(S("ngcrieqajf"), "iqfkm", 9, 2, 9)));
    TC_ASSERT_FUNC((test(S("qosmilgnjb"), "tqjsr", 9, 4, 9)));
    TC_ASSERT_FUNC((test(S("ikabsjtdfl"), "jplqg", 9, 5, S::npos)));
    TC_ASSERT_FUNC((test(S("ersmicafdh"), "oilnrbcgtj", 9, 0, 9)));
    TC_ASSERT_FUNC((test(S("fdnplotmgh"), "morkglpesn", 9, 1, 9)));
    TC_ASSERT_FUNC((test(S("fdbicojerm"), "dmicerngat", 9, 5, S::npos)));
    TC_ASSERT_FUNC((test(S("mbtafndjcq"), "radgeskbtc", 9, 9, 9)));
    TC_ASSERT_FUNC((test(S("mlenkpfdtc"), "ljikprsmqo", 9, 10, 9)));
    TC_ASSERT_FUNC((test(S("ahlcifdqgs"), "trqihkcgsjamfdbolnpe", 9, 0, 9)));
    TC_ASSERT_FUNC((test(S("bgjemaltks"), "lqmthbsrekajgnofcipd", 9, 1, 9)));
    TC_ASSERT_FUNC((test(S("pdhslbqrfc"), "jtalmedribkgqsopcnfh", 9, 10, 9)));
    TC_ASSERT_FUNC((test(S("dirhtsnjkc"), "spqfoiclmtagejbndkrh", 9, 19, S::npos)));
    TC_ASSERT_FUNC((test(S("dlroktbcja"), "nmotklspigjrdhcfaebq", 9, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("ncjpmaekbs"), "", 10, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("hlbosgmrak"), "hpmsd", 10, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("pqfhsgilen"), "qnpor", 10, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("gqtjsbdckh"), "otdma", 10, 2, S::npos)));
    TC_ASSERT_FUNC((test(S("cfkqpjlegi"), "efhjg", 10, 4, S::npos)));
    TC_ASSERT_FUNC((test(S("beanrfodgj"), "odpte", 10, 5, S::npos)));
    TC_ASSERT_FUNC((test(S("adtkqpbjfi"), "bctdgfmolr", 10, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("iomkfthagj"), "oaklidrbqg", 10, 1, S::npos)));
    return 0;
}

template <class S>
static int test2()
{
    TC_ASSERT_FUNC((test(S("sdpcilonqj"), "dnjfsagktr", 10, 5, S::npos)));
    TC_ASSERT_FUNC((test(S("gtfbdkqeml"), "nejaktmiqg", 10, 9, S::npos)));
    TC_ASSERT_FUNC((test(S("bmeqgcdorj"), "pjqonlebsf", 10, 10, S::npos)));
    TC_ASSERT_FUNC((test(S("etqlcanmob"), "dshmnbtolcjepgaikfqr", 10, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("roqmkbdtia"), "iogfhpabtjkqlrnemcds", 10, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("kadsithljf"), "ngridfabjsecpqltkmoh", 10, 10, S::npos)));
    TC_ASSERT_FUNC((test(S("sgtkpbfdmh"), "athmknplcgofrqejsdib", 10, 19, S::npos)));
    TC_ASSERT_FUNC((test(S("qgmetnabkl"), "ldobhmqcafnjtkeisgrp", 10, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("cqjohampgd"), "", 11, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("hobitmpsan"), "aocjb", 11, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("tjehkpsalm"), "jbrnk", 11, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("ngfbojitcl"), "tqedg", 11, 2, S::npos)));
    TC_ASSERT_FUNC((test(S("rcfkdbhgjo"), "nqskp", 11, 4, S::npos)));
    TC_ASSERT_FUNC((test(S("qghptonrea"), "eaqkl", 11, 5, S::npos)));
    TC_ASSERT_FUNC((test(S("hnprfgqjdl"), "reaoicljqm", 11, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("hlmgabenti"), "lsftgajqpm", 11, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("ofcjanmrbs"), "rlpfogmits", 11, 5, S::npos)));
    TC_ASSERT_FUNC((test(S("jqedtkornm"), "shkncmiaqj", 11, 9, S::npos)));
    TC_ASSERT_FUNC((test(S("rfedlasjmg"), "fpnatrhqgs", 11, 10, S::npos)));
    TC_ASSERT_FUNC((test(S("talpqjsgkm"), "sjclemqhnpdbgikarfot", 11, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("lrkcbtqpie"), "otcmedjikgsfnqbrhpla", 11, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("cipogdskjf"), "bonsaefdqiprkhlgtjcm", 11, 10, S::npos)));
    TC_ASSERT_FUNC((test(S("nqedcojahi"), "egpscmahijlfnkrodqtb", 11, 19, S::npos)));
    TC_ASSERT_FUNC((test(S("hefnrkmctj"), "kmqbfepjthgilscrndoa", 11, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("atqirnmekfjolhpdsgcb"), "", 0, 0, 0)));
    TC_ASSERT_FUNC((test(S("echfkmlpribjnqsaogtd"), "prboq", 0, 0, 0)));
    TC_ASSERT_FUNC((test(S("qnhiftdgcleajbpkrosm"), "fjcqh", 0, 1, 0)));
    TC_ASSERT_FUNC((test(S("chamfknorbedjitgslpq"), "fmosa", 0, 2, 0)));
    TC_ASSERT_FUNC((test(S("njhqpibfmtlkaecdrgso"), "qdbok", 0, 4, 0)));
    TC_ASSERT_FUNC((test(S("ebnghfsqkprmdcljoiat"), "amslg", 0, 5, 0)));
    TC_ASSERT_FUNC((test(S("letjomsgihfrpqbkancd"), "smpltjneqb", 0, 0, 0)));
    TC_ASSERT_FUNC((test(S("nblgoipcrqeaktshjdmf"), "flitskrnge", 0, 1, 0)));
    TC_ASSERT_FUNC((test(S("cehkbngtjoiflqapsmrd"), "pgqihmlbef", 0, 5, 0)));
    TC_ASSERT_FUNC((test(S("mignapfoklbhcqjetdrs"), "cfpdqjtgsb", 0, 9, 0)));
    TC_ASSERT_FUNC((test(S("ceatbhlsqjgpnokfrmdi"), "htpsiaflom", 0, 10, 0)));
    TC_ASSERT_FUNC((test(S("ocihkjgrdelpfnmastqb"), "kpjfiaceghsrdtlbnomq", 0, 0, 0)));
    TC_ASSERT_FUNC((test(S("noelgschdtbrjfmiqkap"), "qhtbomidljgafneksprc", 0, 1, 0)));
    TC_ASSERT_FUNC((test(S("dkclqfombepritjnghas"), "nhtjobkcefldimpsaqgr", 0, 10, 0)));
    TC_ASSERT_FUNC((test(S("miklnresdgbhqcojftap"), "prabcjfqnoeskilmtgdh", 0, 19, 11)));
    TC_ASSERT_FUNC((test(S("htbcigojaqmdkfrnlsep"), "dtrgmchilkasqoebfpjn", 0, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("febhmqtjanokscdirpgl"), "", 1, 0, 1)));
    TC_ASSERT_FUNC((test(S("loakbsqjpcrdhftniegm"), "sqome", 1, 0, 1)));
    TC_ASSERT_FUNC((test(S("reagphsqflbitdcjmkno"), "smfte", 1, 1, 1)));
    TC_ASSERT_FUNC((test(S("jitlfrqemsdhkopncabg"), "ciboh", 1, 2, 2)));
    TC_ASSERT_FUNC((test(S("mhtaepscdnrjqgbkifol"), "haois", 1, 4, 2)));
    TC_ASSERT_FUNC((test(S("tocesrfmnglpbjihqadk"), "abfki", 1, 5, 1)));
    TC_ASSERT_FUNC((test(S("lpfmctjrhdagneskbqoi"), "frdkocntmq", 1, 0, 1)));
    TC_ASSERT_FUNC((test(S("lsmqaepkdhncirbtjfgo"), "oasbpedlnr", 1, 1, 1)));
    TC_ASSERT_FUNC((test(S("epoiqmtldrabnkjhcfsg"), "kltqmhgand", 1, 5, 1)));
    TC_ASSERT_FUNC((test(S("emgasrilpknqojhtbdcf"), "gdtfjchpmr", 1, 9, 3)));
    TC_ASSERT_FUNC((test(S("hnfiagdpcklrjetqbsom"), "ponmcqblet", 1, 10, 2)));
    TC_ASSERT_FUNC((test(S("nsdfebgajhmtricpoklq"), "sgphqdnofeiklatbcmjr", 1, 0, 1)));
    TC_ASSERT_FUNC((test(S("atjgfsdlpobmeiqhncrk"), "ljqprsmigtfoneadckbh", 1, 1, 1)));
    TC_ASSERT_FUNC((test(S("sitodfgnrejlahcbmqkp"), "ligeojhafnkmrcsqtbdp", 1, 10, 2)));
    TC_ASSERT_FUNC((test(S("fraghmbiceknltjpqosd"), "lsimqfnjarbopedkhcgt", 1, 19, 13)));
    TC_ASSERT_FUNC((test(S("pmafenlhqtdbkirjsogc"), "abedmfjlghniorcqptks", 1, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("pihgmoeqtnakrjslcbfd"), "", 10, 0, 10)));
    TC_ASSERT_FUNC((test(S("gjdkeprctqblnhiafsom"), "hqtoa", 10, 0, 10)));
    TC_ASSERT_FUNC((test(S("mkpnblfdsahrcqijteog"), "cahif", 10, 1, 10)));
    TC_ASSERT_FUNC((test(S("gckarqnelodfjhmbptis"), "kehis", 10, 2, 10)));
    TC_ASSERT_FUNC((test(S("gqpskidtbclomahnrjfe"), "kdlmh", 10, 4, 11)));
    TC_ASSERT_FUNC((test(S("pkldjsqrfgitbhmaecno"), "paeql", 10, 5, 10)));
    TC_ASSERT_FUNC((test(S("aftsijrbeklnmcdqhgop"), "aghoqiefnb", 10, 0, 10)));
    TC_ASSERT_FUNC((test(S("mtlgdrhafjkbiepqnsoc"), "jrbqaikpdo", 10, 1, 10)));
    TC_ASSERT_FUNC((test(S("pqgirnaefthokdmbsclj"), "smjonaeqcl", 10, 5, 10)));
    TC_ASSERT_FUNC((test(S("kpdbgjmtherlsfcqoina"), "eqbdrkcfah", 10, 9, 11)));
    TC_ASSERT_FUNC((test(S("jrlbothiknqmdgcfasep"), "kapmsienhf", 10, 10, 10)));
    TC_ASSERT_FUNC((test(S("mjogldqferckabinptsh"), "jpqotrlenfcsbhkaimdg", 10, 0, 10)));
    TC_ASSERT_FUNC((test(S("apoklnefbhmgqcdrisjt"), "jlbmhnfgtcqprikeados", 10, 1, 10)));
    TC_ASSERT_FUNC((test(S("ifeopcnrjbhkdgatmqls"), "stgbhfmdaljnpqoicker", 10, 10, 11)));
    TC_ASSERT_FUNC((test(S("ckqhaiesmjdnrgolbtpf"), "oihcetflbjagdsrkmqpn", 10, 19, 11)));
    TC_ASSERT_FUNC((test(S("bnlgapfimcoterskqdjh"), "adtclebmnpjsrqfkigoh", 10, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("kgdlrobpmjcthqsafeni"), "", 19, 0, 19)));
    TC_ASSERT_FUNC((test(S("dfkechomjapgnslbtqir"), "beafg", 19, 0, 19)));
    TC_ASSERT_FUNC((test(S("rloadknfbqtgmhcsipje"), "iclat", 19, 1, 19)));
    TC_ASSERT_FUNC((test(S("mgjhkolrnadqbpetcifs"), "rkhnf", 19, 2, 19)));
    TC_ASSERT_FUNC((test(S("cmlfakiojdrgtbsphqen"), "clshq", 19, 4, 19)));
    TC_ASSERT_FUNC((test(S("kghbfipeomsntdalrqjc"), "dtcoj", 19, 5, S::npos)));
    TC_ASSERT_FUNC((test(S("eldiqckrnmtasbghjfpo"), "rqosnjmfth", 19, 0, 19)));
    TC_ASSERT_FUNC((test(S("abqjcfedgotihlnspkrm"), "siatdfqglh", 19, 1, 19)));
    TC_ASSERT_FUNC((test(S("qfbadrtjsimkolcenhpg"), "mrlshtpgjq", 19, 5, 19)));
    TC_ASSERT_FUNC((test(S("abseghclkjqifmtodrnp"), "adlcskgqjt", 19, 9, 19)));
    TC_ASSERT_FUNC((test(S("ibmsnlrjefhtdokacqpg"), "drshcjknaf", 19, 10, 19)));
    TC_ASSERT_FUNC((test(S("mrkfciqjebaponsthldg"), "etsaqroinghpkjdlfcbm", 19, 0, 19)));
    TC_ASSERT_FUNC((test(S("mjkticdeoqshpalrfbgn"), "sgepdnkqliambtrocfhj", 19, 1, 19)));
    TC_ASSERT_FUNC((test(S("rqnoclbdejgiphtfsakm"), "nlmcjaqgbsortfdihkpe", 19, 10, S::npos)));
    TC_ASSERT_FUNC((test(S("plkqbhmtfaeodjcrsing"), "racfnpmosldibqkghjet", 19, 19, S::npos)));
    TC_ASSERT_FUNC((test(S("oegalhmstjrfickpbndq"), "fjhdsctkqeiolagrnmbp", 19, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("rdtgjcaohpblniekmsfq"), "", 20, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("ofkqbnjetrmsaidphglc"), "ejanp", 20, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("grkpahljcftesdmonqib"), "odife", 20, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("jimlgbhfqkteospardcn"), "okaqd", 20, 2, S::npos)));
    TC_ASSERT_FUNC((test(S("gftenihpmslrjkqadcob"), "lcdbi", 20, 4, S::npos)));
    TC_ASSERT_FUNC((test(S("bmhldogtckrfsanijepq"), "fsqbj", 20, 5, S::npos)));
    TC_ASSERT_FUNC((test(S("nfqkrpjdesabgtlcmoih"), "bigdomnplq", 20, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("focalnrpiqmdkstehbjg"), "apiblotgcd", 20, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("rhqdspkmebiflcotnjga"), "acfhdenops", 20, 5, S::npos)));
    TC_ASSERT_FUNC((test(S("rahdtmsckfboqlpniegj"), "jopdeamcrk", 20, 9, S::npos)));
    TC_ASSERT_FUNC((test(S("fbkeiopclstmdqranjhg"), "trqncbkgmh", 20, 10, S::npos)));
    TC_ASSERT_FUNC((test(S("lifhpdgmbconstjeqark"), "tomglrkencbsfjqpihda", 20, 0, S::npos)));
    return 0;
}

template <class S>
static int test3()
{
    TC_ASSERT_FUNC((test(S("pboqganrhedjmltsicfk"), "gbkhdnpoietfcmrslajq", 20, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("klchabsimetjnqgorfpd"), "rtfnmbsglkjaichoqedp", 20, 10, S::npos)));
    TC_ASSERT_FUNC((test(S("sirfgmjqhctndbklaepo"), "ohkmdpfqbsacrtjnlgei", 20, 19, S::npos)));
    TC_ASSERT_FUNC((test(S("rlbdsiceaonqjtfpghkm"), "dlbrteoisgphmkncajfq", 20, 20, S::npos)));
    TC_ASSERT_FUNC((test(S("ecgdanriptblhjfqskom"), "", 21, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("fdmiarlpgcskbhoteqjn"), "sjrlo", 21, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("rlbstjqopignecmfadkh"), "qjpor", 21, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("grjpqmbshektdolcafni"), "odhfn", 21, 2, S::npos)));
    TC_ASSERT_FUNC((test(S("sakfcohtqnibprjmlged"), "qtfin", 21, 4, S::npos)));
    TC_ASSERT_FUNC((test(S("mjtdglasihqpocebrfkn"), "hpqfo", 21, 5, S::npos)));
    TC_ASSERT_FUNC((test(S("okaplfrntghqbmeicsdj"), "fabmertkos", 21, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("sahngemrtcjidqbklfpo"), "brqtgkmaej", 21, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("dlmsipcnekhbgoaftqjr"), "nfrdeihsgl", 21, 5, S::npos)));
    TC_ASSERT_FUNC((test(S("ahegrmqnoiklpfsdbcjt"), "hlfrosekpi", 21, 9, S::npos)));
    TC_ASSERT_FUNC((test(S("hdsjbnmlegtkqripacof"), "atgbkrjdsm", 21, 10, S::npos)));
    TC_ASSERT_FUNC((test(S("pcnedrfjihqbalkgtoms"), "blnrptjgqmaifsdkhoec", 21, 0, S::npos)));
    TC_ASSERT_FUNC((test(S("qjidealmtpskrbfhocng"), "ctpmdahebfqjgknloris", 21, 1, S::npos)));
    TC_ASSERT_FUNC((test(S("qeindtagmokpfhsclrbj"), "apnkeqthrmlbfodiscgj", 21, 10, S::npos)));
    TC_ASSERT_FUNC((test(S("kpfegbjhsrnodltqciam"), "jdgictpframeoqlsbknh", 21, 19, S::npos)));
    TC_ASSERT_FUNC((test(S("hnbrcplsjfgiktoedmaq"), "qprlsfojamgndekthibc", 21, 20, S::npos)));
    return 0;
}

int tc_libcxx_strings_string_find_first_not_of_pointer_size_size(void)
{
    {
    typedef std::string S;
    TC_ASSERT_FUNC((test0<S>()));
    TC_ASSERT_FUNC((test1<S>()));
    TC_ASSERT_FUNC((test2<S>()));
    TC_ASSERT_FUNC((test3<S>()));
    }
    TC_SUCCESS_RESULT();
    return 0;
}
