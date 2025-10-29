// Copyright 2023-2025 Beken
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

// This is a generated file, if you need to modify it, use the script to
// generate and modify all the struct.h, ll.h, reg.h, debug_dump.c files!

#include <soc/soc.h>
#include <os/mem.h>
#include "hal_port.h"
#include "can_hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#define IDLE_CNT				300
#define LOOP_PRT(x)				\
{								\
	if(!(--x))					\
	{							\
		return BK_FAIL;			\
	}							\
}

static can_core_br_tab_s can_br_tab[] = CAN_BR_MAP;
static can_hal_t s_can_hal_env;

uint32_t can_hal_get_rid_esi_value(void)
{
	return can_ll_get_rid_esi_value();
}

uint32_t can_hal_get_rid(void)
{
	return can_ll_get_rid();
}

uint32_t can_hal_get_rid_esi(void)
{
	return can_ll_get_rid_esi();
}

//reg :rbuf_ctrl

uint32_t can_hal_get_rbuf_ctrl_value(void)
{
	return can_ll_get_rbuf_ctrl_value();
}

uint32_t can_hal_get_rbuf_ctrl_dlc(void)
{
	return can_ll_get_rbuf_ctrl_dlc();
}

uint32_t can_hal_get_rbuf_ctrl_brs(void)
{
	return can_ll_get_rbuf_ctrl_brs();
}

uint32_t can_hal_get_rbuf_ctrl_fdf(void)
{
	return can_ll_get_rbuf_ctrl_fdf();
}

uint32_t can_hal_get_rbuf_ctrl_rtr(void)
{
	return can_ll_get_rbuf_ctrl_rtr();
}

uint32_t can_hal_get_rbuf_ctrl_ide(void)
{
	return can_ll_get_rbuf_ctrl_ide();
}

//reg :rdata

uint32_t can_hal_get_rdata_value(uint32_t i)
{
	return can_ll_get_rdata_value(i);
}

//reg :tid

void can_hal_set_tid_esi_value(uint32_t v)
{
	can_ll_set_tid_esi_value(v);
}

uint32_t can_hal_get_tid_esi_value(void)
{
	return can_ll_get_tid_esi_value();
}

void can_hal_set_tid(uint32_t v)
{
	can_ll_set_tid(v);
}

uint32_t can_hal_get_tid(void)
{
	return can_ll_get_tid();
}

void can_hal_set_tid_esi(uint32_t v)
{
	can_ll_set_tid_esi(v);
}

uint32_t can_hal_get_tid_esi(void)
{
	return can_ll_get_tid_esi();
}

//reg :tbuf_ctrl

void can_hal_set_tbuf_ctrl_value(uint32_t v)
{
	can_ll_set_tbuf_ctrl_value(v);
}

uint32_t can_hal_get_tbuf_ctrl_value(void)
{
	return can_ll_get_tbuf_ctrl_value();
}

void can_hal_set_tbuf_ctrl_dlc(uint32_t v)
{
	can_ll_set_tbuf_ctrl_dlc(v);
}

uint32_t can_hal_get_tbuf_ctrl_dlc(void)
{
	return can_ll_get_tbuf_ctrl_dlc();
}

void can_hal_set_tbuf_ctrl_brs(uint32_t v)
{
	can_ll_set_tbuf_ctrl_brs(v);
}

uint32_t can_hal_get_tbuf_ctrl_brs(void)
{
	return can_ll_get_tbuf_ctrl_brs();
}

void can_hal_set_tbuf_ctrl_fdf(uint32_t v)
{
	can_ll_set_tbuf_ctrl_fdf(v);
}

uint32_t can_hal_get_tbuf_ctrl_fdf(void)
{
	return can_ll_get_tbuf_ctrl_fdf();
}

void can_hal_set_tbuf_ctrl_rtr(uint32_t v)
{
	can_ll_set_tbuf_ctrl_rtr(v);
}

uint32_t can_hal_get_tbuf_ctrl_rtr(void)
{
	return can_ll_get_tbuf_ctrl_rtr();
}

void can_hal_set_tbuf_ctrl_ide(uint32_t v)
{
	can_ll_set_tbuf_ctrl_ide(v);
}

uint32_t can_hal_get_tbuf_ctrl_ide(void)
{
	return can_ll_get_tbuf_ctrl_ide();
}

//reg :tdata

void can_hal_set_tdata(uint32_t i, uint32_t v)
{
	can_ll_set_tdata(i, v);
}


uint32_t can_hal_get_tdata(uint32_t i)
{
	return can_ll_get_tdata(i);
}

//reg :tts

uint32_t can_hal_get_tts_l_value(void)
{
	return can_ll_get_tts_l_value();
}

//reg :

uint32_t can_hal_get_tts_h_value(void)
{
	return can_ll_get_tts_h_value();
}

//reg :cfg_stat

void can_hal_set_cfg_value(uint32_t v)
{
	can_ll_set_cfg_value(v);
}

uint32_t can_hal_get_cfg_value(void)
{
	return can_ll_get_cfg_value();
}

void can_hal_set_busoff(uint32_t v)
{
	can_ll_set_busoff(v);
}

uint32_t can_hal_get_busoff(void)
{
	return can_ll_get_busoff();
}

uint32_t can_hal_get_tactive(void)
{
	return can_ll_get_tactive();
}

uint32_t can_hal_get_ractive(void)
{
	return can_ll_get_ractive();
}

void can_hal_set_tsss(uint32_t v)
{
	can_ll_set_tsss(v);
}

uint32_t can_hal_get_tsss(void)
{
	return can_ll_get_tsss();
}

void can_hal_set_tpss(uint32_t v)
{
	can_ll_set_tpss(v);
}

uint32_t can_hal_get_tpss(void)
{
	return can_ll_get_tpss();
}

void can_hal_set_lbmi(uint32_t v)
{
	can_ll_set_lbmi(v);
}

uint32_t can_hal_get_lbmi(void)
{
	return can_ll_get_lbmi();
}

