// Copyright 2020-2021 Beken
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

#pragma once

#include "hal_config.h"
#include "can_hw.h"
#include "can_ll.h"
#include <driver/hal/hal_can_types.h>

#ifdef __cplusplus
extern "C" {
#endif

uint32_t can_hal_get_rid_esi_value(void);

uint32_t can_hal_get_rid(void);

uint32_t can_hal_get_rid_esi(void);

//reg :rbuf_ctrl

uint32_t can_hal_get_rbuf_ctrl_value(void);

uint32_t can_hal_get_rbuf_ctrl_dlc(void);

uint32_t can_hal_get_rbuf_ctrl_brs(void);

uint32_t can_hal_get_rbuf_ctrl_fdf(void);

uint32_t can_hal_get_rbuf_ctrl_rtr(void);

uint32_t can_hal_get_rbuf_ctrl_ide(void);
//reg :rdata

uint32_t can_hal_get_rdata_value(uint32_t i);

//reg :tid

void can_hal_set_tid_esi_value(uint32_t v);

uint32_t can_hal_get_tid_esi_value(void);

void can_hal_set_tid(uint32_t v);

uint32_t can_hal_get_tid(void);

void can_hal_set_tid_esi(uint32_t v);

uint32_t can_hal_get_tid_esi(void);

//reg :tbuf_ctrl

void can_hal_set_tbuf_ctrl_value(uint32_t v);

uint32_t can_hal_get_tbuf_ctrl_value(void);

void can_hal_set_tbuf_ctrl_dlc(uint32_t v);

uint32_t can_hal_get_tbuf_ctrl_dlc(void);

void can_hal_set_tbuf_ctrl_brs(uint32_t v);

uint32_t can_hal_get_tbuf_ctrl_brs(void);

void can_hal_set_tbuf_ctrl_fdf(uint32_t v);

uint32_t can_hal_get_tbuf_ctrl_fdf(void);

void can_hal_set_tbuf_ctrl_rtr(uint32_t v);

uint32_t can_hal_get_tbuf_ctrl_rtr(void);

void can_hal_set_tbuf_ctrl_ide(uint32_t v);

uint32_t can_hal_get_tbuf_ctrl_ide(void);

//reg :tdata

void can_hal_set_tdata(uint32_t i, uint32_t v);

uint32_t can_hal_get_tdata(uint32_t i);

//reg :tts

uint32_t can_hal_get_tts_l_value(void);

//reg :

uint32_t can_hal_get_tts_h_value(void);

//reg :cfg_stat

void can_hal_set_cfg_value(uint32_t v);

uint32_t can_hal_get_cfg_value(void);

void can_hal_set_busoff(uint32_t v);

uint32_t can_hal_get_busoff(void);

uint32_t can_hal_get_tactive(void);

uint32_t can_hal_get_ractive(void);

void can_hal_set_tsss(uint32_t v);

uint32_t can_hal_get_tsss(void);

void can_hal_set_tpss(uint32_t v);

uint32_t can_hal_get_tpss(void);

void can_hal_set_lbmi(uint32_t v);

uint32_t can_hal_get_lbmi(void);

void can_hal_set_lbme(uint32_t v);

uint32_t can_hal_get_lbme(void);

void can_hal_set_reset(uint32_t v);

uint32_t can_hal_get_reset(void);

//reg :tcmd

void can_hal_set_tsa(uint32_t v);

uint32_t can_hal_get_tsa(void);

void can_hal_set_tsall(uint32_t v);

uint32_t can_hal_get_tsall(void);

void can_hal_set_tsone(uint32_t v);

uint32_t can_hal_get_tsone(void);

void can_hal_set_tpa(uint32_t v);

uint32_t can_hal_get_tpa(void);

void can_hal_set_tpe(uint32_t v);

uint32_t can_hal_get_tpe(void);

void can_hal_set_stby(uint32_t v);

uint32_t can_hal_get_stby(void);

void can_hal_set_lom(uint32_t v);

uint32_t can_hal_get_lom(void);

void can_hal_set_tbsel(uint32_t v);

uint32_t can_hal_get_tbsel(void);

//reg :tctrl

uint32_t can_hal_get_tsstat(void);

void can_hal_set_tttbm(uint32_t v);

uint32_t can_hal_get_tttbm(void);

void can_hal_set_tsmode(uint32_t v);

uint32_t can_hal_get_tsmode(void);

void can_hal_set_tsnext(uint32_t v);

uint32_t can_hal_get_tsnext (void);

void can_hal_set_fd_iso(uint32_t v);

uint32_t can_hal_get_fd_iso(void);

//reg :rctrl

uint32_t can_hal_get_rstat(void);

void can_hal_set_rball(uint32_t v);

uint32_t can_hal_get_rball(void);

void can_hal_set_rrel(uint32_t v);

uint32_t can_hal_get_rrel(void);

uint32_t can_hal_get_rov(void);

void can_hal_set_rom(uint32_t v);

uint32_t can_hal_get_rom(void);

void can_hal_set_sack(uint32_t v);

uint32_t can_hal_get_sack(void);

//reg :trie

void can_hal_set_ie_value(uint32_t v);

uint32_t can_hal_get_ie_value(void);

uint32_t can_hal_get_tsff(void);

void can_hal_set_eie(uint32_t v);

uint32_t can_hal_get_eie(void);

void can_hal_set_tsie(uint32_t v);

uint32_t can_hal_get_tsie(void);

void can_hal_set_tpie(uint32_t v);

uint32_t can_hal_get_tpie(void);

void can_hal_set_rafie(uint32_t v);

uint32_t can_hal_get_rafie(void);

void can_hal_set_rfie(uint32_t v);

uint32_t can_hal_get_rfie(void);

void can_hal_set_roie(uint32_t v);

uint32_t can_hal_get_roie(void);

void can_hal_set_rie(uint32_t v);

uint32_t can_hal_get_rie(void);

//reg :trif

void can_hal_set_aif(uint32_t v);

uint32_t can_hal_get_aif(void);

void can_hal_set_eif(uint32_t v);

uint32_t can_hal_get_eif(void);

void can_hal_set_tsif(uint32_t v);

uint32_t can_hal_get_tsif(void);

void can_hal_set_tpif(uint32_t v);

uint32_t can_hal_get_tpif(void);

void can_hal_set_rafif(uint32_t v);

uint32_t can_hal_get_rafif (void);

void can_hal_set_rfif(uint32_t v);

uint32_t can_hal_get_rfif(void);

void can_hal_set_roif(uint32_t v);

uint32_t can_hal_get_roif(void);

void can_hal_set_rif(uint32_t v);

uint32_t can_hal_get_rif(void);

//reg :errint

void can_hal_set_beif(uint32_t v);

uint32_t can_hal_beif(void);

void can_hal_set_beie(uint32_t v);

uint32_t can_hal_get_beie(void);

void can_hal_set_alif(uint32_t v);

uint32_t can_hal_get_alif(void);

void can_hal_set_alie(uint32_t v);

uint32_t can_hal_get_alie(void);

void can_hal_set_epif(uint32_t v);

uint32_t can_hal_get_epif(void);

void can_hal_set_epie(uint32_t v);

uint32_t can_hal_get_epie(void);

uint32_t can_hal_get_epass (void);

uint32_t can_hal_get_ewarn(void);

//reg :limit

void can_hal_set_ewl(uint32_t v);

uint32_t can_hal_get_ewl(void);

void can_hal_set_afwl(uint32_t v);

uint32_t can_hal_get_afwl(void);

//reg :s_seg_1

void can_hal_set_sseg_value(uint32_t v);

uint32_t can_hal_get_sseg_value(void);

void can_hal_set_sseg1(uint32_t v);

uint32_t can_hal_get_sseg1(void);
//reg :s_seg_2

void can_hal_set_sseg2(uint32_t v);

uint32_t can_hal_get_sseg2(void);

void can_hal_set_ssjw(uint32_t v);

uint32_t can_hal_get_ssjw(void);

void can_hal_set_spresc(uint32_t v);

uint32_t can_hal_get_spresc(void);

//reg :f_seg_2

void can_hal_set_fseg_value(uint32_t v);

uint32_t can_hal_get_fseg_value(void);

void can_hal_set_fseg1(uint32_t v);

uint32_t can_hal_get_fseg1(void);
//reg :f_seg_2

void can_hal_set_fseg2(uint32_t v);

uint32_t can_hal_get_fseg2(void);

//reg :f_sjw

void can_hal_set_fsjw(uint32_t v);

uint32_t can_hal_get_fsjw(void);
//reg :f_presc

void can_hal_set_fpresc(uint32_t v);

uint32_t can_hal_get_fpresc(void);

//reg :ealcap

void can_hal_set_cap_value(uint32_t v);

uint32_t can_hal_get_cap_value(void);

uint32_t can_hal_get_alc(void);

uint32_t can_hal_get_koer(void);

//reg :tdc

void can_hal_set_sspoff(uint32_t v);

uint32_t can_hal_get_sspoff(void);

void can_hal_set_tdcen(uint32_t v);

uint32_t can_hal_get_tdcen(void);

//reg :recnt

uint32_t can_hal_get_recnt(void);

//reg :

uint32_t can_hal_get_tecnt(void);

//reg :acfctrl

void can_hal_set_acf_value(uint32_t v);

uint32_t can_hal_get_acf_value(void);

void can_hal_set_acfadr(uint32_t v);

uint32_t can_hal_get_acfadr(void);

void can_hal_set_selmask(uint32_t v);

uint32_t can_hal_get_selmask(void);
//reg :timecfg

void can_hal_set_timeen(uint32_t v);

uint32_t can_hal_get_timeen(void);

void can_hal_set_timepos(uint32_t v);

uint32_t can_hal_get_timepos(void);

void can_hal_set_acf_en(uint32_t v);

uint32_t can_hal_get_acf_en(void);

//reg :acf_en_2

// void can_hal_set_acf_en(uint32_t v);

// uint32_t can_hal_get_acf_en(void);
// //reg :acode_x or amask_x 

void can_hal_set_aid_value(uint32_t v);

uint32_t can_hal_get_aid_value(void);

void can_hal_set_acode(uint32_t v);

uint32_t can_hal_get_acode(void);

void can_hal_set_amask(uint32_t v);

uint32_t can_hal_get_amask(void);

//reg :

void can_hal_set_aide(uint32_t v);

uint32_t can_hal_get_aide(void);

void can_hal_set_aidee(uint32_t v);

uint32_t can_hal_get_aidee(void);

//reg :ver_0

void can_hal_set_ttcfg_value(uint32_t v);

uint32_t can_hal_get_ttcfg_value(void);

uint32_t can_hal_get_ver_0(void);

//reg :ver_1

uint32_t can_hal_get_ver_1(void);

//reg :tbslot

void can_hal_set_tbptr(uint32_t v);

uint32_t can_hal_get_tbptr(void);

void can_hal_set_tbf(uint32_t v);

uint32_t can_hal_get_tbf(void);

void can_hal_set_tbe(uint32_t v);

uint32_t can_hal_get_tbe(void);

//reg :ttcfg

void can_hal_set_ttcfg_tten(uint32_t v);

uint32_t can_hal_get_ttcfg_tten(void);

void can_hal_set_ttcfg_t_presc(uint32_t v);

uint32_t can_hal_get_ttcfg_t_presc(void);

void can_hal_set_ttcfg_ttif(uint32_t v);

uint32_t can_hal_get_ttcfg_ttif(void);

void can_hal_set_ttcfg_ttie(uint32_t v);

uint32_t can_hal_get_ttcfg_ttie(void);

void can_hal_set_ttcfg_teif(uint32_t v);

uint32_t can_hal_get_ttcfg_teif(void);

void can_hal_set_ttcfg_wtif(uint32_t v);

uint32_t can_hal_get_ttcfg_wtif(void);

void can_hal_set_ttcfg_wtie(uint32_t v);

uint32_t can_hal_get_ttcfg_wtie(void);
//reg :ref_msg

void can_hal_set_ref_msg_value(uint32_t v);

uint32_t can_hal_get_ref_msg_value(void);

void can_hal_set_ref_msg_id(uint32_t v);

uint32_t can_hal_get_ref_msg_id(void);

void can_hal_set_ref_msg_ide(uint32_t v);

uint32_t can_hal_get_ref_msg_ide(void);

//reg :trig_cfg0

void can_hal_set_trig_cfg_value(uint32_t v);

uint32_t can_hal_get_trig_cfg_value(void);

void can_hal_set_ttptr(uint32_t v);

uint32_t can_hal_get__ttptr(void);

//reg :

void can_hal_set_ttype(uint32_t v);

uint32_t can_hal_get_ttype(void);

void can_hal_set_tew(uint32_t v);

uint32_t can_hal_get_tew(void);

//reg :

void can_hal_set_tt_trig(uint32_t v);

uint32_t can_hal_get_tt_trig (void);


//reg : mem_port

void can_hal_set_mem_stat_value(uint32_t v);

uint32_t can_hal_get_mem_stat_value(void);

void can_hal_set_tt_wtrig(uint32_t v);

uint32_t can_hal_get_tt_wtrig(void);

void can_hal_set_mpen(uint32_t v);

uint32_t can_hal_get_mpen(void);

void can_hal_set_mdwie(uint32_t v);

uint32_t can_hal_get_mdwie(void);

void can_hal_set_mdwif(uint32_t v);

uint32_t can_hal_get_mdwif(void);

void can_hal_set_mdeif(uint32_t v);

uint32_t can_hal_get_mdeif(void);

void can_hal_set_maeif (uint32_t v);

uint32_t can_hal_get_maeif(void);

void can_hal_set_acfa (uint32_t v);

uint32_t can_hal_get_acfa(void);

uint32_t can_hal_get_txs(void);

uint32_t can_hal_get_txb(void);

uint32_t can_hal_get_heloc(void);

//reg :mem_es0

void can_hal_set_mem_es_value(uint32_t v);

uint32_t can_hal_get_mem_es_value(void);

void can_hal_set_mebp1 (uint32_t v);

uint32_t can_hal_get_mebp1 (void);

void can_hal_set_me1ee (uint32_t v);

uint32_t can_hal_get_me1ee(void);

void can_hal_set_meaee(uint32_t v);

uint32_t can_hal_get_meaee(void);
//reg :

void can_hal_set_mebp2(uint32_t v);

uint32_t can_hal_get_mebp2(void);

void can_hal_set_me2ee(uint32_t v);

uint32_t can_hal_get_me2ee(void);

//reg :mem_es1

void can_hal_set_meeec(uint32_t v);

uint32_t can_hal_get_meeec(void);

void can_hal_set_menec(uint32_t v);

uint32_t can_hal_get_menec(void);

//reg :mem_es3

void can_hal_set_mel(uint32_t v);

uint32_t can_hal_get_mel(void);

void can_hal_set_mes(uint32_t v);

uint32_t can_hal_get_mes(void);

//reg :scfg

void can_hal_set_scfg_value(uint32_t v);

uint32_t can_hal_get_scfg_value(void);

void can_hal_set_xmren(uint32_t v);

uint32_t can_hal_get_xmren(void);

void can_hal_set_seif(uint32_t v);

uint32_t can_hal_get_seif(void);

void can_hal_set_swie (uint32_t v);

uint32_t can_hal_get_swie (void);

void can_hal_set_swif(uint32_t v);

uint32_t can_hal_get_swif(void);

void can_hal_set_fstim(uint32_t v);

uint32_t can_hal_get_fstim(void);

//reg :can_fd

void can_hal_set_fd_enable(uint32_t v);

uint32_t can_hal_get_fd_enable(void);

void can_hal_error_analysis(uint32_t err_c);

void can_hal_receive_frame(void);

void can_hal_send_frame(void);

bk_err_t can_hal_ctrl(uint32_t cmd, void *parm);

void can_hal_bit_rate_config(can_bit_rate_e s_speed, can_bit_rate_e f_speed);

#if CFG_HAL_DEBUG_CAN
void can_struct_dump(uint32_t start, uint32_t end);
#else
#define can_struct_dump(start, end)
#endif

#ifdef __cplusplus
}
#endif