void can_hal_set_lbme(uint32_t v)
{
	can_ll_set_lbme(v);
}

uint32_t can_hal_get_lbme(void)
{
	return can_ll_get_lbme();
}

void can_hal_set_reset(uint32_t v)
{
	can_ll_set_reset(v);
}

uint32_t can_hal_get_reset(void)
{
	return can_ll_get_reset();
}

//reg :tcmd

void can_hal_set_tsa(uint32_t v)
{
	can_ll_set_tsa(v);
}

uint32_t can_hal_get_tsa(void)
{
	return can_ll_get_tsa();
}

void can_hal_set_tsall(uint32_t v)
{
	can_ll_set_tsall(v);
}

uint32_t can_hal_get_tsall(void)
{
	return can_ll_get_tsall();
}

void can_hal_set_tsone(uint32_t v)
{
	can_ll_set_tsone(v);
}

uint32_t can_hal_get_tsone(void)
{
	return can_ll_get_tsone();
}

void can_hal_set_tpa(uint32_t v)
{
	can_ll_set_tpa(v);
}

uint32_t can_hal_get_tpa(void)
{
	return can_ll_get_tpa();
}

void can_hal_set_tpe(uint32_t v)
{
	can_ll_set_tpe(v);
}

uint32_t can_hal_get_tpe(void)
{
	return can_ll_get_tpe();
}

void can_hal_set_stby(uint32_t v)
{
	can_ll_set_stby(v);
}

uint32_t can_hal_get_stby(void)
{
	return can_ll_get_stby();
}

void can_hal_set_lom(uint32_t v)
{
	can_ll_set_lom(v);
}

uint32_t can_hal_get_lom(void)
{
	return can_ll_get_lom();
}

void can_hal_set_tbsel(uint32_t v)
{
	can_ll_set_tbsel(v);
}

uint32_t can_hal_get_tbsel(void)
{
	return can_ll_get_tbsel();
}

//reg :tctrl

uint32_t can_hal_get_tsstat(void)
{
	return can_ll_get_tsstat();
}

void can_hal_set_tttbm(uint32_t v)
{
	can_ll_set_tttbm(v);
}

uint32_t can_hal_get_tttbm(void)
{
	return can_ll_get_tttbm();
}

void can_hal_set_tsmode(uint32_t v)
{
	can_ll_set_tsmode(v);
}

uint32_t can_hal_get_tsmode(void)
{
	return can_ll_get_tsmode();
}

void can_hal_set_tsnext(uint32_t v)
{
	can_ll_set_tsnext(v);
}

uint32_t can_hal_get_tsnext (void)
{
	return can_ll_get_tsnext();
}

void can_hal_set_fd_iso(uint32_t v)
{
	can_ll_set_fd_iso(v);
}

uint32_t can_hal_get_fd_iso(void)
{
	return can_ll_get_fd_iso();
}

//reg :rctrl

uint32_t can_hal_get_rstat(void)
{
	return can_ll_get_rstat();
}

void can_hal_set_rball(uint32_t v)
{
	can_ll_set_rball(v);
}

uint32_t can_hal_get_rball(void)
{
	return can_ll_get_rball();
}

void can_hal_set_rrel(uint32_t v)
{
	can_ll_set_rrel(v);
}

uint32_t can_hal_get_rrel(void)
{
	return can_ll_get_rrel();
}

uint32_t can_hal_get_rov(void)
{
	return can_ll_get_rov();
}

void can_hal_set_rom(uint32_t v)
{
	can_ll_set_rom(v);
}

uint32_t can_hal_get_rom(void)
{
	return can_ll_get_rom();
}

void can_hal_set_sack(uint32_t v)
{
	can_ll_set_sack(v);
}

uint32_t can_hal_get_sack(void)
{
	return can_ll_get_sack();
}

//reg :trie

void can_hal_set_ie_value(uint32_t v)
{
	can_ll_set_ie_value(v);
}

uint32_t can_hal_get_ie_value(void)
{
	return can_ll_get_ie_value();
}

uint32_t can_hal_get_tsff(void)
{
	return can_ll_get_tsff();
}

void can_hal_set_eie(uint32_t v)
{
	can_ll_set_eie(v);
}

uint32_t can_hal_get_eie(void)
{
	return can_ll_get_eie();
}

void can_hal_set_tsie(uint32_t v)
{
	can_ll_set_tsie(v);
}

uint32_t can_hal_get_tsie(void)
{
	return can_ll_get_tsie();
}

void can_hal_set_tpie(uint32_t v)
{
	can_ll_set_tpie(v);
}

uint32_t can_hal_get_tpie(void)
{
	return can_ll_get_tpie();
}

void can_hal_set_rafie(uint32_t v)
{
	can_ll_set_rafie(v);
}

uint32_t can_hal_get_rafie(void)
{
	return can_ll_get_rafie();
}

void can_hal_set_rfie(uint32_t v)
{
	can_ll_set_rfie(v);
}

uint32_t can_hal_get_rfie(void)
{
	return can_ll_get_rfie();
}

void can_hal_set_roie(uint32_t v)
{
	can_ll_set_roie(v);
}

uint32_t can_hal_get_roie(void)
{
	return can_ll_get_roie();
}

void can_hal_set_rie(uint32_t v)
{
	can_ll_set_rie(v);
}

uint32_t can_hal_get_rie(void)
{
	return can_ll_get_rie();
}

//reg :trif

void can_hal_set_aif(uint32_t v)
{
	can_ll_set_aif(v);
}

uint32_t can_hal_get_aif(void)
{
	return can_ll_get_aif();
}

void can_hal_set_eif(uint32_t v)
{
	can_ll_set_eif(v);
}

uint32_t can_hal_get_eif(void)
{
	return can_ll_get_eif();
}

void can_hal_set_tsif(uint32_t v)
{
	can_ll_set_tsif(v);
}

uint32_t can_hal_get_tsif(void)
{
	return can_ll_get_tsif();
}

void can_hal_set_tpif(uint32_t v)
{
	can_ll_set_tpif(v);
}

uint32_t can_hal_get_tpif(void)
{
	return can_ll_get_tpif();
}

void can_hal_set_rafif(uint32_t v)
{
	can_ll_set_rafif(v);
}

uint32_t can_hal_get_rafif (void)
{
	return can_ll_get_rafif();
}

void can_hal_set_rfif(uint32_t v)
{
	can_ll_set_rfif(v);
}

uint32_t can_hal_get_rfif(void)
{
	return can_ll_get_rfif();
}

void can_hal_set_roif(uint32_t v)
{
	can_ll_set_roif(v);
}

uint32_t can_hal_get_roif(void)
{
	return can_ll_get_roif();
}

void can_hal_set_rif(uint32_t v)
{
	can_ll_set_rif(v);
}

uint32_t can_hal_get_rif(void)
{
	return can_ll_get_rif();
}

//reg :errint

void can_hal_set_beif(uint32_t v)
{
	can_ll_set_beif(v);
}

uint32_t can_hal_beif(void)
{
	return can_ll_beif();
}

void can_hal_set_beie(uint32_t v)
{
	can_ll_set_beie(v);
}

uint32_t can_hal_get_beie(void)
{
	return can_ll_get_beie();
}

void can_hal_set_alif(uint32_t v)
{
	can_ll_set_alif(v);
}

uint32_t can_hal_get_alif(void)
{
	return can_ll_get_alif();
}

void can_hal_set_alie(uint32_t v)
{
	can_ll_set_alie(v);
}

uint32_t can_hal_get_alie(void)
{
	return can_ll_get_alie();
}

void can_hal_set_epif(uint32_t v)
{
	can_ll_set_epif(v);
}

uint32_t can_hal_get_epif(void)
{
	return can_ll_get_epif();
}

void can_hal_set_epie(uint32_t v)
{
	can_ll_set_epie(v);
}

uint32_t can_hal_get_epie(void)
{
	return can_ll_get_epie();
}

uint32_t can_hal_get_epass (void)
{
	return can_ll_get_epass();
}

uint32_t can_hal_get_ewarn(void)
{
	return can_ll_get_ewarn();
}

//reg :limit

void can_hal_set_ewl(uint32_t v)
{
	can_ll_set_ewl(v);
}

uint32_t can_hal_get_ewl(void)
{
	return can_ll_get_ewl();
}

void can_hal_set_afwl(uint32_t v)
{
	can_ll_set_afwl(v);
}

uint32_t can_hal_get_afwl(void)
{
	return can_ll_get_afwl();
}

//reg :s_seg_1

void can_hal_set_sseg_value(uint32_t v)
{
	can_ll_set_sseg_value(v);
}

uint32_t can_hal_get_sseg_value(void)
{
	return can_ll_get_sseg_value();
}

void can_hal_set_sseg1(uint32_t v)
{
	can_ll_set_sseg1(v);
}

uint32_t can_hal_get_sseg1(void)
{
	return can_ll_get_sseg1();
}

//reg :s_seg_2

void can_hal_set_sseg2(uint32_t v)
{
	can_ll_set_sseg2(v);
}

uint32_t can_hal_get_sseg2(void)
{
	return can_ll_get_sseg2();
}

void can_hal_set_ssjw(uint32_t v)
{
	can_ll_set_ssjw(v);
}

uint32_t can_hal_get_ssjw(void)
{
	return can_ll_get_ssjw();
}

void can_hal_set_spresc(uint32_t v)
{
	can_ll_set_spresc(v);
}

uint32_t can_hal_get_spresc(void)
{
	return can_ll_get_spresc();
}

//reg :f_seg_2

void can_hal_set_fseg_value(uint32_t v)
{
	can_ll_set_fseg_value(v);
}

uint32_t can_hal_get_fseg_value(void)
{
	return can_ll_get_fseg_value();
}

void can_hal_set_fseg1(uint32_t v)
{
	can_ll_set_fseg1(v);
}

uint32_t can_hal_get_fseg1(void)
{
	return can_ll_get_fseg1();
}
//reg :f_seg_2

void can_hal_set_fseg2(uint32_t v)
{
	can_ll_set_fseg2(v);
}

uint32_t can_hal_get_fseg2(void)
{
	return can_ll_get_fseg2();
}

//reg :f_sjw

void can_hal_set_fsjw(uint32_t v)
{
	can_ll_set_fsjw(v);
}

uint32_t can_hal_get_fsjw(void)
{
	return can_ll_get_fsjw();
}

//reg :f_presc

void can_hal_set_fpresc(uint32_t v)
{
	can_ll_set_fpresc(v);
}

uint32_t can_hal_get_fpresc(void)
{
	return can_ll_get_fpresc();
}

//reg :ealcap

void can_hal_set_cap_value(uint32_t v)
{
	can_ll_set_cap_value(v);
}

uint32_t can_hal_get_cap_value(void)
{
	return can_ll_get_cap_value();
}

uint32_t can_hal_get_alc(void)
{
	return can_ll_get_alc();
}

uint32_t can_hal_get_koer(void)
{
	return can_ll_get_koer();
}

//reg :tdc

void can_hal_set_sspoff(uint32_t v)
{
	can_ll_set_sspoff(v);
}

uint32_t can_hal_get_sspoff(void)
{
	return can_ll_get_sspoff();
}

void can_hal_set_tdcen(uint32_t v)
{
	can_ll_set_tdcen(v);
}

uint32_t can_hal_get_tdcen(void)
{
	return can_ll_get_tdcen();
}

//reg :recnt

uint32_t can_hal_get_recnt(void)
{
	return can_ll_get_recnt();
}

//reg :

uint32_t can_hal_get_tecnt(void)
{
	return can_ll_get_tecnt();
}

//reg :acfctrl

void can_hal_set_acf_value(uint32_t v)
{
	can_ll_set_acf_value(v);
}

uint32_t can_hal_get_acf_value(void)
{
	return can_ll_get_acf_value();
}

void can_hal_set_acfadr(uint32_t v)
{
	can_ll_set_acfadr(v);
}

uint32_t can_hal_get_acfadr(void)
{
	return can_ll_get_acfadr();
}

void can_hal_set_selmask(uint32_t v)
{
	can_ll_set_selmask(v);
}

uint32_t can_hal_get_selmask(void)
{
	return can_ll_get_selmask();
}

//reg :timecfg

void can_hal_set_timeen(uint32_t v)
{
	can_ll_set_timeen(v);
}

uint32_t can_hal_get_timeen(void)
{
	return can_ll_get_timeen();
}

void can_hal_set_timepos(uint32_t v)
{
	can_ll_set_timepos(v);
}

uint32_t can_hal_get_timepos(void)
{
	return can_ll_get_timepos();
}

void can_hal_set_acf_en(uint32_t v)
{
	can_ll_set_acf_en(v);
}

uint32_t can_hal_get_acf_en(void)
{
	return can_ll_get_acf_en();
}

//reg :acode_x or amask_x 

void can_hal_set_aid_value(uint32_t v)
{
	can_ll_set_aid_value(v);
}

uint32_t can_hal_get_aid_value(void)
{
	return can_ll_get_aid_value();
}

void can_hal_set_acode(uint32_t v)
{
	can_hal_set_selmask(0);
	can_ll_set_acode_or_amask(v);
}

uint32_t can_hal_get_acode(void)
{
	can_hal_set_selmask(0);
	return can_ll_get_acode_or_amask();
}

void can_hal_set_amask(uint32_t v)
{
	can_hal_set_selmask(1);
	can_ll_set_acode_or_amask(v);
}

uint32_t can_hal_get_amask(void)
{
	can_hal_set_selmask(1);
	return can_ll_get_acode_or_amask();
}

//reg :

void can_hal_set_aide(uint32_t v)
{
	can_ll_set_aide(v);
}

uint32_t can_hal_get_aide(void)
{
	return can_ll_get_aide();
}

void can_hal_set_aidee(uint32_t v)
{
	can_ll_set_aidee(v);
}

uint32_t can_hal_get_aidee(void)
{
	return can_ll_get_aidee();
}

//reg :ver_0

void can_hal_set_ttcfg_value(uint32_t v)
{
	can_ll_set_ttcfg_value(v);
}

uint32_t can_hal_get_ttcfg_value(void)
{
	return can_ll_get_ttcfg_value();
}

uint32_t can_hal_get_ver_0(void)
{
	return can_ll_get_ver_0();
}

//reg :ver_1

uint32_t can_hal_get_ver_1(void)
{
	return can_ll_get_ver_1();
}

//reg :tbslot

void can_hal_set_tbptr(uint32_t v)
{
	can_ll_set_tbptr(v);
}

uint32_t can_hal_get_tbptr(void)
{
	return can_ll_get_tbptr();
}

void can_hal_set_tbf(uint32_t v)
{
	can_ll_set_tbf(v);
}

uint32_t can_hal_get_tbf(void)
{
	return can_ll_get_tbf();
}

void can_hal_set_tbe(uint32_t v)
{
	can_ll_set_tbe(v);
}

uint32_t can_hal_get_tbe(void)
{
	return can_ll_get_tbe();
}

//reg :ttcfg

void can_hal_set_ttcfg_tten(uint32_t v)
{
	can_ll_set_ttcfg_tten(v);
}

uint32_t can_hal_get_ttcfg_tten(void)
{
	return can_ll_get_ttcfg_tten();
}

void can_hal_set_ttcfg_t_presc(uint32_t v)
{
	can_ll_set_ttcfg_t_presc(v);
}

uint32_t can_hal_get_ttcfg_t_presc(void)
{
	return can_ll_get_ttcfg_t_presc();
}

void can_hal_set_ttcfg_ttif(uint32_t v)
{
	can_ll_set_ttcfg_ttif(v);
}

uint32_t can_hal_get_ttcfg_ttif(void)
{
	return can_ll_get_ttcfg_ttif();
}

void can_hal_set_ttcfg_ttie(uint32_t v)
{
	can_ll_set_ttcfg_ttie(v);
}

uint32_t can_hal_get_ttcfg_ttie(void)
{
	return can_ll_get_ttcfg_ttie();
}

void can_hal_set_ttcfg_teif(uint32_t v)
{
	can_ll_set_ttcfg_teif(v);
}

uint32_t can_hal_get_ttcfg_teif(void)
{
	return can_ll_get_ttcfg_teif();
}

void can_hal_set_ttcfg_wtif(uint32_t v)
{
	can_ll_set_ttcfg_wtif(v);
}

uint32_t can_hal_get_ttcfg_wtif(void)
{
	return can_ll_get_ttcfg_wtif();
}

void can_hal_set_ttcfg_wtie(uint32_t v)
{
	can_ll_set_ttcfg_wtie(v);
}

uint32_t can_hal_get_ttcfg_wtie(void)
{
	return can_ll_get_ttcfg_wtie();
}

//reg :ref_msg

void can_hal_set_ref_msg_value(uint32_t v)
{
	can_ll_set_ref_msg_value(v);
}

uint32_t can_hal_get_ref_msg_value(void)
{
	return can_ll_get_ref_msg_value();
}

void can_hal_set_ref_msg_id(uint32_t v)
{
	can_ll_set_ref_msg_id(v);
}

uint32_t can_hal_get_ref_msg_id(void)
{
	return can_ll_get_ref_msg_id();
}

void can_hal_set_ref_msg_ide(uint32_t v)
{
	can_ll_set_ref_msg_ide(v);
}

uint32_t can_hal_get_ref_msg_ide(void)
{
	return can_ll_get_ref_msg_ide();
}

//reg :trig_cfg0

void can_hal_set_trig_cfg_value(uint32_t v)
{
	can_ll_set_trig_cfg_value(v);
}

uint32_t can_hal_get_trig_cfg_value(void)
{
	return can_ll_get_trig_cfg_value();
}

void can_hal_set_ttptr(uint32_t v)
{
	can_ll_set_ttptr(v);
}

uint32_t can_hal_get__ttptr(void)
{
	return can_ll_get__ttptr();
}

//reg :

void can_hal_set_ttype(uint32_t v)
{
	can_ll_set_ttype(v);
}

uint32_t can_hal_get_ttype(void)
{
	return can_ll_get_ttype();
}

void can_hal_set_tew(uint32_t v)
{
	can_ll_set_tew(v);
}

uint32_t can_hal_get_tew(void)
{
	return can_ll_get_tew();
}

//reg :

void can_hal_set_tt_trig(uint32_t v)
{
	can_ll_set_tt_trig(v);
}

uint32_t can_hal_get_tt_trig (void)
{
	return can_ll_get_tt_trig();
}


//reg : mem_port

void can_hal_set_mem_stat_value(uint32_t v)
{
	can_ll_set_mem_stat_value(v);
}

uint32_t can_hal_get_mem_stat_value(void)
{
	return can_ll_get_mem_stat_value();
}

void can_hal_set_tt_wtrig(uint32_t v)
{
	can_ll_set_tt_wtrig(v);
}

uint32_t can_hal_get_tt_wtrig(void)
{
	return can_ll_get_tt_wtrig();
}

void can_hal_set_mpen(uint32_t v)
{
	can_ll_set_mpen(v);
}

uint32_t can_hal_get_mpen(void)
{
	return can_ll_get_mpen();
}

void can_hal_set_mdwie(uint32_t v)
{
	can_ll_set_mdwie(v);
}

uint32_t can_hal_get_mdwie(void)
{
	return can_ll_get_mdwie();
}

void can_hal_set_mdwif(uint32_t v)
{
	can_ll_set_mdwif(v);
}

uint32_t can_hal_get_mdwif(void)
{
	return can_ll_get_mdwif();
}

void can_hal_set_mdeif(uint32_t v)
{
	can_ll_set_mdeif(v);
}

uint32_t can_hal_get_mdeif(void)
{
	return can_ll_get_mdeif();
}

void can_hal_set_maeif (uint32_t v)
{
	can_ll_set_maeif(v);
}

uint32_t can_hal_get_maeif(void)
{
	return can_ll_get_maeif();
}

void can_hal_set_acfa (uint32_t v)
{
	can_ll_set_acfa(v);
}

uint32_t can_hal_get_acfa(void)
{
	return can_ll_get_acfa();
}

uint32_t can_hal_get_txs(void)
{
	return can_ll_get_txs();
}

uint32_t can_hal_get_txb(void)
{
	return can_ll_get_txb();
}

uint32_t can_hal_get_heloc(void)
{
	return can_ll_get_heloc();
}

//reg :mem_es0

void can_hal_set_mem_es_value(uint32_t v)
{
	can_ll_set_mem_es_value(v);
}

uint32_t can_hal_get_mem_es_value(void)
{
	return can_ll_get_mem_es_value();
}

void can_hal_set_mebp1 (uint32_t v)
{
	can_ll_set_mebp1(v);
}

uint32_t can_hal_get_mebp1 (void)
{
	return can_ll_get_mebp1();
}

void can_hal_set_me1ee (uint32_t v)
{
	can_ll_set_me1ee(v);
}

uint32_t can_hal_get_me1ee(void)
{
	return can_ll_get_me1ee();
}

void can_hal_set_meaee(uint32_t v)
{
	can_ll_set_meaee(v);
}

uint32_t can_hal_get_meaee(void)
{
	return can_ll_get_meaee();
}

//reg :

void can_hal_set_mebp2(uint32_t v)
{
	can_ll_set_mebp2(v);
}

uint32_t can_hal_get_mebp2(void)
{
	return can_ll_get_mebp2();
}

void can_hal_set_me2ee(uint32_t v)
{
	can_ll_set_me2ee(v);
}

uint32_t can_hal_get_me2ee(void)
{
	return can_ll_get_me2ee();
}

//reg :mem_es1

void can_hal_set_meeec(uint32_t v)
{
	can_ll_set_meeec(v);
}

uint32_t can_hal_get_meeec(void)
{
	return can_ll_get_meeec();
}

void can_hal_set_menec(uint32_t v)
{
	can_ll_set_menec(v);
}

uint32_t can_hal_get_menec(void)
{
	return can_ll_get_menec();
}

//reg :mem_es3

void can_hal_set_mel(uint32_t v)
{
	can_ll_set_mel(v);
}

uint32_t can_hal_get_mel(void)
{
	return can_ll_get_mel();
}

void can_hal_set_mes(uint32_t v)
{
	can_ll_set_mes(v);
}

uint32_t can_hal_get_mes(void)
{
	return can_ll_get_mes();
}

//reg :scfg

void can_hal_set_scfg_value(uint32_t v)
{
	can_ll_set_scfg_value(v);
}

uint32_t can_hal_get_scfg_value(void)
{
	return can_ll_get_scfg_value();
}

void can_hal_set_xmren(uint32_t v)
{
	can_ll_set_xmren(v);
}

uint32_t can_hal_get_xmren(void)
{
	return can_ll_get_xmren();
}

void can_hal_set_seif(uint32_t v)
{
	can_ll_set_seif(v);
}

uint32_t can_hal_get_seif(void)
{
	return can_ll_get_seif();
}

void can_hal_set_swie (uint32_t v)
{
	can_ll_set_swie(v);
}

uint32_t can_hal_get_swie (void)
{
	return can_ll_get_swie();
}

void can_hal_set_swif(uint32_t v)
{
	can_ll_set_swif(v);
}

uint32_t can_hal_get_swif(void)
{
	return can_ll_get_swif();
}

void can_hal_set_fstim(uint32_t v)
{
	can_ll_set_fstim(v);
}

uint32_t can_hal_get_fstim(void)
{
	return can_ll_get_fstim();
}

//reg :can_fd

void can_hal_set_fd_enable(uint32_t v)
{
	can_ll_set_fd_enable(v);
}

uint32_t can_hal_get_fd_enable(void)
{
	return can_ll_get_fd_enable();
}

static uint32_t can_hal_get_br_reg_val(can_bit_rate_e br)
{
	uint32_t i;

	for (i = 0; i < ARRAY_SIZE(can_br_tab); i++) {
		if (br == can_br_tab[i].br) {
			return can_br_tab[i].reg_val;
		}
	}

	return 0;
}

void can_hal_bit_rate_config(can_bit_rate_e s_speed, can_bit_rate_e f_speed)
{
	uint32_t value;

	value = can_hal_get_br_reg_val(s_speed);

	can_hal_set_sseg_value(value);

	value = can_hal_get_br_reg_val(f_speed);

	can_ll_set_fseg_value(value);
}

static void can_hal_mem_protect_set(void)
{
	uint32_t value = 0;

	//no protection
	value = can_hal_get_mem_stat_value();
	value &= ~(0xFF << CAN_MEM_MPEN_POS);
	can_hal_set_mem_stat_value(value);

}

static bk_err_t can_hw_init(can_speed_t *can_speed)
{
	BK_RETURN_ON_NULL(can_speed);

	can_hal_set_reset(1);

	can_hal_bit_rate_config(can_speed->s_speed, can_speed->f_speed);

	can_hal_mem_protect_set();

	can_hal_set_afwl(MAX_ACF_NUM & CAN_IE_AFWL_MASK);

	can_hal_set_fd_enable(1);

	can_hal_set_reset(0);
	return BK_OK;
}

static void can_hal_acc_filter_switch_mask(void)
{
	can_hal_set_selmask(1);
}

static void can_hal_acc_filter_switch_code(void)
{
	can_hal_set_selmask(0);
}

//effect when RESET=1
static void can_hal_accept_filter_set(can_acc_filter_cmd_s *cmd)
{
	uint8_t acfadr = cmd->seq & 0x0F;
	uint32_t value = 0;

	if (cmd->onoff == DRIVER_DISABLE) {
		value = can_hal_get_acf_en();
		value &= ~(1 << acfadr);
		can_hal_set_acf_en(value);
	} else {
		//choose the addr of acc filter
		can_hal_set_acfadr(acfadr);
		HAL_LOGI("acfadr 0x%x\r\n", acfadr);

		switch (cmd->aide) {
			case CAN_ACCEPT_BOTH:
				can_hal_set_aidee(0);
				HAL_LOGI("CAN_ACCEPT_BOTH 0x%x\r\n", value);
				break;

			case CAN_ACCEPT_STANDARD:
				can_hal_set_aidee(1);
				can_hal_set_aide(0);
				break;

			case CAN_ACCEPT_EXTENDED:
				can_hal_set_aidee(1);
				can_hal_set_aide(1);
				break;
		}
		//first set mask
		can_hal_set_amask(cmd->mask & CAN_AID_ACODE_OR_AMASK_MASK);
		cmd->mask = can_hal_get_amask();
		HAL_LOGI("amask 0x%x\r\n", cmd->mask);

		//second set code
		can_hal_set_acode(cmd->code & CAN_AID_ACODE_OR_AMASK_MASK);
		cmd->code = can_hal_get_acode();
		HAL_LOGI("acode 0x%x\r\n", cmd->code);

		//ACF_EN
		value = can_hal_get_acf_en();
		value |= (1 << acfadr);
		can_hal_set_acf_en(value);
		HAL_LOGI("ACF_EN 0x%x\r\n", value);
	}
}

static void can_hal_frame_tag_copy(can_frame_tag_t* des, can_frame_tag_t* src)
{
	os_memcpy(des, src, sizeof(can_frame_tag_t));
}

static uint8_t can_hal_dlc_to_len(uint8_t fdf, uint8_t dlc)
{
	uint8_t len = 0;

	if (dlc <= CAN_20_MAX_PAYLOAD) {
		len = dlc;
	} else {
		if (fdf == FDF_CAN_20) {
			len = CAN_20_MAX_PAYLOAD;
		} else if (fdf == FDF_CAN_FD) {
			switch (dlc) {
				case 0x9:
					len = 12;
					break;
				case 0xa:
					len = 16;
					break;
				case 0xb:
					len = 20;
					break;
				case 0xc:
					len = 24;
					break;
				case 0xd:
					len = 32;
					break;
				case 0xe:
					len = 48;
					break;
				case 0xf:
					len = 64;
					break;
			}
		}
	}

	return len;
}

static uint8_t can_hal_frame_len_to_dlc(uint8_t fdf, uint32_t size)
{
	uint8_t dlc = 0;

	if (size <= CAN_20_MAX_PAYLOAD) {
		dlc = size;
	} else {
		if (fdf == FDF_CAN_20) {
			dlc = CAN_20_MAX_PAYLOAD;
		} else if (fdf == FDF_CAN_FD) {
			if (FDF_CAN_20 < size && size < 12) {
				dlc = CAN_20_MAX_PAYLOAD;
			} else if (12 <= size && size < 16) {
				dlc = 0x9;
			} else if (16 <= size && size < 20) {
				dlc = 0xa;
			} else if(20 <= size && size < 24) {
				dlc = 0xb;
			} else if (24 <= size && size < 32) {
				dlc = 0xc;
			} else if (32 <= size && size < 48) {
				dlc = 0xd;
			} else if (48 <= size && size < CAN_FD_MAX_PAYLOAD) {
				dlc = 0xe;
			} else if(CAN_FD_MAX_PAYLOAD <= size) {
				dlc = 0xf;
			}
		}
	}

	return dlc;
}

static void can_hal_trans_switch(uint32_t seq)
{
	uint32_t value = 0;

	value = can_hal_get_cfg_value();
	if (seq & CAN_TPE) {
		value |= 1 << CAN_CFG_TPE_POS;
	}

	if (seq & CAN_TPA) {
		value |= 1 << CAN_CFG_TPA_POS;
	}

	if (seq & CAN_TSONE) {
		value |= 1 << CAN_CFG_TSONE_POS;
	}

	if (seq & CAN_TSALL) {
		value |= 1 << CAN_CFG_TSALL_POS;
	}

	if (seq & CAN_TSA) {
		value |= 1 << CAN_CFG_TSA_POS;
	}

	can_hal_set_cfg_value(value);
}

static int can_hal_send_ptb(can_frame_s *frame)
{
	uint32_t value = 0;
	HAL_LOGI(" %s, %d \r\n\r\n", __func__, __LINE__);

	can_hal_set_tbsel(0);

	value |= ((can_hal_frame_len_to_dlc(frame->tag.fdf, frame->size)) & CAN_TBUF_CTRL_DLC_MASK) << CAN_TBUF_CTRL_DLC_POS;
	value |= ((frame->tag.brs) & CAN_TBUF_CTRL_BRS_MASK) << CAN_TBUF_CTRL_BRS_POS;
	value |= ((frame->tag.fdf) & CAN_TBUF_CTRL_FDF_MASK) << CAN_TBUF_CTRL_FDF_POS;
	value |= ((frame->tag.rtr) & CAN_TBUF_CTRL_RTR_MASK) << CAN_TBUF_CTRL_RTR_POS;
	value |= ((frame->tag.ide) & CAN_TBUF_CTRL_IDE_MASK) << CAN_TBUF_CTRL_IDE_POS;
	can_hal_set_tbuf_ctrl_value(value);

	value = frame->tag.id;
	can_hal_set_tid(value);

	uint8_t reg_x = 0, i = 0;
	uint8_t *p_o = (uint8_t*)&value;
	uint8_t *p_i = (uint8_t*)frame->data;

	while (frame->size) {
		*(p_o + i) = *(p_i++);
		frame->size--;
		i++;
		if (i == 4 || frame->size == 0) {
			can_hal_set_tdata(reg_x, value);
			reg_x++;
			i = 0;
		}
	}
	return BK_OK;
}

static bk_err_t can_hal_send_stb(void)
{
	uint32_t data_size = 0, id = 0, value = 0, loop_prt = 0;
	uint8_t ide = 0, rtr = 0, fdf = 0, brs = 0, ttsen = 0, dlc = 0;

	if (s_can_hal_env.tx_fifo_get == NULL) {
		return BK_ERR_NULL_PARAM;
	}

	can_hal_set_tbsel(1);

	while ((data_size = s_can_hal_env.tx_size())) {
		ide = s_can_hal_env.tx_frame_tag.ide;
		id = s_can_hal_env.tx_frame_tag.id;
		rtr = s_can_hal_env.tx_frame_tag.rtr;
		fdf = s_can_hal_env.tx_frame_tag.fdf;
		brs = s_can_hal_env.tx_frame_tag.brs;
		ttsen = s_can_hal_env.tx_frame_tag.ttsen;

		dlc = can_hal_frame_len_to_dlc(fdf, data_size);
		data_size = can_hal_dlc_to_len(fdf, dlc);

		value = (id & (ide?CAN_EXT_ID_MASK:CAN_STAND_ID_MASK)) | ((ttsen & TTSEN_MASK) << POS_TTSEN);
		can_hal_set_tid(value);

		value = ((dlc & CAN_TBUF_CTRL_DLC_MASK) << CAN_TBUF_CTRL_DLC_POS) | \
				((brs & CAN_TBUF_CTRL_BRS_MASK) << CAN_TBUF_CTRL_BRS_POS) | \
				((fdf & CAN_TBUF_CTRL_FDF_MASK) << CAN_TBUF_CTRL_FDF_POS) | \
				((rtr & CAN_TBUF_CTRL_RTR_MASK) << CAN_TBUF_CTRL_RTR_POS) | \
				((ide & CAN_TBUF_CTRL_IDE_MASK) << CAN_TBUF_CTRL_IDE_POS);
		can_hal_set_tbuf_ctrl_value(value);

		uint8_t reg_x = 0, size = 0;
		value = 0;
		while (data_size && reg_x < BUF_REG_NUM) {
			size = s_can_hal_env.tx_fifo_get((uint8_t*)&value, 4);
			data_size -= size;
			can_hal_set_tdata(reg_x++, value);
			value = 0;
		}

		can_hal_set_tsnext(1);

		loop_prt = IDLE_CNT;
		while (1) {
			if (can_hal_get_tsnext() == 0) {
				break;
			}
			LOOP_PRT(loop_prt);

			can_hal_trans_switch(CAN_TSONE);
		}
	}
	HAL_LOGI("ide 0x%x, id0x%x, rtr0x%x, fdf0x%x, brs0x%x, ttsen0x%x, dlc0x%x, data_size0x%x\r\n",ide, id, rtr, fdf, brs, ttsen, dlc, data_size);
	return BK_OK;
}

bk_err_t can_hal_ctrl(uint32_t cmd, void *parm)
{
	int ret = 0;

	switch (cmd) {
		case CMD_CAN_MODUILE_INIT:
			if (parm) {
				ret = can_hw_init((can_speed_t *)parm);
			} else {
				return BK_ERR_NULL_PARAM;
			}
			break;
		case CMD_CAN_SET_RX_CALLBACK:
			if (parm) {
				can_callback_des_t *p;
				p = (can_callback_des_t *)parm;
				s_can_hal_env.rx_cb.cb = p->cb;
				s_can_hal_env.rx_cb.param = p->param;
			} else {
				s_can_hal_env.rx_cb.cb = NULL;
				s_can_hal_env.rx_cb.param = NULL;
			}
			break;
		case CMD_CAN_SET_TX_CALLBACK:
			if (parm) {
				can_callback_des_t *p;
				p = (can_callback_des_t *)parm;
				s_can_hal_env.tx_cb.cb = p->cb;
				s_can_hal_env.tx_cb.param = p->param;
			} else {
				s_can_hal_env.tx_cb.cb = NULL;
				s_can_hal_env.tx_cb.param = NULL;
			}
			break;
		case CMD_CAN_SET_ERR_CALLBACK:
			if (parm) {
				can_callback_des_t *p;
				p = (can_callback_des_t *)parm;
				s_can_hal_env.err_cb.cb = p->cb;
				s_can_hal_env.err_cb.param = p->param;
			} else {
				s_can_hal_env.err_cb.cb = NULL;
				s_can_hal_env.err_cb.param = NULL;
			}
			break;
		case CMD_CAN_RESET_REQ:
			switch ((can_loc_reset_c)parm) {
				case CAN_RESET_REQ_EN:
					can_hal_set_reset(1);
					break;
				case CAN_RESET_REQ_NO:
					can_hal_set_reset(0);
					break;
			}
			break;
		case CMD_CAN_SET_TX_FIFO:
			if (parm) {
				can_fifo_f f;
				f = (can_fifo_f)parm;
				s_can_hal_env.tx_fifo_get = f;
			} else {
				s_can_hal_env.tx_fifo_get = NULL;
			}
			break;
		case CMD_CAN_SET_RX_FIFO:
			if (parm) {
				can_fifo_f f;
				f = (can_fifo_f)parm;
				s_can_hal_env.rx_fifo_put = f;
			} else {
				s_can_hal_env.rx_fifo_put = NULL;
			}
			break;
		case CMD_CAN_GET_TX_SIZE:
			if (parm) { 
				s_can_hal_env.tx_size = (can_size_f)parm;
			} else {
				s_can_hal_env.tx_size = NULL;
			}
			break;
		case CMD_CAN_ACC_FILTER_SET:
			if (parm) {
				can_hal_accept_filter_set((can_acc_filter_cmd_s*)parm);
			} else {
				return BK_ERR_NULL_PARAM;
			}
			break;
		case CMD_CAN_FD_BOSCH_MODE:
			if (parm) {
				can_hal_set_fd_iso(*(uint32_t*)parm);
			} else {
				return BK_ERR_NULL_PARAM;
			}
			break;
		case CMD_CAN_GET_RX_FRAME_TAG:
			can_hal_frame_tag_copy((can_frame_tag_t*)parm, &(s_can_hal_env.rx_frame_tag));
			break;
		case CMD_CAN_SET_TX_FRAME_TAG:
			can_hal_frame_tag_copy(&(s_can_hal_env.tx_frame_tag), (can_frame_tag_t*)parm);
			break;
		case CMD_CAN_PTB_INBUF:
			if (parm) {
				can_hal_send_ptb((can_frame_s*)parm);
			} else {
				return BK_ERR_PARAM;
			}
			break;
		case CMD_CAN_TRANS_SWITCH:
			can_hal_trans_switch((uint32_t)parm);
			break;
		case CMD_CAN_STB_INBUF:
			ret = can_hal_send_stb();
			break;
		case CMD_CAN_BUSOFF_CLR:
			can_hal_set_busoff(1);
			break;
		case CMD_CAN_GET_KOER:
			*(can_koer_code_e*)parm = can_hal_get_koer();
			break;
		default:
			HAL_LOGE("unknow cmd!!!");
			return BK_ERR_PARAM;
	}
	return ret;
}

void can_hal_error_analysis(uint32_t err_c)
{
    if (s_can_hal_env.err_cb.cb) {
        s_can_hal_env.err_cb.cb((void *)err_c);
    }
}

void can_hal_receive_frame(void)
{
    uint32_t value;
    uint32_t id;
    uint8_t ide, rtr, fdf, brs, dlc, esi, len;

    if (s_can_hal_env.rx_fifo_put == NULL) {
        return;
    }

    while (can_hal_get_rstat() != RSTAT_EMPTY) {
		value = can_hal_get_rbuf_ctrl_value();

        ide = (value >> CAN_RBUF_CTRL_IDE_POS) & CAN_RBUF_CTRL_IDE_MASK;
        dlc = (value >> CAN_RBUF_CTRL_DLC_POS) & CAN_RBUF_CTRL_DLC_MASK;
        brs = (value >> CAN_RBUF_CTRL_BRS_POS) & CAN_RBUF_CTRL_BRS_MASK;
        fdf = (value >> CAN_RBUF_CTRL_FDF_POS) & CAN_RBUF_CTRL_FDF_MASK;
        rtr = (value >> CAN_RBUF_CTRL_RTR_POS) & CAN_RBUF_CTRL_RTR_MASK;

        s_can_hal_env.rx_fifo_put((uint8_t*)&dlc, 1);

		value = can_hal_get_rid();

        id = (value) & (ide?CAN_EXT_ID_MASK:CAN_STAND_ID_MASK);

		esi = can_hal_get_rid_esi();

        len = can_hal_dlc_to_len(fdf, dlc);

        s_can_hal_env.rx_fifo_put((uint8_t*)&id, 4);

        s_can_hal_env.rx_frame_tag.id = id;
        s_can_hal_env.rx_frame_tag.ide = ide;
        s_can_hal_env.rx_frame_tag.rtr = rtr;
        s_can_hal_env.rx_frame_tag.fdf = fdf;
        s_can_hal_env.rx_frame_tag.brs = brs;
        s_can_hal_env.rx_frame_tag.esi = esi;

        uint32_t s_size, i=0;
        while (len) {
            if (len > 4) {
                s_size = 4;
            } else {
                s_size = len;
            }

            value = can_hal_get_rdata_value(i);
            i++;
            s_can_hal_env.rx_fifo_put((uint8_t*)&value, s_size);
            len -= s_size;
        }

		can_hal_set_rrel(1);
    }

	if (s_can_hal_env.rx_cb.cb) {
		void* param = s_can_hal_env.rx_cb.param;
		s_can_hal_env.rx_cb.cb(param);
	}
}

void can_hal_send_frame(void)
{
	if (s_can_hal_env.tx_cb.cb) {
		void* param = s_can_hal_env.tx_cb.param;
		s_can_hal_env.tx_cb.cb(param);
	}
}

#ifdef __cplusplus
}
#endif